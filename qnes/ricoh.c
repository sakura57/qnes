#include "ricoh.h"

static const instructionHandler ih[256] = {
//	00				01				02				03				04				05				06				07
	&ih_brk,		&ih_ora_izx,	&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,
//	08				09				0A				0B				0C				0D				0E				0F
	&ih_brk,		&ih_ora_imm,	&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,
//	10				11				12				13				14				15				16				17
	&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,
//	18				19				1A				1B				1C				1D				1E				1F
	&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,
//	20				21				22				23				24				25				26				27
	&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,
//	28				29				2A				2B				2C				2D				2E				2F
	&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,
//	30				31				32				33				34				35				36				37
	&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,
//	38				39				3A				3B				3C				3D				3E				3F
	&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,
//	40				41				42				43				44				45				46				47
	&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,
//	48				49				4A				4B				4C				4D				4E				4F
	&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,
//	50				51				52				53				54				55				56				57
	&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,
//	58				59				5A				5B				5C				5D				5E				5F
	&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,
//	60				61				62				63				64				65				66				67
	&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,
//	68				69				6A				6B				6C				6D				6E				6F
	&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,
//	70				71				72				73				74				75				76				77
	&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,
//	78				79				7A				7B				7C				7D				7E				7F
	&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,
//	80				81				82				83				84				85				86				87
	&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,
//	88				89				8A				8B				8C				8D				8E				8F
	&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,
//	90				91				92				93				94				95				96				97
	&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,
//	98				99				9A				9B				9C				9D				9E				9F
	&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,
//	A0				A1				A2				A3				A4				A5				A6				A7
	&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,
//	A8				A9				AA				AB				AC				AD				AE				AF
	&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,
//	B0				B1				B2				B3				B4				B5				B6				B7
	&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,
//	B8				B9				BA				BB				BC				BD				BE				BF
	&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,
//	C0				C1				C2				C3				C4				C5				C6				C7
	&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,
//	C8				C9				CA				CB				CC				CD				CE				CF
	&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,
//	D0				D1				D2				D3				D4				D5				D6				D7
	&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,
//	D8				D9				DA				DB				DC				DD				DE				DF
	&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,
//	E0				E1				E2				E3				E4				E5				E6				E7
	&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,
//	E8				E9				EA				EB				EC				ED				EE				EF
	&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,
//	F0				F1				F2				F3				F4				F5				F6				F7
	&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,
//	F8				F9				FA				FB				FC				FD				FE				FF
	&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,		&ih_brk,
};

void initialize(R2A03 *proc, void *memory)
{
	proc->memory = memory;
	proc->a.byte = proc->x.byte = proc->y.byte = proc->s.byte = proc->p.byte = 0;
}

void cycle(R2A03 *proc)
{
	byte_t opcode;

	//fetch the byte at address PC
	opcode.byte = fetch_next_byte(proc);

	//execute the associated instruction handler
	ih[opcode.byte](proc);
}

unsigned char fetch_next_byte(R2A03 *proc)
{
	return ((unsigned char*)proc->memory)[proc->pc.word++];
}

unsigned char fetch_byte(R2A03 *proc, unsigned short addr)
{
	return ((unsigned char*)proc->memory)[addr];
}

unsigned short fetch_next_word(R2A03 *proc)
{
	return *(((unsigned short*)proc->memory) + proc->pc.word++);
}

unsigned short fetch_word(R2A03 *proc, unsigned short addr)
{
	return *(((unsigned short*)proc->memory) + addr);
}

///////////////////////////////////////////////////////////////
//                 INSTRUCTION HANDLERS                      //
///////////////////////////////////////////////////////////////

#define SETZ(x) proc->p.bit.zero = (x == 0)
#define SETN(x) proc->p.bit.negative = ((x & 0x80) == 0x80)


/*
	BRK handler
*/
void ih_brk(R2A03 *proc)
{
	//todo
}

///////////////////////////////////////////////////////////////
//                            ORA                            //
///////////////////////////////////////////////////////////////

/*
	ORA (imm) handler
*/
void ih_ora_imm(R2A03 *proc)
{
	unsigned char byte = fetch_next_byte(proc);
	unsigned char result = proc->a.byte | byte;
	SETZ(result);
	SETN(result);
	proc->a.byte = result;
}

