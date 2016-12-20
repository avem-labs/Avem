#ifndef DRAWILLE_H
#define DRAWILLE_H

#include "uart.h"

//  ---
// |0 3|
// |1 4|
// |2 5|
// |6 7|
//  ---
#define BRAILLE_OFFSET	0x2800
#define PIXEL(x)		1<<(x)
#define DRAW(bit)		BRAILLE_OFFSET + (bit)

#endif
