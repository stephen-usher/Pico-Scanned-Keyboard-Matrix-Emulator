This is the Pico firmware to emulate a QL keyboard matrix.

This can either be used via a serial connection or, with the addition of a USB OTG (On The Go) cable, a USB keyboard.

For the USB keyboard the code can be configured for either a UK keyboard or a US one. For other languages a rather more radical redefinition of shifted keys would be required and possibly the additon of more USB keycodes to the table.

It should be noted that if the board is only going to be used with a QL and only with a USB keyboard then the second 74VLC244 and all the serial port parts (MAX232, capacitors and DB9 port) can be omitted from the PCB and only 5V and GND power is required.
