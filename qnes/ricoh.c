#include "ricoh.h"

void initialize(R2A03 *proc, void *memory)
{
	proc->critical = 0;
	proc->memory = memory;
}

void reset(R2A03 *proc)
{
	proc->a.byte = proc->x.byte = proc->y.byte = proc->p.byte = 0;
	proc->p.bit.unused = 1;
	proc->s.byte = 0xFF;
	proc->pc.word = fetch_word(proc, 0xFFFC);
}

void cycle(R2A03 *proc)
{
	byte_t opcode;

	//fetch the byte at address PC
	opcode.byte = fetch_next_byte(proc);

	//execute the associated instruction handler
	if(ih[opcode.byte] == &ih_brk) {
		proc->critical = 1;
	}
	ih[opcode.byte](proc);
}

void write_byte(R2A03 *proc, unsigned short addr, unsigned char byte)
{
	if(addr < 0x2000) {
		((unsigned char*)proc->memory)[addr&0x7FF] = byte;
		((unsigned char*)proc->memory)[addr&0x7FF+0x800] = byte;
		((unsigned char*)proc->memory)[addr&0x7FF+0x1000] = byte;
		((unsigned char*)proc->memory)[addr&0x7FF+0x1800] = byte;
	}
	((unsigned char*)proc->memory)[addr] = byte;
}

word_t makeword(unsigned short word)
{
	word_t w;
	w.word = word;
	return w;
}

void set_nmi_vector(R2A03 *proc, word_t addr)
{
	((unsigned char*)proc->memory)[0xFFFA] = addr.lo.byte;
	((unsigned char*)proc->memory)[0xFFFB] = addr.hi.byte;
}

void set_irqbrk_vector(R2A03 *proc, word_t addr)
{
	((unsigned char*)proc->memory)[0xFFFE] = addr.lo.byte;
	((unsigned char*)proc->memory)[0xFFFF] = addr.hi.byte;
}

void set_reset_vector(R2A03 *proc, word_t addr)
{
	((unsigned char*)proc->memory)[0xFFFC] = addr.lo.byte;
	((unsigned char*)proc->memory)[0xFFFD] = addr.hi.byte;
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
	word_t w;
	w.lo.byte = ((unsigned char*)proc->memory)[proc->pc.word++];
	w.hi.byte = ((unsigned char*)proc->memory)[proc->pc.word++];
	return w.word;
}

unsigned short fetch_word(R2A03 *proc, unsigned short addr)
{
	word_t w;
	w.lo.byte = ((unsigned char*)proc->memory)[addr++];
	w.hi.byte = ((unsigned char*)proc->memory)[addr++];
	return w.word;
}

void push(R2A03 *proc, unsigned char byte)
{
	((unsigned char*)proc->memory)[proc->s.byte--] = byte;
}

unsigned char pull(R2A03 *proc)
{
	return ((unsigned char*)proc->memory)[++proc->s.byte];
}

///////////////////////////////////////////////////////////////
//                 INSTRUCTION HANDLERS                      //
///////////////////////////////////////////////////////////////

#define SETZ(x) proc->p.bit.zero = ((x) == 0)
#define SETN(x) proc->p.bit.negative = ((x & 0x80) == 0x80)
#define SETC(x) proc->p.bit.carry = ((x) == 1)
#define SETC_A(x) proc->p.bit.carry = (x > 0xFF)
#define SETC_S(x) proc->p.bit.carry = (x < 0x100)
#define SETV_A() proc->p.bit.overflow = (!((proc->a.byte^byte)&0x80) && ((proc->a.byte^result)&0x80))
#define SETV_S() proc->p.bit.overflow = (((proc->a.byte^result)&0x80) && ((proc->a.byte^byte)&0x80))

#define INI_IIZX() unsigned char zp_addr = fetch_next_byte(proc) + proc->x.byte; \
	unsigned short addr = fetch_word(proc, zp_addr)
#define INI_IIZY() unsigned char zp_addr = fetch_next_byte(proc); \
	unsigned short addr = fetch_word(proc, zp_addr) + proc->y.byte
