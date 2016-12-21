#include "cli.h"
#include "tty.h"
//	Command Line Interface

//		1110 xxxx : 10xx xxxx : 10xx xxxx	-	UTF-8
//		0x2800 - braille unicode offset
//		0010 1000 : 0000 0000

const char cli_bitmap[BLOCK_HEIGHT][BLOCK_WIDTH] = {
	{0x01, 0x08},
	{0x02, 0x10},
	{0x04, 0x20},
	{0x40, 0x80}
};

unsigned char cli_cache[CACHE_HEIGHT][CACHE_WIDTH] = {};


void cli_fresh() {
	for(unsigned char x = 0; x < CACHE_WIDTH; x++) {
		for(unsigned char y = 0; y < CACHE_HEIGHT; y++) {
			cli_drawBlock(&cli_cache[y][x]);
		}
		UART_CR();
	}
}

void cli_drawBlock(unsigned char *uni) {	//unicode to UTF-8
	uart_sendData(0xE2);	//First byte must be 0xE2
	uart_sendData(0xA0 | ((*uni>>6) & 0b00000011));
	uart_sendData(0x80 | (*uni & 0b00111111));
}

void cli_drawSpot(unsigned char x, unsigned char y) {
	if((x < WIDTH)&&(y < HEIGHT)) {
		//	Get offset of block
		unsigned char x_offset = x % BLOCK_WIDTH;
		unsigned char y_offset = y % BLOCK_HEIGHT;
		//	Get position of block
		x /= BLOCK_WIDTH;
		y /= BLOCK_HEIGHT;
		cli_cache[y][x] |= cli_bitmap[y_offset][x_offset];
	}
}
