#include <stdio.h>
#include <pico/stdlib.h>

#include "bsp/board.h"
#include "tusb.h"

#include "qlkeydecode.h"

#define KB_LANG	KB_GB

struct qlkeycode ascii2qlcodes[] =
{
	{8,4,MOD_CTRL},	/* 0 NUL */
	{4,3,MOD_CTRL},	/* 1 SOH */
	{6,3,MOD_CTRL},	/* 2 STX */
	{6,2,MOD_CTRL},	/* 3 ETX */
	{4,4,MOD_CTRL},	/* 4 EOT */
	{2,3,MOD_CTRL},	/* 5 ENQ */
	{5,3,MOD_CTRL},	/* 6 ACK */
	{5,4,MOD_CTRL},	/* 7 BEL */
	{8,1,MOD_CTRL},	/* 8 Backspace */
	{3,2,MOD_NONE},	/* 9 TAB */
	{4,6,MOD_CTRL},	/* 10 Linefeed */
	{5,5,MOD_CTRL},	/* 11 Vertical Tab */
	{4,7,MOD_CTRL},	/* 12 Form Feed */
	{8,7,MOD_NONE},	/* 13 Carriage Return */
	{7,4,MOD_CTRL},	/* 14 SO */
	{3.6,MOD_CTRL},	/* 15 SI */
	{4,8,MOD_CTRL},	/* 16 DLE */
	{2,2,MOD_CTRL},	/* 17 DC1 */
	{3,3,MOD_CTRL},	/* 18 DC2 */
	{5,2,MOD_CTRL},	/* 19 DC3 */
	{2,4,MOD_CTRL},	/* 20 DC4 */
	{2,6,MOD_CTRL},	/* 21 NAK */
	{7,3,MOD_CTRL},	/* 22 SYN */
	{3,1,MOD_CTRL},	/* 23 ETB */
	{7,2,MOD_CTRL},	/* 24 CAN */
	{3,4,MOD_CTRL},	/* 25 EM */
	{6,1,MOD_CTRL},	/* 26 SUB */
	{8,2,MOD_NONE},	/* 27 ESC */
	{8,8,MOD_CTRL},	/* 28 FS */
	{6,7,MOD_CTRL},	/* 29 GS */
	{2,5,MOD_CTRL},	/* 30 RS */
	{3,8,MOD_CTRL},	/* 31 US */
	{8,4,MOD_NONE},	/* 32   */
	{4,2,MOD_SHFT},	/* 33 ! */
	{6,6,MOD_SHFT},	/* 34 " */
	{4,1,MOD_SHFT},	/* 35 # */
	{1,4,MOD_SHFT},	/* 36 $ */
	{1,2,MOD_SHFT},	/* 37 % */
	{1,6,MOD_SHFT},	/* 38 & */
	{6,6,MOD_NONE},	/* 39 ' */
	{3,6,MOD_SHFT},	/* 40 ( */
	{2,8,MOD_SHFT},	/* 41 ) */
	{2,7,MOD_SHFT},	/* 42 * */
	{5,8,MOD_SHFT},	/* 43 + */
	{7,6,MOD_NONE},	/* 44 ,	*/
	{3,8,MOD_NONE},	/* 45 - */
	{6,5,MOD_NONE},	/* 46 . */
	{7,8,MOD_NONE},	/* 47 / */
	{2,8,MOD_NONE},	/* 48 0 */
	{4,2,MOD_NONE},	/* 49 1 */
	{2,1,MOD_NONE},	/* 50 2 */
	{4,1,MOD_NONE},	/* 51 3 */
	{1,4,MOD_NONE},	/* 52 4 */
	{1,5,MOD_NONE},	/* 53 5 */
	{2,5,MOD_NONE},	/* 54 6 */
	{1,6,MOD_NONE},	/* 55 7 */
	{2,7,MOD_NONE},	/* 56 8 */
	{3,7,MOD_NONE},	/* 57 9 */
	{5,6,MOD_SHFT},	/* 58 : */
	{5,6,MOD_NONE},	/* 59 ; */
	{7,6,MOD_SHFT},	/* 60 < */
	{5,8,MOD_NONE},	/* 61 = */
	{6,5,MOD_SHFT},	/* 62 > */
	{7,8,MOD_SHFT},	/* 63 ? */
	{2,1,MOD_SHFT},	/* 64 @ */
	{4,3,MOD_SHFT},	/* 65 A */
	{6,3,MOD_SHFT},	/* 66 B */
	{6,2,MOD_SHFT},	/* 67 C */
	{4,4,MOD_SHFT},	/* 68 D */
	{2,3,MOD_SHFT},	/* 69 E */
	{5,3,MOD_SHFT},	/* 70 F */
	{5,4,MOD_SHFT},	/* 71 G */
	{4,5,MOD_SHFT},	/* 72 H */
	{3,5,MOD_SHFT},	/* 73 I */
	{4,6,MOD_SHFT},	/* 74 J */
	{5,5,MOD_SHFT},	/* 75 K */
	{4,7,MOD_SHFT},	/* 76 L */
	{6,4,MOD_SHFT},	/* 77 M */
	{7,4,MOD_SHFT},	/* 78 N */
	{3,6,MOD_SHFT},	/* 79 O */
	{4,8,MOD_SHFT},	/* 80 P */
	{2,2,MOD_SHFT},	/* 81 Q */
	{3,3,MOD_SHFT},	/* 82 R */
	{5,2,MOD_SHFT},	/* 83 S */
	{2,4,MOD_SHFT},	/* 84 T */
	{2,6,MOD_SHFT},	/* 85 U */
	{7,3,MOD_SHFT},	/* 86 V */
	{3,1,MOD_SHFT},	/* 87 W */
	{7,2,MOD_SHFT},	/* 88 X */
	{3,4,MOD_SHFT},	/* 89 Y */
	{6,1,MOD_SHFT},	/* 90 Z */
	{5,7,MOD_NONE},	/* 91 [ */
	{8,8,MOD_NONE},	/* 92 \ */
	{6,7,MOD_NONE},	/* 93 ] */
	{2,5,MOD_SHFT},	/* 94 ^ */
	{3,8,MOD_SHFT},	/* 95 _ */
	{6,8,MOD_NONE},	/* 96 ` */
	{4,3,MOD_NONE},	/* 97 a */
	{6,3,MOD_NONE},	/* 98 b */
	{6,2,MOD_NONE},	/* 99 c */
	{4,4,MOD_NONE},	/* 100 d */
	{2,3,MOD_NONE},	/* 101 e */
	{5,3,MOD_NONE},	/* 102 f */
	{5,4,MOD_NONE},	/* 103 g */
	{4,5,MOD_NONE},	/* 104 h */
	{3,5,MOD_NONE},	/* 105 i */
	{4,6,MOD_NONE},	/* 106 j */
	{5,5,MOD_NONE},	/* 107 k */
	{4,7,MOD_NONE},	/* 108 l */
	{6,4,MOD_NONE},	/* 109 m */
	{7,4,MOD_NONE},	/* 110 n */
	{3,6,MOD_NONE},	/* 111 o */
	{4,8,MOD_NONE},	/* 112 p */
	{2,2,MOD_NONE},	/* 113 q */
	{3,3,MOD_NONE},	/* 114 r */
	{5,2,MOD_NONE},	/* 115 s */
	{2,4,MOD_NONE},	/* 116 t */
	{2,6,MOD_NONE},	/* 117 u */
	{7,3,MOD_NONE},	/* 118 v */
	{3,1,MOD_NONE},	/* 119 w */
	{7,2,MOD_NONE},	/* 120 x */
	{3,4,MOD_NONE},	/* 121 y */
	{6,1,MOD_NONE},	/* 122 z */
	{5,7,MOD_SHFT},	/* 123 { */
	{8,8,MOD_SHFT},	/* 124 | */
	{6,7,MOD_SHFT},	/* 125 } */
	{2,5,MOD_SHFT},	/* 126 ~ */
	{8,1,MOD_CTRL},	/* 127 Del */
	{0,0,MOD_NONE},	/* Nothing */
	{8,3,MOD_NONE},	/* Right arrow */
	{8,1,MOD_NONE},	/* Left arrow */
	{8,5,MOD_NONE},	/* Up arrow */
	{8,6,MOD_NONE},	/* Down arrow */
	{1,1,MOD_NONE},	/* F1 */
	{1,2,MOD_NONE},	/* F2 */
	{1,3,MOD_NONE},	/* F3 */
	{1,7,MOD_NONE},	/* F4 */
	{1,8,MOD_NONE}	/* F5 */
};

