This is the Pico firmware to emulate a QL keyboard matrix.



This can either be used via a serial connection or, with the addition of a USB OTG (On The Go) cable, a USB keyboard.



For the USB keyboard the code can be configured for either a UK keyboard or a US one. For other languages a rather more radical redefinition of shifted keys would be required and possibly the addition of more USB keycodes to the table.

It should be noted that if the board is only going to be used with a QL and only with a USB keyboard then the second 74VLC244 and all the serial port parts (MAX232, capacitors and DB9 port) can be omitted from the PCB and only 5V and GND power is required.



You will need to replace the two keyboard ribbon cable sockets (J11 and J12) on the QL motherboard with pin headers. Run two 8 way ribbon cables with Dupont connectors on the end from the two connectors on this board to the motherboard. The left (scan lines) plug into pins 1 to 8 of J11, pin 1 goes to pin 1. The right (sense lines) go from pin 3 to pin 10 of J12, pin 1 on this board connects to pin 3 on J12 etc.



