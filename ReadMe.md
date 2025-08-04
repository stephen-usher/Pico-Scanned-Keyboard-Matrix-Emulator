This is a generic scanned keyboard matrix emulation board based upon a Raspberry Pi Pico.

It supports any scanned keyboard matrix which has up to 8 scan lines (outputs from the computer) and 12 sense lines (inputs to the computer).

The board itself is really just a Pico interface board with 8 TTL compatible inputs to the GPIO and 12 TTL compatible outputs with an RS232 compatible interface to the default UART. The 8 and 12 pin ports are contiguous ranges on the Pico allowing PIO operations as a bus. All the keyboard matrix magic happens in software. It's using 74VLC244 chips to act as step-down voltage converted on the input and the 74LS244 as a step-up on the output.

The initial firmware supports the Sinclair QL but I will later probably add the Memotech MTX Series and maybe ZX Spectrum and ZX81.

To use the board with a USB keyboard you will need to add a USB OTG (On The Go) adapter cable and set the Pico SDK to use the serial port for stdio so as to use the TinyUSB library.

Note: Because I didn't realise that the Pico doesn't drive the VBUS power line when powered from the VSYS line a bodge wire will be needed to bridge between the two pins on the Pico otherwise the output 74LS244 doesn't get power.
