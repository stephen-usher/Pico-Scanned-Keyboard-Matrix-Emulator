#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/dma.h"
#include "hardware/pio.h"

#include "bsp/board.h"
#include "tusb.h"

#define DEBUG 0

#include "qlkeyboard.h"

#include "qlkeydecode.h"

#define INPUT_NOINPUT 0
#define INPUT_SERIAL 1
#define INPUT_USB 2

int read_input_devices(struct qlkeycode *keycode);
int read_serial_device(struct qlkeycode *keycode);
int read_usb_device(struct qlkeycode *keycode);
void setup_input_devices();

extern volatile unsigned char qlcodes[];

extern PIO pio_1;
extern uint sm_1;

static int last_key_valid = 0;

void input_read_process()
{
	int i, y;
	int valid_keycode, keycodeval, oldkeycodeval;
	struct qlkeycode keycode;;
	unsigned char myqlcodes[8] = {0, 0, 0, 0, 0, 0, 0, 0};

	sleep_ms(10);	// Make sure that the PIO instances have been set up by the other core.

	setup_input_devices();

	while(1)
	{
		keycode.row = 0;
		keycode.column = 0;
		keycode.modifier = 0;
		
		valid_keycode = read_input_devices(&keycode);
#if DEBUG > 2
		printf("valid_keycode = %d\n", valid_keycode);
#endif

		for (i = 0; i < 8; i++)
		{
			myqlcodes[i] = 0;
		}

		keycodeval = (keycode.row << 16) + (keycode.column << 8) + keycode.modifier;

		if (valid_keycode == INPUT_NOINPUT)
		{
			continue;
		}

#if DEBUG > 1
		printf("oldkeycodeval = %06x, keycodeval = %06x\n", oldkeycodeval, keycodeval);
#endif
		if (keycodeval == oldkeycodeval)
			continue;

		oldkeycodeval = keycodeval;
#if DEBUG > 1
		printf("input_read_process[0]: Got keycode: Row: %d, column: %d, modifier: %d\n", keycode.row, keycode.column, keycode.modifier);

		printf("input_read_process[0]: last key %s valid.\n", (last_key_valid ? "was" : "was not"));
#endif

// Ignore keys or characters which have no QL equivalent.

		if ((keycode.row == 0) && (keycode.modifier == 0) && (last_key_valid == 0))
		{
#if DEBUG > 1
			printf("input_read_process[0]: Not a QL keyboard character.\n");
#endif

			for (i = 0; i < 8; i++)
				qlcodes[i] = myqlcodes[i];

			last_key_valid = 0;
			continue;
		}

		last_key_valid = 1;

		myqlcodes[MOD_ROW] = keycode.modifier;

		if ((keycode.modifier != 0) && (valid_keycode == INPUT_SERIAL))
		{
#if DEBUG > 1
			printf("Modifier set over serial doing stuff.\n");
#endif
			for (i = 0; i < 8; i++)
				qlcodes[i] = 0;
			qlcodes[MOD_ROW] = keycode.modifier;
			sleep_ms(1);
		}
		
		for (i = 1; i < 9; i++)
		{
			y = i - 1;
			if (keycode.row == i)
			{
#if DEBUG > 1
				printf("input_read_process[0]: keycode.row (%d) matches i (%d)\n", keycode.row, i);
#endif

				myqlcodes[y] |= 1 << (keycode.column - 1);
#if DEBUG > 1
				printf("input_read_process[0]: qlcodes[%d] = %d keycode.column = %d\n", y, myqlcodes[y], keycode.column);
#endif

			}
#if DEBUG > 2
			else
			{
				printf("input_read_process[0]: keycode.row (%d) doesn't match i (%d)\n", keycode.row, i);
			}
#endif
#if DEBUG > 1
			printf("input_read_process[0]: qlcodes[%d] = %02x\n", y, myqlcodes[y]);
#endif
		}

		for (i = 0; i < 8; i++)
			qlcodes[i] = myqlcodes[i];

#if DEBUG > 0
		printf("input_read_process[0]: Scan line output values:");
#endif


#if DEBUG > 0
		for (i = 0; i < 8; i++)
			printf(" %02x", qlcodes[i]);
#endif


#if DEBUG > 0
		printf("\n");
#endif

	}
}

