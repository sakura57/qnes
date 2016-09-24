#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ricoh.h"
#include "SDL.h"

int main(int argc, char **argv)
{
	printf("qnes v0.1a\nJacob Farnsworth\n");

	/*if(argc != 2) {
		printf("Syntax: qnes <romfile>\n");
		return 0;
	}*/

	printf("Loading \"%s\"...\n", argv[1]);

	{
		void * memory = malloc(0x10000);	//allocate 64 KB to use as system RAM
		R2A03 * proc = (R2A03*)malloc(sizeof(R2A03));
		const char code[] = {0xA9, 0x80, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x6A, 0x8D, 0x00, 0x30, 0x00};
		memset(memory, 0, 0x10000);

		initialize(proc, memory);
		proc->pc.word = 0xA000;

		memcpy((void*)((unsigned int)proc->memory+0xA000), code, 0x13);

		while(1) {
			//main execution loop
			printf("PC=$%0.4X\nA=#$%0.2X X=#$%0.2X Y=#$%0.2X\n", proc->pc.word, proc->a.byte, proc->x.byte, proc->y.byte);
			printf("%d%d%d%d%d%d%d%d\nSV?BDIZC\n\n", proc->p.bit.negative, proc->p.bit.overflow, proc->p.bit.unused, proc->p.bit.brkflag, proc->p.bit.decimal, proc->p.bit.irq, proc->p.bit.zero, proc->p.bit.carry);
			if(fetch_byte(proc, proc->pc.word) == 0) {
				printf("Processor encountered BRK at PC=$%0.4X.\nHalting execution.\n\n", proc->pc.word);
				break;
			}
			cycle(proc);
		}

		free(proc);
		free(memory);
	}
	
	return 0;
}