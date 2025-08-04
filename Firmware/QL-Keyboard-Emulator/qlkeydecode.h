#define KB_GB	1
#define KB_US	2

#define MOD_NONE	0
#define MOD_SHFT	0x40
#define MOD_CTRL	0x01
#define MOD_ALT		0x10

struct qlkeycode 
{
	unsigned char row;
	unsigned char column;
	unsigned char modifier;
};

#define MOD_ROW 6

void ascii2ql(unsigned char chr, struct qlkeycode *keycode);
void usbkeycode2ql(int usb_keycode, int modifiers, struct qlkeycode *keycode);