/*
 * Read the serial port and USB keyboard and return a QL keycode structure.
 */

#define SPECIAL_RIGHT	1
#define SPECIAL_LEFT	2
#define SPECIAL_UP	3
#define SPECIAL_DOWN	4
#define SPECIAL_F1	5
#define SPECIAL_F2	6
#define SPECIAL_F3	7
#define SPECIAL_F4	8
#define SPECIAL_F5	9

#define PEND_TOUT 100

int read_input_devices(struct qlkeycode *keycode)
{
	struct qlkeycode serial_keycode, usb_keycode;

	sleep_ms(1);
	
	if ( read_serial_device(keycode) > 0 )
	{
#if DEBUG > 0
		printf("read_input_devices: Input from serial port detected.\n");
#endif
		return INPUT_SERIAL;
	}
	
	if ( read_usb_device(keycode) > 0 )
	{
#if DEBUG > 0
		printf("read_input_devices: Input from usb port detected.\n");
#endif
		return INPUT_USB;
	}

	return INPUT_NOINPUT;
}

hid_keyboard_report_t current_report;
int usb_report_ready = 0;
int processing_modifier = 0;

int read_usb_device(struct qlkeycode *keycode)
{
	int modifier = 0;
	int tmp_modifier = 0;
	int is_shifted, is_alt, is_control;
	static int usb_keycode;
	static hid_keyboard_report_t report;
	static struct qlkeycode oldkeycode = {0, 0, 0};
	
	if (processing_modifier == 0)
	{
		tuh_task();
	
		if (usb_report_ready == 0)
		{
			keycode->row = oldkeycode.row;
			keycode->column = oldkeycode.column;
			keycode->modifier = oldkeycode.modifier;
			return 0;
		}

		report = current_report;

		usb_report_ready = 0;
	}


// Only bothering with the first key press.

	usb_keycode = report.keycode[0];

	is_shifted = report.modifier & (KEYBOARD_MODIFIER_LEFTSHIFT | KEYBOARD_MODIFIER_RIGHTSHIFT);
	is_alt = report.modifier & (KEYBOARD_MODIFIER_LEFTALT | KEYBOARD_MODIFIER_RIGHTALT);
	is_control = report.modifier & (KEYBOARD_MODIFIER_LEFTCTRL | KEYBOARD_MODIFIER_RIGHTCTRL);

#if DEBUG > 0
	printf("Got USB keyboard input. Keycode %02x, is %s shifted, is %s ALT'd, is %s CTRL'd\n", usb_keycode, (is_shifted ? "": "not"), (is_alt ? "": "not"), (is_control ? "": "not"));
#endif

	modifier = 0;

	if (is_shifted)
		modifier |= MOD_SHFT;

	if (is_alt)
		modifier |= MOD_ALT;

	if (is_control)
		modifier |= MOD_CTRL;

	if (usb_keycode == 0)
	{
		keycode->row = 0;
		keycode->column = 0;
		keycode->modifier = modifier;
	}
	else
	{
		usbkeycode2ql(usb_keycode, modifier, keycode);
	}

	if ((processing_modifier == 0) && (keycode->modifier != 0))
	{
		processing_modifier = 1;

		keycode->row = 0;
		keycode->column = 0;
	}
	else
	{
		processing_modifier = 0;
	}

	return INPUT_USB;
}

int last_character_ok = 0;

