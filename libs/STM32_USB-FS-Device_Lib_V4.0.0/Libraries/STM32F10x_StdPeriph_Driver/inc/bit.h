#pragma once
#ifndef _BIT_H_
#define _BIT_H_

#define BITBAND(addr, bitnum) ((addr & 0xF0000000) + 0x2000000 + ((addr & 0xFFFFF) << 5) + (bitnum<<2))
#define BIT_ADDR(addr, bitnum) *((volatile unsigned long *)(BITBAND(addr, bitnum)))

#endif