#define INI_IABS() unsigned short addr = fetch_next_word(proc)
#define INI_IABX() unsigned short addr = fetch_next_word(proc) + proc->x.byte
#define INI_IABY() unsigned short addr = fetch_next_word(proc) + proc->y.byte
#define INI_IZPX() unsigned char addr = fetch_next_byte(proc) + proc->x.byte
#define INI_IZPY() unsigned char addr = fetch_next_byte(proc) + proc->y.byte
#define INI_IZP() unsigned char addr = fetch_next_byte(proc)

#define INI_A() unsigned char byte = proc->a.byte
#define INI_IMM() unsigned char byte = fetch_next_byte(proc)
#define INI_IZX() unsigned char zp_addr = fetch_next_byte(proc) + proc->x.byte; \
	unsigned short addr = fetch_word(proc, zp_addr); \
	unsigned char byte = fetch_byte(proc, addr)
#define INI_IZY() unsigned char zp_addr = fetch_next_byte(proc); \
	unsigned short addr = fetch_word(proc, zp_addr) + proc->y.byte; \
	unsigned char byte = fetch_byte(proc, addr)
#define INI_ABS() unsigned short addr = fetch_next_word(proc); \
	unsigned char byte = fetch_byte(proc, addr)
#define INI_ABX() unsigned short addr = fetch_next_word(proc) + proc->x.byte; \
	unsigned char byte = fetch_byte(proc, addr)
#define INI_ABY() unsigned short addr = fetch_next_word(proc) + proc->y.byte; \
	unsigned char byte = fetch_byte(proc, addr)
#define INI_ZPX() unsigned char addr = fetch_next_byte(proc) + proc->x.byte; \
	unsigned char byte = fetch_byte(proc, (unsigned short)addr)
#define INI_ZPY() unsigned char addr = fetch_next_byte(proc) + proc->y.byte; \
	unsigned char byte = fetch_byte(proc, (unsigned short)addr)
#define INI_ZP() unsigned char addr = fetch_next_byte(proc); \
	unsigned char byte = fetch_byte(proc, (unsigned short)addr)

#define OP_ORA() unsigned char result = proc->a.byte | byte
#define OP_AND() unsigned char result = proc->a.byte & byte
#define OP_EOR() unsigned char result = proc->a.byte ^ byte
#define OP_ADC() unsigned short result = proc->a.byte + byte + (proc->p.bit.carry?1:0)
#define OP_SBC() unsigned short result = proc->a.byte - byte - (proc->p.bit.carry?0:1)

#define OP_SBCSA() unsigned short result = proc->a.byte - byte
#define OP_SBCSX() unsigned short result = proc->x.byte - byte
#define OP_SBCSY() unsigned short result = proc->y.byte - byte

#define OP_ROL() unsigned char result = (byte << 1) | (proc->p.bit.carry?1:0);  \
	SETC((byte & 0x80) == 0x80)
#define OP_ROR() unsigned char result = (byte >> 1) | (proc->p.bit.carry?0x80:0);  \
	SETC((byte & 1))
#define OP_ASL() unsigned char result = (byte << 1); SETC((byte & 0x80) == 0x80)
#define OP_LSR() unsigned char result = (byte >> 1); SETC((byte & 1))

#define OP_INC() unsigned char result = ++byte
#define OP_DEC() unsigned char result = --byte

#define FN_LOGIC_OP(n,i,o) void n(R2A03 *proc) { \
	i(); o(); SETZ(result); SETN(result); proc->a.byte = result; }
#define FN_ARITH_OP(n,i,o,v,c) void n(R2A03 *proc) { \
	i(); o(); c(result); SETZ(result & 0xFF); \
	SETN(result); v(); proc->a.byte = (unsigned char)(result&0xFF); }
#define FN_LOAD_OP(n,i,r) void n(R2A03 *proc) { \
	i(); r = byte; SETZ(r); SETN(r); }
#define FN_STORE_OP(n,i,r) void n(R2A03 *proc) { \
	i(); write_byte(proc,addr,r); }
#define FN_SET_OP(n, f) void n(R2A03 *proc) { \
	proc->p.bit.f = 1; }
