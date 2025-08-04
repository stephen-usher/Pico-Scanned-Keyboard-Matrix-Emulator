#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "pico/multicore.h"

#include "qlkeyboard.h"

#include "qlkeydecode.h"

unsigned char qlcodes[8] = {0, 0, 0, 0, 0, 0, 0, 0};
unsigned int codereadsem = 0;

int main()
{
	stdio_init_all();

	multicore_launch_core1(bus_io_handler);

	input_read_process();
}
