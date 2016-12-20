#include "drawille.h"

//		1110 xxxx : 10xx xxxx : 10xx xxxx	-	UTF-8
//		0x2800 - braille unicode offset
//		0010 1000 : 0000 0000
void draw(char uni) {	//unicode to UTF-8
	uart_sendData(0xE2);	//First byte must be 0xE2
	uart_sendData(0xA0 | ((uni>>6) & 0b00000011));
	uart_showData(0x80 | (uni & 0b00111111));
}