#define FN_CLEAR_OP(n, f) void n(R2A03 *proc) { \
	proc->p.bit.f = 0; }
#define FN_MEMORY_OP(n,i,o) void n(R2A03 *proc) { \
	i(); o(); SETZ(result); SETN(result); write_byte(proc, addr, result); }
#define FN_INC_REG_OP(n,r) void n(R2A03 *proc) { \
	++r; SETZ(r); SETN(r); }
#define FN_DEC_REG_OP(n,r) void n(R2A03 *proc) { \
	--r; SETZ(r); SETN(r); }
#define FN_XFER_OP(n,r1,r2) void n(R2A03 *proc) { \
	SETZ(r1); SETN(r1); r2 = r1; }
#define FN_BRANCH_OP(n,c) void n(R2A03 *proc) { \
	signed char offs = fetch_next_byte(proc); \
	if(c) { proc->pc.word = (unsigned short)((signed int)proc->pc.word + offs); } }
#define FN_COMPARE_OP(n,i,o) void n(R2A03 *proc) { \
	i(); o(); SETC_S(result); SETZ(result & 0xFF); \
	SETN(result);  }
	

FN_BRANCH_OP(ih_beq, proc->p.bit.zero)
FN_BRANCH_OP(ih_bne, !proc->p.bit.zero)
FN_BRANCH_OP(ih_bvs, proc->p.bit.overflow)
FN_BRANCH_OP(ih_bvc, !proc->p.bit.overflow)
FN_BRANCH_OP(ih_bcs, proc->p.bit.carry)
FN_BRANCH_OP(ih_bcc, !proc->p.bit.carry)
FN_BRANCH_OP(ih_bmi, proc->p.bit.negative)
FN_BRANCH_OP(ih_bpl, !proc->p.bit.negative)

FN_SET_OP(ih_sec, carry)
FN_CLEAR_OP(ih_clc, carry)

//overflow flag has no set instruction
FN_CLEAR_OP(ih_clv, overflow)

FN_SET_OP(ih_sei, irq)
FN_CLEAR_OP(ih_cli, irq)

FN_SET_OP(ih_sed, decimal)
FN_CLEAR_OP(ih_cld, decimal)

FN_COMPARE_OP(ih_cmp_imm, INI_IMM, OP_SBCSA)
FN_COMPARE_OP(ih_cmp_izx, INI_IZX, OP_SBCSA)
FN_COMPARE_OP(ih_cmp_izy, INI_IZY, OP_SBCSA)
FN_COMPARE_OP(ih_cmp_zp, INI_ZP, OP_SBCSA)
FN_COMPARE_OP(ih_cmp_zpx, INI_ZPX, OP_SBCSA)
FN_COMPARE_OP(ih_cmp_abs, INI_ABS, OP_SBCSA)
FN_COMPARE_OP(ih_cmp_abx, INI_ABX, OP_SBCSA)
FN_COMPARE_OP(ih_cmp_aby, INI_ABY, OP_SBCSA)

FN_COMPARE_OP(ih_cpx_imm, INI_IMM, OP_SBCSX)
FN_COMPARE_OP(ih_cpx_zp, INI_ZP, OP_SBCSX)
FN_COMPARE_OP(ih_cpx_abs, INI_ABS, OP_SBCSX)

FN_COMPARE_OP(ih_cpy_imm, INI_IMM, OP_SBCSY)
FN_COMPARE_OP(ih_cpy_zp, INI_ZP, OP_SBCSY)
FN_COMPARE_OP(ih_cpy_abs, INI_ABS, OP_SBCSY)

FN_XFER_OP(ih_tax, proc->a.byte, proc->x.byte)
FN_XFER_OP(ih_txa, proc->x.byte, proc->a.byte)
FN_XFER_OP(ih_tay, proc->a.byte, proc->x.byte)
FN_XFER_OP(ih_tya, proc->x.byte, proc->a.byte)
FN_XFER_OP(ih_tsx, proc->s.byte, proc->x.byte)
FN_XFER_OP(ih_txs, proc->x.byte, proc->s.byte)

