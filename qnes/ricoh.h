#ifndef _RICOH_H
#define _RICOH_H

#include "defs.h"

typedef union {
	unsigned char byte;
	struct {
		unsigned char carry : 1;
		unsigned char zero : 1;
		unsigned char irq : 1;
		unsigned char decimal : 1;
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

typedef void (*instructionHandler)(R2A03 *);

void initialize(R2A03 *, void *);
unsigned char fetch_next_byte(R2A03 *);
unsigned char fetch_byte(R2A03 *, unsigned short);
unsigned short fetch_next_word(R2A03 *);
unsigned short fetch_word(R2A03 *, unsigned short);
void cycle(R2A03 *);


///////////////////////////////////////////////////////////////////
//                 INSTRUCTION HANDLER DEFS                      //
///////////////////////////////////////////////////////////////////

void ih_brk(R2A03 *);

void ih_ora_imm(R2A03 *);
void ih_ora_izx(R2A03 *);
void ih_ora_izy(R2A03 *);
void ih_ora_zp(R2A03 *);
void ih_ora_zpx(R2A03 *);
void ih_ora_abx(R2A03 *);
void ih_ora_aby(R2A03 *);
void ih_ora_abs(R2A03 *);
void ih_ora_imm(R2A03 *);

void ih_and_imm(R2A03 *);
void ih_and_izx(R2A03 *);
void ih_and_izy(R2A03 *);
void ih_and_zp(R2A03 *);
void ih_and_zpx(R2A03 *);
void ih_and_abx(R2A03 *);
void ih_and_aby(R2A03 *);
void ih_and_abs(R2A03 *);
void ih_and_imm(R2A03 *);

void ih_eor_imm(R2A03 *);
void ih_eor_izx(R2A03 *);
void ih_eor_izy(R2A03 *);
void ih_eor_zp(R2A03 *);
void ih_eor_zpx(R2A03 *);
void ih_eor_abx(R2A03 *);
void ih_eor_aby(R2A03 *);
void ih_eor_abs(R2A03 *);
void ih_eor_imm(R2A03 *);

#endif