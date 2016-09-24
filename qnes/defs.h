#ifndef _DEFS_H
#define _DEFS_H

typedef struct {
	unsigned char bit0 : 1;
	unsigned char bit1 : 1;
	unsigned char bit2 : 1;
	unsigned char bit3 : 1;
	unsigned char bit4 : 1;
	unsigned char bit5 : 1;
	unsigned char bit6 : 1;
	unsigned char bit7 : 1;
} byte_t_bits;

typedef union {
	byte_t_bits bit;
	unsigned char byte;
} byte_t;

typedef struct {
	unsigned char bit0 : 1;
	unsigned char bit1 : 1;
	unsigned char bit2 : 1;
	unsigned char bit3 : 1;
	unsigned char bit4 : 1;
	unsigned char bit5 : 1;
	unsigned char bit6 : 1;
	unsigned char bit7 : 1;
	unsigned char bit8 : 1;
	unsigned char bit9 : 1;
	unsigned char bit10 : 1;
	unsigned char bit11 : 1;
	unsigned char bit12 : 1;
	unsigned char bit13 : 1;
	unsigned char bit14 : 1;
	unsigned char bit15 : 1;
} word_t_bits;

typedef union {
	word_t_bits bit;
	unsigned short word;
	struct {
		byte_t lo;
		byte_t hi;
	};
} word_t;

typedef struct {
	unsigned char nametable_select0 : 1;
	unsigned char nametable_select1 : 1;
	unsigned char increment_mode : 1;
	unsigned char sprite_tile_select : 1;
	unsigned char bg_tile_select : 1;
	unsigned char sprite_height : 1;
	unsigned char ppu_master_slave : 1;
	unsigned char nmi_enabled : 1;
} ppuctrl_t_bits;

typedef struct {
	unsigned char greyscale : 1;
	unsigned char bg_left_column_enable : 1;
	unsigned char sprite_left_column_enable : 1;
	unsigned char bg_enable : 1;
	unsigned char sprite_enable : 1;
	unsigned char emphasis_red : 1;
	unsigned char emphasis_green : 1;
	unsigned char emphasis_blue : 1;
} ppumask_t_bits;

typedef struct {
	unsigned char unused0 : 1;
	unsigned char unused1 : 1;
	unsigned char unused2 : 1;
	unsigned char unused3 : 1;
	unsigned char unused4 : 1;
	unsigned char sprite_overflow : 1;
	unsigned char sprite0_hit : 1;
	unsigned char vblank : 1;
} ppustatus_t_bits;

typedef union {
	ppuctrl_t_bits bit;
	unsigned char byte;
} ppuctrl_t;

typedef union {
	ppumask_t_bits bit;
	unsigned char byte;
} ppumask_t;

typedef union {
	ppustatus_t_bits bit;
	unsigned char byte;
} ppustatus_t;

typedef unsigned char oamaddr_t;
typedef unsigned char oamdata_t;

#endif