FN_INC_REG_OP(ih_inx, proc->x.byte)
FN_DEC_REG_OP(ih_dex, proc->x.byte)
FN_INC_REG_OP(ih_iny, proc->y.byte)
FN_DEC_REG_OP(ih_dey, proc->y.byte)

FN_MEMORY_OP(ih_dec_zp, INI_ZP, OP_DEC)
FN_MEMORY_OP(ih_dec_zpx, INI_ZPX, OP_DEC)
FN_MEMORY_OP(ih_dec_abs, INI_ABS, OP_DEC)
FN_MEMORY_OP(ih_dec_abx, INI_ABX, OP_DEC)

FN_MEMORY_OP(ih_inc_zp, INI_ZP, OP_INC)
FN_MEMORY_OP(ih_inc_zpx, INI_ZPX, OP_INC)
FN_MEMORY_OP(ih_inc_abs, INI_ABS, OP_INC)
FN_MEMORY_OP(ih_inc_abx, INI_ABX, OP_INC)

FN_LOGIC_OP(ih_rol, INI_A, OP_ROL)
FN_MEMORY_OP(ih_rol_zp, INI_ZP, OP_ROL)
FN_MEMORY_OP(ih_rol_zpx, INI_ZPX, OP_ROL)
FN_MEMORY_OP(ih_rol_abs, INI_ABS, OP_ROL)
FN_MEMORY_OP(ih_rol_abx, INI_ABX, OP_ROL)

FN_LOGIC_OP(ih_ror, INI_A, OP_ROR)
FN_MEMORY_OP(ih_ror_zp, INI_ZP, OP_ROR)
FN_MEMORY_OP(ih_ror_zpx, INI_ZPX, OP_ROR)
FN_MEMORY_OP(ih_ror_abs, INI_ABS, OP_ROR)
FN_MEMORY_OP(ih_ror_abx, INI_ABX, OP_ROR)

FN_LOGIC_OP(ih_asl, INI_A, OP_ASL)
FN_MEMORY_OP(ih_asl_zp, INI_ZP, OP_ASL)
FN_MEMORY_OP(ih_asl_zpx, INI_ZPX, OP_ASL)
FN_MEMORY_OP(ih_asl_abs, INI_ABS, OP_ASL)
FN_MEMORY_OP(ih_asl_abx, INI_ABX, OP_ASL)

FN_LOGIC_OP(ih_lsr, INI_A, OP_LSR)
FN_MEMORY_OP(ih_lsr_zp, INI_ZP, OP_LSR)
FN_MEMORY_OP(ih_lsr_zpx, INI_ZPX, OP_LSR)
FN_MEMORY_OP(ih_lsr_abs, INI_ABS, OP_LSR)
FN_MEMORY_OP(ih_lsr_abx, INI_ABX, OP_LSR)

FN_LOGIC_OP(ih_ora_imm, INI_IMM, OP_ORA)
FN_LOGIC_OP(ih_ora_izx, INI_IZX, OP_ORA)
FN_LOGIC_OP(ih_ora_izy, INI_IZY, OP_ORA)
FN_LOGIC_OP(ih_ora_zp, INI_ZP, OP_ORA)
FN_LOGIC_OP(ih_ora_zpx, INI_ZPX, OP_ORA)
FN_LOGIC_OP(ih_ora_abs, INI_ABS, OP_ORA)
FN_LOGIC_OP(ih_ora_abx, INI_ABX, OP_ORA)
FN_LOGIC_OP(ih_ora_aby, INI_ABY, OP_ORA)

FN_LOGIC_OP(ih_and_imm, INI_IMM, OP_AND)
FN_LOGIC_OP(ih_and_izx, INI_IZX, OP_AND)
FN_LOGIC_OP(ih_and_izy, INI_IZY, OP_AND)
FN_LOGIC_OP(ih_and_zp, INI_ZP, OP_AND)
FN_LOGIC_OP(ih_and_zpx, INI_ZPX, OP_AND)
FN_LOGIC_OP(ih_and_abs, INI_ABS, OP_AND)
FN_LOGIC_OP(ih_and_abx, INI_ABX, OP_AND)
FN_LOGIC_OP(ih_and_aby, INI_ABY, OP_AND)

