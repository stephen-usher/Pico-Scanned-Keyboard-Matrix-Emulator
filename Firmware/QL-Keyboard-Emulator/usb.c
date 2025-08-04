// Based upon hid_app.c from https://github.com/a-h/pico_usb_kbd_mouse
#include <stdio.h>
#include "pico/stdlib.h"

#include "bsp/board.h"
#include "tusb.h"

#include "qlkeyboard.h"
#include "qlkeydecode.h"

#define DEBUG 0

static void process_kbd_report(hid_keyboard_report_t const *report);
static void process_mouse_report(hid_mouse_report_t const * report);

// tuh_hid_mount_cb is executed when a new device is mounted.
void tuh_hid_mount_cb(uint8_t dev_addr, uint8_t instance, uint8_t const* desc_report, uint16_t desc_len)
{
	uint8_t const itf_protocol = tuh_hid_interface_protocol(dev_addr, instance);
	if(itf_protocol == HID_ITF_PROTOCOL_NONE) {
#if DEBUG > 0
		printf("Device with address %d, instance %d is not a keyboard or mouse.\r\n", dev_addr, instance);
#endif
		return;
	}
	const char* protocol_str[] = { "None", "Keyboard", "Mouse" };
#if DEBUG > 0
	printf("Device with address %d, instance %d is a %s.\r\n", dev_addr, instance, protocol_str[itf_protocol]);
#endif

	// request to receive report
  	// tuh_hid_report_received_cb() will be invoked when report is available
  	if ( !tuh_hid_receive_report(dev_addr, instance) )
  	{
#if DEBUG > 0
    		printf("Error: cannot request to receive report\r\n");
#else
		instance = instance;
#endif
    	}
}

// tuh_hid_report_received_cb is executed when data is received from the keyboard or mouse.
void tuh_hid_report_received_cb(uint8_t dev_addr, uint8_t instance, uint8_t const* report, uint16_t len)
{
#if DEBUG > 0
	printf("Report from HID USB device: Dev address: %d Instance %d\n");
#endif
	
	switch(tuh_hid_interface_protocol(dev_addr, instance))
	{
	case HID_ITF_PROTOCOL_KEYBOARD:
		process_kbd_report((hid_keyboard_report_t const*) report);
		break;
	}

	// request to receive next report
  	// tuh_hid_report_received_cb() will be invoked when report is available
  	if ( !tuh_hid_receive_report(dev_addr, instance) )
  	{
#if DEBUG > 0
    		printf("Error: cannot request to receive report\r\n");
#else
		instance = instance;
#endif
  	}
}

// tuh_hid_umount_cb is executed when a device is unmounted.
void tuh_hid_umount_cb(uint8_t dev_addr, uint8_t instance)
{
#if DEBUG > 0
	printf("Device with address %d, instance %d was unmounted.\r\n", dev_addr, instance);
#else
	instance = instance;
#endif
}

extern hid_keyboard_report_t current_report;
extern int usb_report_ready;

static void process_kbd_report(hid_keyboard_report_t const *report)
{
	current_report = *report;
	usb_report_ready = 1;
}
