#ifndef _RICOH_H
#define _RICOH_H

#include <stdio.h>
#include "defs.h"

typedef union {
	unsigned char byte;
	struct {
		unsigned char carry : 1;
		unsigned char zero : 1;
		unsigned char irq : 1;
		unsigned char decimal : 1;		//unused in 2a03
		unsigned char brkflag : 1;
		unsigned char unused : 1;
		unsigned char overflow : 1;
		unsigned char negative : 1;
	} bit;
} status_t;

typedef struct {
	byte_t a,x,y,s;		//s=stack pointer
	status_t p;			//p=status register

	word_t pc;			//program counter

	void * memory;
} R2A03;

typedef void (__cdecl *instructionHandler)(R2A03 *);

void initialize(R2A03 *, void *);
unsigned char fetch_next_byte(R2A03 *);
unsigned char fetch_byte(R2A03 *, unsigned short);
unsigned short fetch_next_word(R2A03 *);
unsigned short fetch_word(R2A03 *, unsigned short);
void cycle(R2A03 *);


///////////////////////////////////////////////////////////////////
//                 INSTRUCTION HANDLER DEFS                      //
///////////////////////////////////////////////////////////////////

static const instructionHandler ih[256];

#endif