int read_serial_device(struct qlkeycode *keycode)
{
	unsigned char character, char2;
	int pending = 0;
	int pendingptr = 0;
	int specialkey = 0;
	unsigned char pending_chars[12];
	struct qlkeycode mykeycode;

	specialkey = 0;

	keycode->row = 0;
	keycode->column = 0;
	keycode->modifier = 0;

	if (pending == 0)
	{
#if DEBUG > 2
		printf("Nothing pending.\n");
#endif
		if ((character = getchar_timeout_us(0)) >= 0xfe)
		{
#if DEBUG > 2
		printf("Character received (%02x) invalid, returning.\n", character);
#endif
			if (last_character_ok == 1)
			{
#if DEBUG > 0
		printf("Character received (%02x) invalid, but last character OK.\n", character);
#endif
				last_character_ok = 0;
				return 1;
			}
			else
			{
				return 0;
			}
		}

		last_character_ok = 1;
		
#if DEBUG > 0
		printf("Got character %d (%c) Processing.\n", character, character);
#endif
		if (character == 27)
		{
#if DEBUG > 0
			printf("Got ESC Processing.\n");
#endif
			pending_chars[pending++] = 27;
			if ((pending_chars[pending] = getchar_timeout_us(PEND_TOUT)) < 0x7e)
			{
#if DEBUG > 0
				printf("Got character %d (%c) Processing.\n", pending_chars[pending], pending_chars[pending]);
#endif
				switch (pending_chars[pending])
				{
				case '6':
#if DEBUG > 0
					printf("Got F5, ending.\n");
#endif
					specialkey = SPECIAL_F5;
					pending = 0;
					break;
				case '[':
				case 'O':
#if DEBUG > 0
					printf("Got [ or O Processing.\n");
#endif
					pending++;
					if ((pending_chars[pending] = getchar_timeout_us(PEND_TOUT)) < 0x7e)
					{
#if DEBUG > 0
						printf("Got character %d (%c) Processing.\n", pending_chars[pending], pending_chars[pending]);
#endif
						switch (pending_chars[pending])
						{
						case 'A':
							specialkey = SPECIAL_UP;
							pending = 0;
							break;
						case 'B':
							specialkey = SPECIAL_DOWN;
							pending = 0;
							break;
						case 'C':
							specialkey = SPECIAL_RIGHT;
							pending = 0;
							break;
						case 'D':
							specialkey = SPECIAL_LEFT;
							pending = 0;
							break;
						case 'P':
							specialkey = SPECIAL_F1;
							pending = 0;
							break;
						case 'Q':
							specialkey = SPECIAL_F2;
							pending = 0;
							break;
						case 'R':
							specialkey = SPECIAL_F3;
							pending = 0;
							break;
						case 'S':
							specialkey = SPECIAL_F4;
							pending = 0;
							break;
						case '1':
							pending++;
							if ((pending_chars[pending] = getchar_timeout_us(PEND_TOUT)) < 0x7e)
							{
#if DEBUG > 0
								printf("Got character %d (%c) Processing.\n", pending_chars[pending], pending_chars[pending]);
#endif
								switch (pending_chars[pending])
								{
								case '6':
									specialkey = SPECIAL_F5;
									pending = 0;
								default:
									pending = 0;
								}
								getchar_timeout_us(PEND_TOUT);
								getchar_timeout_us(PEND_TOUT);
							}
							break;
						default:
							pending++;
						}
					}
					break;
				default:
//					pending++;
				}
			}
		}
	}
	else
	{
#if DEBUG > 0
		printf("Pending > 0: %d\n", pending);
#endif
		character = pending_chars[pendingptr++];
		if (pendingptr == pending)
		{
			pending = 0;
			pendingptr = 0;
		}
	}

	if (character < 128)
	{
		if (specialkey != 0)
		{
			character = 128 + specialkey;
			specialkey = 0;
		}

#if DEBUG > 0
		printf("read_input_devices: Character from serial port is: %02x\n", character);
#endif

		ascii2ql(character, keycode);
	}
	
	return 1;
}

/*
 * Do any setting up required for the serial input or USB.
 */

void setup_input_devices()
{
	board_init();
	tusb_init();
	return;
}