/*
	ORA (izx) handler
*/
void ih_ora_izx(R2A03 *proc)
{
	unsigned char zp_addr = fetch_next_byte(proc) + proc->x.byte;
	unsigned short long_addr = fetch_word(proc, zp_addr);
	unsigned char byte = fetch_byte(proc, long_addr);
	unsigned char result = proc->a.byte | byte;
	SETZ(result);
	SETN(result);
	proc->a.byte = result;
}

/*
	ORA (izy) handler
*/
void ih_ora_izy(R2A03 *proc)
{
	unsigned char zp_addr = fetch_next_byte(proc);
	unsigned short addr = fetch_word(proc, zp_addr) + proc->y.byte;
	unsigned char byte = fetch_byte(proc, addr);
	unsigned char result = proc->a.byte | byte;
	SETZ(result);
	SETN(result);
	proc->a.byte = result;
}

/*
	ORA (abs) handler
*/
void ih_ora_abs(R2A03 *proc)
{
	unsigned short addr = fetch_next_word(proc);
	unsigned char byte = fetch_byte(proc, addr);
	unsigned char result = proc->a.byte | byte;
	SETZ(result);
	SETN(result);
	proc->a.byte = result;
}

/*
	ORA (abx) handler
*/
void ih_ora_abx(R2A03 *proc)
{
	unsigned short addr = fetch_next_word(proc) + proc->x.byte;
	unsigned char byte = fetch_byte(proc, addr);
	unsigned char result = proc->a.byte | byte;
	SETZ(result);
	SETN(result);
	proc->a.byte = result;
}

/*
	ORA (aby) handler
*/
void ih_ora_aby(R2A03 *proc)
{
	unsigned short addr = fetch_next_word(proc) + proc->y.byte;
	unsigned char byte = fetch_byte(proc, addr);
	unsigned char result = proc->a.byte | byte;
	SETZ(result);
	SETN(result);
	proc->a.byte = result;
}

/*
	ORA (zpx) handler
*/
void ih_ora_zpx(R2A03 *proc)
{
	unsigned char zp_addr = fetch_next_byte(proc) + proc->x.byte;
	unsigned char byte = fetch_byte(proc, (unsigned short)zp_addr);
	unsigned char result = proc->a.byte | byte;
	SETZ(result);
	SETN(result);
	proc->a.byte = result;
}

/*
	ORA (zp) handler
*/
void ih_ora_zp(R2A03 *proc)
{
	unsigned char zp_addr = fetch_next_byte(proc);
	unsigned char byte = fetch_byte(proc, (unsigned short)zp_addr);
	unsigned char result = proc->a.byte | byte;
	SETZ(result);
	SETN(result);
	proc->a.byte = result;
}

///////////////////////////////////////////////////////////////
//                            AND                            //
///////////////////////////////////////////////////////////////

/*
	and (imm) handler
*/
void ih_and_imm(R2A03 *proc)
{
	unsigned char byte = fetch_next_byte(proc);
	unsigned char result = proc->a.byte & byte;
	SETZ(result);
	SETN(result);
	proc->a.byte = result;
}

/*
	and (izx) handler
*/
void ih_and_izx(R2A03 *proc)
{
	unsigned char zp_addr = fetch_next_byte(proc) + proc->x.byte;
	unsigned short long_addr = fetch_word(proc, zp_addr);
	unsigned char byte = fetch_byte(proc, long_addr);
	unsigned char result = proc->a.byte & byte;
	SETZ(result);
	SETN(result);
	proc->a.byte = result;
}

/*
	and (izy) handler
*/
void ih_and_izy(R2A03 *proc)
{
	unsigned char zp_addr = fetch_next_byte(proc);
	unsigned short addr = fetch_word(proc, zp_addr) + proc->y.byte;
	unsigned char byte = fetch_byte(proc, addr);
	unsigned char result = proc->a.byte & byte;
	SETZ(result);
	SETN(result);
	proc->a.byte = result;
}

/*
	and (abs) handler
*/
void ih_and_abs(R2A03 *proc)
{
	unsigned short addr = fetch_next_word(proc);
	unsigned char byte = fetch_byte(proc, addr);
	unsigned char result = proc->a.byte & byte;
	SETZ(result);
	SETN(result);
	proc->a.byte = result;
}

/*
	and (abx) handler
*/
void ih_and_abx(R2A03 *proc)
{
	unsigned short addr = fetch_next_word(proc) + proc->x.byte;
	unsigned char byte = fetch_byte(proc, addr);
	unsigned char result = proc->a.byte & byte;
	SETZ(result);
	SETN(result);
	proc->a.byte = result;
}