void ascii2ql(unsigned char chr, struct qlkeycode *keycode)
{
	if (chr > 137)
	{
		keycode->row = 0;
		keycode->column = 0;
		keycode->modifier = 0;
	}
	
#ifdef DEBUG
	printf("ascii2ql: Character is %d: %d %d %d\n", chr, ascii2qlcodes[chr].row, ascii2qlcodes[chr].column, ascii2qlcodes[chr].modifier);
#endif

	keycode->row = ascii2qlcodes[chr].row;
	keycode->column = ascii2qlcodes[chr].column;
	keycode->modifier = ascii2qlcodes[chr].modifier;
}

struct qlkeycode usb_keycodes [] = {
	{0,0,0}, /* HID_KEY_NONE */
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{4,3,0}, /* HID_KEY_A */
	{6,3,0}, /* HID_KEY_B */
	{6,2,0}, /* HID_KEY_C */
	{4,4,0}, /* HID_KEY_D */
	{2,3,0}, /* HID_KEY_E */
	{5,3,0}, /* HID_KEY_F */
	{5,4,0}, /* HID_KEY_G */
	{4,5,0}, /* HID_KEY_H */
	{3,5,0}, /* HID_KEY_I */
	{4,6,0}, /* HID_KEY_J */
	{5,5,0}, /* HID_KEY_K */
	{4,7,0}, /* HID_KEY_L */
	{6,4,0}, /* HID_KEY_M */
	{7,4,0}, /* HID_KEY_N */
	{3,6,0}, /* HID_KEY_O */
	{4,8,0}, /* HID_KEY_P */
	{2,2,0}, /* HID_KEY_Q */
	{3,3,0}, /* HID_KEY_R */
	{5,2,0}, /* HID_KEY_S */
	{2,4,0}, /* HID_KEY_T */
	{2,6,0}, /* HID_KEY_U */
	{7,3,0}, /* HID_KEY_V */
	{3,1,0}, /* HID_KEY_W */
	{7,2,0}, /* HID_KEY_X */
	{3,4,0}, /* HID_KEY_Y */
	{6,1,0}, /* HID_KEY_Z */
	{4,2,0}, /* HID_KEY_1 */
	{2,1,0}, /* HID_KEY_2 */
	{4,1,0}, /* HID_KEY_3 */
	{1,4,0}, /* HID_KEY_4 */
	{1,5,0}, /* HID_KEY_5 */
	{2,5,0}, /* HID_KEY_6 */
	{1,6,0}, /* HID_KEY_7 */
	{2,7,0}, /* HID_KEY_8 */
	{3.7,0}, /* HID_KEY_9 */
	{2,8,0}, /* HID_KEY_0 */
	{8,7,0}, /* HID_KEY_ENTER */
	{8,2,0}, /* HID_KEY_ESCAPE */
	{8,1,MOD_CTRL}, /* HID_KEY_BACKSPACE */
	{3,2,0}, /* HID_KEY_TAB */
	{8,4,0}, /* HID_KEY_SPACE */
	{3,8,0}, /* HID_KEY_MINUS */
	{5,8,0}, /* HID_KEY_EQUAL */
	{5,7,0}, /* HID_KEY_BRACKET_LEFT */
	{6,7,0}, /* HID_KEY_BRACKET_RIGHT */
	{8,8,0}, /* HID_KEY_BACKSLASH */
	{0,0,0},
	{5,6,0}, /* HID_KEY_SEMICOLON */
	{6,6,0}, /* HID_KEY_APOSTROPHE */
	{6,8,0}, /* HID_KEY_GRAVE */
	{7,6,0}, /* HID_KEY_COMMA */
	{6,5,0}, /* HID_KEY_PERIOD */
	{7,8,0}, /* HID_KEY_SLASH */
	{5,1,0}, /* HID_KEY_CAPS_LOCK */
	{1,1,0}, /* HID_KEY_F1 */
	{1,2,0}, /* HID_KEY_F2 */
	{1,3,0}, /* HID_KEY_F3 */
	{1,7,0}, /* HID_KEY_F4 */
	{1,8,0}, /* HID_KEY_F5 */
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{8,1,MOD_CTRL}, /* HID_KEY_DELETE */
	{0,0,0},
	{0,0,0},
	{8,3,0}, /* HID_KEY_ARROW_RIGHT */
	{8,1,0}, /* HID_KEY_ARROW_LEFT */
	{8,6,0}, /* HID_KEY_ARROW_DOWN */
	{8,5,0}, /* HID_KEY_ARROW_UP */
	{0,0,0}, /* HID_KEY_NUM_LOCK */
	{7,8,0}, /* HID_KEY_KEYPAD_DIVIDE */
	{2,7,MOD_SHFT}, /* HID_KEY_KEYPAD_MULTIPLY */
	{3,8,0}, /* HID_KEY_KEYPAD_SUBTRACT */
	{8,8,MOD_SHFT}, /* HID_KEY_KEYPAD_ADD */
	{8,7,0}, /* HID_KEY_KEYPAD_ENTER */
	{4,2,0}, /* HID_KEY_KEYPAD_1 */
	{2,1,0}, /* HID_KEY_KEYPAD_2 */
	{4,1,0}, /* HID_KEY_KEYPAD_3 */
	{1,4,0}, /* HID_KEY_KEYPAD_4 */
	{1,5,0}, /* HID_KEY_KEYPAD_5 */
	{2,5,0}, /* HID_KEY_KEYPAD_6 */
	{1,6,0}, /* HID_KEY_KEYPAD_7 */
	{2,7,0}, /* HID_KEY_KEYPAD_8 */
	{3,7,0}, /* HID_KEY_KEYPAD_9 */
	{2,8,0}, /* HID_KEY_KEYPAD_0 */
	{6,5,0}, /* HID_KEY_KEYPAD_DECIMAL */
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{5,8,0}, /* HID_KEY_KEYPAD_EQUAL */
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{7,6,0}, /* HID_KEY_KEYPAD_COMMA */
	{5,8,0}, /* HID_KEY_KEYPAD_EQUAL_SIGN */
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{8,7,0}, /* HID_KEY_RETURN */
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{3,7,MOD_SHFT}, /* HID_KEY_KEYPAD_LEFT_PARENTHESIS */
	{2,8,MOD_SHFT}, /* HID_KEY_KEYPAD_RIGHT_PARENTHESIS */
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0},
	{0,0,0}};


void usbkeycode2ql(int usb_keycode, int modifiers, struct qlkeycode *keycode)
{
	struct qlkeycode mykeycode;

	mykeycode = usb_keycodes[usb_keycode];

	keycode->row = mykeycode.row;
	keycode->column = mykeycode.column;
	keycode->modifier = (mykeycode.modifier | modifiers);

// If shift isn't pressed then our job is done as we have a direct translation.

	if ((modifiers & MOD_SHFT) == 0)
		return;

/*
 *	Now we have to deal with shifted keys! Tricky due to keyboard languages.
 *
 *	Although the USB HID standard defines a way for hosts to enquire the keyboard
 *	language almost none of the hardware manufacturers implemented it, so we have
 *	to have it hard coded.
 */

// Alpha keys are easy, we can just return what we have.

	if ((usb_keycode >= HID_KEY_A) && (usb_keycode <= HID_KEY_Z))
		return;

// Now we can do a switch on the other keys.

	switch (usb_keycode)
	{
#if KB_LANG == KB_GB
	case HID_KEY_2:
		keycode->row = 6;
		keycode->column = 6;
		break;
	case HID_KEY_APOSTROPHE:
		keycode->row = 2;
		keycode->column = 1;
#endif
	}

	return;
}
