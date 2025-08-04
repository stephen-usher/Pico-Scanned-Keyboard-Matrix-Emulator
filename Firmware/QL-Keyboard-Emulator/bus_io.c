#include <stdio.h>
#include <pico/stdlib.h>
#include <hardware/dma.h>
#include <hardware/pio.h>
#include <hardware/gpio.h>

#define NODEBUG

#include "input.pio.h"
#include "output.pio.h"

#include "qlkeyboard.h"

#include "qlkeydecode.h"

extern volatile unsigned char qlcodes[];
extern volatile unsigned int codereadsem;

PIO pio_0, pio_1;
uint sm_0, sm_1;

static unsigned long input_state;
static unsigned char input_state_new = 0;

#ifdef USE_INTERRUPTS

void input_interrupt_handler()
{
#ifdef DEBUG
	printf("Interrupt!\n");
#endif	
	input_state = pio_sm_get(pio0, sm_0);
	pio_interrupt_clear(pio0, 0);
	input_state_new = 1;
}

#endif

void bus_io_handler()
{
	int i, input_index, codeval;
	unsigned char codereadsemcopy;
	uint msg;
	
	pio_0 = pio0;
	uint offset0 = pio_add_program(pio_0, &input_program);
#ifdef DEBUG
	printf("Loaded program input at %d\n", offset0);
#endif	
	sm_0 = pio_claim_unused_sm(pio_0, true);


	input_program_init(pio_0, sm_0, offset0, 2);

	pio_1 = pio1;
	uint offset1 = pio_add_program(pio_1, &output_program);
#ifdef DEBUG
	printf("Loaded program output at %d\n", offset1);
#endif	

	sm_1 = pio_claim_unused_sm(pio_1, true);
	output_program_init(pio_1, sm_1, offset1, 10);

#ifdef DEBUG
	printf("Debug:\npio0->ctrl %x\npio0->fdebug %x\npio0->fstat %x\npio0->flevel %x\n",
	       pio0->ctrl,
	       pio0->fdebug,
	       pio0->fstat,
	       pio0->flevel);
#endif	

#ifdef USE_INTERRUPTS
	pio_set_irq0_source_enabled(pio_0, pis_interrupt0, true);

	irq_set_exclusive_handler(PIO0_IRQ_0, input_interrupt_handler);
	irq_set_enabled(PIO0_IRQ_0, true);
#endif

	pio_sm_put(pio_1, sm_1, 0);

#ifdef DEBUG
	printf("bus_io[1]: Entering loop.\n");
#endif	

	while(1)
	{
#ifdef USE_INTERRUPTS
		while (input_state_new == 0) i = i;
#else
		input_state = pio_sm_get_blocking(pio_0,sm_0);
#endif

		codereadsemcopy = codereadsem;

		if (codereadsemcopy > 0)
			codereadsemcopy--;

#ifdef DEBUG
//		printf("bus_io[1]: New value from bus scan: %08x\n", input_state);
#endif		

		input_index = 0;

		if (input_state & 0x01)
			input_index = 0;
		if (input_state & 0x02)
			input_index = 1;
		if (input_state & 0x04)
			input_index = 2;
		if (input_state & 0x08)
			input_index = 3;
		if (input_state & 0x10)
			input_index = 4;
		if (input_state & 0x20)
			input_index = 5;
		if (input_state & 0x40)
			input_index = 6;
		if (input_state & 0x80)
			input_index = 7;

		codeval = qlcodes[input_index];

//		msg = (codeval & 0x80) ? 0xf0000000 : 0 |
//			(codeval & 0x40) ? 0x0f000000 : 0 |
//			(codeval & 0x20) ? 0x00f00000 : 0 |
//			(codeval & 0x10) ? 0x000f0000 : 0 |
//			(codeval & 0x08) ? 0x0000f000 : 0 |
//			(codeval & 0x04) ? 0x00000f00 : 0 |
//			(codeval & 0x04) ? 0x000000f0 : 0 |
//			(codeval & 0x01) ? 0x0000000f : 0;

		msg = codeval;

#ifdef DEBUG
//		printf("bus_io[1]: Triggered qlcodes[%d] = %02x\n", input_index, qlcodes[input_index]);

//		for (i = 0; i < 8; i++)
//			printf("bus_io[1]: qlcodes[%d] = %02x\n", i, qlcodes[i]);

//		printf("bus_io[1]: codereadsem = %d\n", codereadsemcopy);
		printf("bus_io[1]: input_index = %d msg = %08x\n", input_index, msg);
#endif		

		pio_sm_put_blocking(pio_1, sm_1, msg);

		codereadsem = codereadsemcopy;
	}
}