FN_LOGIC_OP(ih_eor_imm, INI_IMM, OP_EOR)
FN_LOGIC_OP(ih_eor_izx, INI_IZX, OP_EOR)
FN_LOGIC_OP(ih_eor_izy, INI_IZY, OP_EOR)
FN_LOGIC_OP(ih_eor_zp, INI_ZP, OP_EOR)
FN_LOGIC_OP(ih_eor_zpx, INI_ZPX, OP_EOR)
FN_LOGIC_OP(ih_eor_abs, INI_ABS, OP_EOR)
FN_LOGIC_OP(ih_eor_abx, INI_ABX, OP_EOR)
FN_LOGIC_OP(ih_eor_aby, INI_ABY, OP_EOR)

FN_ARITH_OP(ih_adc_imm, INI_IMM, OP_ADC, SETV_A, SETC_A)
FN_ARITH_OP(ih_adc_izx, INI_IZX, OP_ADC, SETV_A, SETC_A)
FN_ARITH_OP(ih_adc_izy, INI_IZY, OP_ADC, SETV_A, SETC_A)
FN_ARITH_OP(ih_adc_zp, INI_ZP, OP_ADC, SETV_A, SETC_A)
FN_ARITH_OP(ih_adc_zpx, INI_ZPX, OP_ADC, SETV_A, SETC_A)
FN_ARITH_OP(ih_adc_abs, INI_ABS, OP_ADC, SETV_A, SETC_A)
FN_ARITH_OP(ih_adc_abx, INI_ABX, OP_ADC, SETV_A, SETC_A)
FN_ARITH_OP(ih_adc_aby, INI_ABY, OP_ADC, SETV_A, SETC_A)

FN_ARITH_OP(ih_sbc_imm, INI_IMM, OP_SBC, SETV_S, SETC_S)
FN_ARITH_OP(ih_sbc_izx, INI_IZX, OP_SBC, SETV_S, SETC_S)
FN_ARITH_OP(ih_sbc_izy, INI_IZY, OP_SBC, SETV_S, SETC_S)
FN_ARITH_OP(ih_sbc_zp, INI_ZP, OP_SBC, SETV_S, SETC_S)
FN_ARITH_OP(ih_sbc_zpx, INI_ZPX, OP_SBC, SETV_S, SETC_S)
FN_ARITH_OP(ih_sbc_abs, INI_ABS, OP_SBC, SETV_S, SETC_S)
FN_ARITH_OP(ih_sbc_abx, INI_ABX, OP_SBC, SETV_S, SETC_S)
FN_ARITH_OP(ih_sbc_aby, INI_ABY, OP_SBC, SETV_S, SETC_S)

FN_LOAD_OP(ih_lda_imm, INI_IMM, proc->a.byte)
FN_LOAD_OP(ih_lda_izx, INI_IZX, proc->a.byte)
FN_LOAD_OP(ih_lda_izy, INI_IZY, proc->a.byte)
FN_LOAD_OP(ih_lda_zp, INI_ZP, proc->a.byte)
FN_LOAD_OP(ih_lda_zpx, INI_ZPX, proc->a.byte)
FN_LOAD_OP(ih_lda_abs, INI_ABS, proc->a.byte)
FN_LOAD_OP(ih_lda_abx, INI_ABX, proc->a.byte)
FN_LOAD_OP(ih_lda_aby, INI_ABY, proc->a.byte)

FN_LOAD_OP(ih_ldx_imm, INI_IMM, proc->x.byte)
FN_LOAD_OP(ih_ldx_zp, INI_ZP, proc->x.byte)
FN_LOAD_OP(ih_ldx_zpy, INI_ZPY, proc->x.byte)
FN_LOAD_OP(ih_ldx_abs, INI_ABS, proc->x.byte)
FN_LOAD_OP(ih_ldx_aby, INI_ABY, proc->x.byte)

