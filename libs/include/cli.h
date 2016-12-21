#ifndef CLI_H
#define CLI_H

#include "uart.h"

#define BLOCK_WIDTH		2
#define BLOCK_HEIGHT	8
#define WIDTH			200
#define HEIGHT			160
#define GET_WIDTH(x)	(x)/BLOCK_WIDTH
#define GET_HEIGHT(x)	(x)/BLOCK_HEIGHT
#define CACHE_WIDTH		GET_WIDTH(WIDTH)
#define CACHE_HEIGHT	GET_HEIGHT(HEIGHT)

//  ---
// |0 3|
// |1 4|
// |2 5|
// |6 7|
//  ---
// #define BRAILLE_OFFSET	0x2800
// #define PIX(x)			(1<<(x))
// #define MAP(bit)		BRAILLE_OFFSET + (bit)

void cli_fresh();
void cli_drawBlock(unsigned char *uni);
void cli_drawSpot(unsigned char x, unsigned char y);


#endif
