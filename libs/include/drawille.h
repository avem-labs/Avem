#ifndef DRAWILLE_H
#define DRAWILLE_H

#include "uart.h"

//  ---
// |0 3|
// |1 4|
// |2 5|
// |6 7|
//  ---
// #define BRAILLE_OFFSET	0x2800
#define PIX(x)			(1<<(x))
// #define MAP(bit)		BRAILLE_OFFSET + (bit)

void draw(unsigned char uni);

#endif