FN_LOAD_OP(ih_ldy_imm, INI_IMM, proc->x.byte)
FN_LOAD_OP(ih_ldy_zp, INI_ZP, proc->x.byte)
FN_LOAD_OP(ih_ldy_zpx, INI_ZPX, proc->x.byte)
FN_LOAD_OP(ih_ldy_abs, INI_ABS, proc->x.byte)
FN_LOAD_OP(ih_ldy_abx, INI_ABX, proc->x.byte)

FN_STORE_OP(ih_sta_zp, INI_IZP, proc->a.byte)
FN_STORE_OP(ih_sta_zpx, INI_IZPX, proc->a.byte)
FN_STORE_OP(ih_sta_abs, INI_IABS, proc->a.byte)
FN_STORE_OP(ih_sta_abx, INI_IABX, proc->a.byte)
FN_STORE_OP(ih_sta_aby, INI_IABY, proc->a.byte)
FN_STORE_OP(ih_sta_izx, INI_IIZX, proc->a.byte)
FN_STORE_OP(ih_sta_izy, INI_IIZY, proc->a.byte)

FN_STORE_OP(ih_stx_zp, INI_IZP, proc->x.byte)
FN_STORE_OP(ih_stx_zpy, INI_IZPY, proc->x.byte)
FN_STORE_OP(ih_stx_abs, INI_IABS, proc->x.byte)

FN_STORE_OP(ih_sty_zp, INI_IZP, proc->y.byte)
FN_STORE_OP(ih_sty_zpx, INI_IZPX, proc->y.byte)
FN_STORE_OP(ih_sty_abs, INI_IABS, proc->y.byte)

/*
	BRK handler
*/
void ih_brk(R2A03 *proc)
{
	//todo
}

/*
	NOP handler
*/
void ih_nop(R2A03 *proc)
{
	//do nothing
}

/*
	PHP handler
*/
void ih_php(R2A03 *proc)
{
	push(proc, proc->p.byte);
}

/*
	BIT handler
*/
void ih_bit_abs(R2A03 *proc)
{
	unsigned short addr = fetch_next_word(proc);
	unsigned char byte = fetch_byte(proc, addr);
	SETN(byte);
	proc->p.bit.overflow = byte & 0x40;
	proc->p.bit.zero = byte & proc->a.byte;
}

/*
	BIT handler
*/
void ih_bit_zp(R2A03 *proc)
{
	unsigned char addr = fetch_next_byte(proc);
	unsigned char byte = fetch_byte(proc, (unsigned short)addr);
	SETN(byte);
	proc->p.bit.overflow = byte & 0x40;
	proc->p.bit.zero = byte & proc->a.byte;
}

/*
	JMP handler
*/
void ih_jmp_imm(R2A03 *proc)
{
	unsigned short addr = fetch_next_word(proc);
	proc->pc.word = addr;
}

/*
	JMP handler
*/
void ih_jmp_ind(R2A03 *proc)
{
	unsigned short addr = fetch_next_word(proc);
	unsigned short word = fetch_word(proc, addr);
	proc->pc.word = word;
}

/*
	JSR handler
*/
void ih_jsr(R2A03 *proc)
{
	unsigned short addr = fetch_next_word(proc);
	push(proc, ((proc->pc.word--)>>8)&0xFF);
	push(proc, proc->pc.word&0xFF);
	proc->pc.word = addr;
}

/*
	RTS handler
*/
void ih_rts(R2A03 *proc)
{
	unsigned short addr = pull(proc);
	addr += (pull(proc) << 8) + 1;
	proc->pc.word = addr;
}

/*
	RTI handler
*/
void ih_rti(R2A03 *proc)
{
	unsigned short addr;
	proc->p.byte = pull(proc);
	addr = pull(proc);
	addr |= pull(proc) << 8;
	proc->pc.word = addr;
}

/*
	PLP handler
*/
void ih_plp(R2A03 *proc)
{
	proc->p.byte = pull(proc);
}

/*
	PHA handler
*/
void ih_pha(R2A03 *proc)
{
	push(proc, proc->a.byte);
}

/*
	PLA handler
*/
void ih_pla(R2A03 *proc)
{
	unsigned char byte = pull(proc);
	SETN(byte);
	SETZ(byte);
	proc->a.byte = byte;
}

