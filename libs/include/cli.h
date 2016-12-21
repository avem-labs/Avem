#ifndef CLI_H
#define CLI_H

#include "uart.h"

#define BLOCK_WIDTH		2
#define BLOCK_HEIGHT	8
#define WIDTH			40
#define HEIGHT			40
#define CACHE_WIDTH		WIDTH/BLOCK_WIDTH
#define CACHE_HEIGHT	HEIGHT/BLOCK_HEIGHT

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