/*
	and (aby) handler
*/
void ih_and_aby(R2A03 *proc)
{
	unsigned short addr = fetch_next_word(proc) + proc->y.byte;
	unsigned char byte = fetch_byte(proc, addr);
	unsigned char result = proc->a.byte & byte;
	SETZ(result);
	SETN(result);
	proc->a.byte = result;
}

/*
	and (zpx) handler
*/
void ih_and_zpx(R2A03 *proc)
{
	unsigned char zp_addr = fetch_next_byte(proc) + proc->x.byte;
	unsigned char byte = fetch_byte(proc, (unsigned short)zp_addr);
	unsigned char result = proc->a.byte & byte;
	SETZ(result);
	SETN(result);
	proc->a.byte = result;
}

/*
	and (zp) handler
*/
void ih_and_zp(R2A03 *proc)
{
	unsigned char zp_addr = fetch_next_byte(proc);
	unsigned char byte = fetch_byte(proc, (unsigned short)zp_addr);
	unsigned char result = proc->a.byte & byte;
	SETZ(result);
	SETN(result);
	proc->a.byte = result;
}

///////////////////////////////////////////////////////////////
//                            eor                            //
///////////////////////////////////////////////////////////////

/*
	eor (imm) heorler
*/
void ih_eor_imm(R2A03 *proc)
{
	unsigned char byte = fetch_next_byte(proc);
	unsigned char result = proc->a.byte ^ byte;
	SETZ(result);
	SETN(result);
	proc->a.byte = result;
}

/*
	eor (izx) handler
*/
void ih_eor_izx(R2A03 *proc)
{
	unsigned char zp_addr = fetch_next_byte(proc) + proc->x.byte;
	unsigned short long_addr = fetch_word(proc, zp_addr);
	unsigned char byte = fetch_byte(proc, long_addr);
	unsigned char result = proc->a.byte ^ byte;
	SETZ(result);
	SETN(result);
	proc->a.byte = result;
}

/*
	eor (izy) handler
*/
void ih_eor_izy(R2A03 *proc)
{
	unsigned char zp_addr = fetch_next_byte(proc);
	unsigned short addr = fetch_word(proc, zp_addr) + proc->y.byte;
	unsigned char byte = fetch_byte(proc, addr);
	unsigned char result = proc->a.byte ^ byte;
	SETZ(result);
	SETN(result);
	proc->a.byte = result;
}

/*
	eor (abs) handler
*/
void ih_eor_abs(R2A03 *proc)
{
	unsigned short addr = fetch_next_word(proc);
	unsigned char byte = fetch_byte(proc, addr);
	unsigned char result = proc->a.byte ^ byte;
	SETZ(result);
	SETN(result);
	proc->a.byte = result;
}

/*
	eor (abx) handler
*/
void ih_eor_abx(R2A03 *proc)
{
	unsigned short addr = fetch_next_word(proc) + proc->x.byte;
	unsigned char byte = fetch_byte(proc, addr);
	unsigned char result = proc->a.byte ^ byte;
	SETZ(result);
	SETN(result);
	proc->a.byte = result;
}

/*
	eor (aby) handler
*/
void ih_eor_aby(R2A03 *proc)
{
	unsigned short addr = fetch_next_word(proc) + proc->y.byte;
	unsigned char byte = fetch_byte(proc, addr);
	unsigned char result = proc->a.byte ^ byte;
	SETZ(result);
	SETN(result);
	proc->a.byte = result;
}

/*
	eor (zpx) handler
*/
void ih_eor_zpx(R2A03 *proc)
{
	unsigned char zp_addr = fetch_next_byte(proc) + proc->x.byte;
	unsigned char byte = fetch_byte(proc, (unsigned short)zp_addr);
	unsigned char result = proc->a.byte ^ byte;
	SETZ(result);
	SETN(result);
	proc->a.byte = result;
}

/*
	eor (zp) handler
*/
void ih_eor_zp(R2A03 *proc)
{
	unsigned char zp_addr = fetch_next_byte(proc);
	unsigned char byte = fetch_byte(proc, (unsigned short)zp_addr);
	unsigned char result = proc->a.byte ^ byte;
	SETZ(result);
	SETN(result);
	proc->a.byte = result;
}