static const instructionHandler ih[256] = {
//	00				01				02				03				04				05				06				07
	&ih_brk,		&ih_ora_izx,	&ih_brk,		&ih_brk,		&ih_brk,		&ih_ora_zp,		&ih_asl_zp,		&ih_brk,
//	08				09				0A				0B				0C				0D				0E				0F
	&ih_php,		&ih_ora_imm,	&ih_asl,		&ih_brk,		&ih_brk,		&ih_ora_abs,	&ih_asl_abs,	&ih_brk,
//	10				11				12				13				14				15				16				17
	&ih_bpl,		&ih_ora_izy,	&ih_brk,		&ih_brk,		&ih_brk,		&ih_ora_zpx,	&ih_asl_zpx,	&ih_brk,
//	18				19				1A				1B				1C				1D				1E				1F
	&ih_clc,		&ih_ora_aby,	&ih_brk,		&ih_brk,		&ih_brk,		&ih_ora_abx,	&ih_asl_abx,	&ih_brk,
//	20				21				22				23				24				25				26				27
	&ih_jsr,		&ih_and_izx,	&ih_brk,		&ih_brk,		&ih_brk,		&ih_and_zp,		&ih_rol_zp,		&ih_brk,
//	28				29				2A				2B				2C				2D				2E				2F
	&ih_plp,		&ih_and_imm,	&ih_rol,		&ih_brk,		&ih_brk,		&ih_and_abs,	&ih_rol_abs,	&ih_brk,
//	30				31				32				33				34				35				36				37
	&ih_bmi,		&ih_and_izy,	&ih_brk,		&ih_brk,		&ih_brk,		&ih_and_zpx,	&ih_rol_zpx,	&ih_brk,
//	38				39				3A				3B				3C				3D				3E				3F
	&ih_sec,		&ih_and_aby,	&ih_brk,		&ih_brk,		&ih_brk,		&ih_and_abx,	&ih_rol_abx,	&ih_brk,
//	40				41				42				43				44				45				46				47
	&ih_rti,		&ih_eor_izx,	&ih_brk,		&ih_brk,		&ih_brk,		&ih_eor_zp,		&ih_lsr_zp,		&ih_brk,
//	48				49				4A				4B				4C				4D				4E				4F
	&ih_pha,		&ih_eor_imm,	&ih_lsr,		&ih_brk,		&ih_jmp_imm,	&ih_eor_abs,	&ih_lsr_abs,	&ih_brk,
//	50				51				52				53				54				55				56				57
	&ih_bvc,		&ih_eor_izy,	&ih_brk,		&ih_brk,		&ih_brk,		&ih_eor_zpx,	&ih_lsr_zpx,	&ih_brk,
//	58				59				5A				5B				5C				5D				5E				5F
	&ih_cli,		&ih_eor_aby,	&ih_brk,		&ih_brk,		&ih_brk,		&ih_eor_abx,	&ih_lsr_abx,	&ih_brk,
//	60				61				62				63				64				65				66				67
	&ih_rts,		&ih_adc_izx,	&ih_brk,		&ih_brk,		&ih_brk,		&ih_adc_zp,		&ih_ror_zp,		&ih_brk,
//	68				69				6A				6B				6C				6D				6E				6F
	&ih_pla,		&ih_adc_imm,	&ih_ror,		&ih_brk,		&ih_jmp_ind,	&ih_adc_abs,	&ih_ror_abs,	&ih_brk,
//	70				71				72				73				74				75				76				77
	&ih_bvs,		&ih_adc_izy,	&ih_brk,		&ih_brk,		&ih_brk,		&ih_adc_zpx,	&ih_ror_zpx,	&ih_brk,
//	78				79				7A				7B				7C				7D				7E				7F
	&ih_sei,		&ih_adc_aby,	&ih_brk,		&ih_brk,		&ih_brk,		&ih_adc_abx,	&ih_ror_abx,	&ih_brk,
//	80				81				82				83				84				85				86				87
	&ih_brk,		&ih_sta_izx,	&ih_brk,		&ih_brk,		&ih_sty_zp,		&ih_sta_zp,		&ih_stx_zp,		&ih_brk,
//	88				89				8A				8B				8C				8D				8E				8F
	&ih_dey,		&ih_brk,		&ih_txa,		&ih_brk,		&ih_sty_abs,	&ih_sta_abs,	&ih_stx_abs,	&ih_brk,
//	90				91				92				93				94				95				96				97
	&ih_bcc,		&ih_sta_izy,	&ih_brk,		&ih_brk,		&ih_sty_zpx,	&ih_sta_zpx,	&ih_stx_zpy,	&ih_brk,
//	98				99				9A				9B				9C				9D				9E				9F
	&ih_tya,		&ih_sta_aby,	&ih_txs,		&ih_brk,		&ih_brk,		&ih_sta_abx,	&ih_brk,		&ih_brk,
//	A0				A1				A2				A3				A4				A5				A6				A7
	&ih_ldy_imm,	&ih_lda_izx,	&ih_ldx_imm,	&ih_brk,		&ih_ldy_zp,		&ih_lda_zp,		&ih_ldx_zp,		&ih_brk,
//	A8				A9				AA				AB				AC				AD				AE				AF
	&ih_tay,		&ih_lda_imm,	&ih_tax,		&ih_brk,		&ih_ldy_abs,	&ih_lda_abs,	&ih_ldx_abs,	&ih_brk,
//	B0				B1				B2				B3				B4				B5				B6				B7
	&ih_bcs,		&ih_lda_izy,	&ih_brk,		&ih_brk,		&ih_ldy_zpx,	&ih_lda_zpx,	&ih_ldx_zpy,	&ih_brk,
//	B8				B9				BA				BB				BC				BD				BE				BF
	&ih_clv,		&ih_lda_aby,	&ih_tsx,		&ih_brk,		&ih_ldy_abx,	&ih_lda_abx,	&ih_ldx_aby,	&ih_brk,
//	C0				C1				C2				C3				C4				C5				C6				C7
	&ih_cpy_imm,	&ih_cmp_izx,	&ih_brk,		&ih_brk,		&ih_cpy_zp,		&ih_cmp_zp,		&ih_dec_zp,		&ih_brk,
//	C8				C9				CA				CB				CC				CD				CE				CF
	&ih_iny,		&ih_cmp_imm,	&ih_dex,		&ih_brk,		&ih_cpy_abs,	&ih_cmp_abs,	&ih_dec_abs,	&ih_brk,
//	D0				D1				D2				D3				D4				D5				D6				D7
	&ih_bne,		&ih_cmp_izy,	&ih_brk,		&ih_brk,		&ih_brk,		&ih_cmp_zpx,	&ih_dec_zpx,	&ih_brk,
//	D8				D9				DA				DB				DC				DD				DE				DF
	&ih_cld,		&ih_cmp_aby,	&ih_brk,		&ih_brk,		&ih_brk,		&ih_cmp_abx,	&ih_dec_abx,	&ih_brk,
//	E0				E1				E2				E3				E4				E5				E6				E7
	&ih_cpx_imm,	&ih_sbc_izx,	&ih_brk,		&ih_brk,		&ih_cpx_zp,		&ih_sbc_zp,		&ih_inc_zp,		&ih_brk,
//	E8				E9				EA				EB				EC				ED				EE				EF
	&ih_inx,		&ih_sbc_imm,	&ih_nop,		&ih_brk,		&ih_cpx_abs,	&ih_sbc_abs,	&ih_inc_abs,	&ih_brk,
//	F0				F1				F2				F3				F4				F5				F6				F7
	&ih_beq,		&ih_sbc_izy,	&ih_brk,		&ih_brk,		&ih_brk,		&ih_sbc_zpx,	&ih_inc_zpx,	&ih_brk,
//	F8				F9				FA				FB				FC				FD				FE				FF
	&ih_sed,		&ih_sbc_aby,	&ih_brk,		&ih_brk,		&ih_brk,		&ih_sbc_abx,	&ih_inc_abx,	&ih_brk
};