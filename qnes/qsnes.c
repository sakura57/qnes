#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ricoh.h"
#include "SDL.h"

SDL_Renderer *renderer;
static const char header[] = {'N', 'E', 'S', 0x1A};

void draw_pixel(int x, int y)
{
	SDL_Rect rect;
	rect.x = x*2;
	rect.y = y*2;
	rect.w = rect.h = 2;
	SDL_RenderDrawRect(renderer, &rect);
}

int main(int argc, char **argv)
{
	FILE *romfile;
	unsigned char *filedata;
	long size;
	unsigned char *memory;	//allocate 64 KB to use as system RAM
	unsigned char *ppumem;	//allocate 16 KB for PPU memory
	R2A03 * proc;
	char prgbanks=0,chrbanks=0;

	printf("qnes v0.1a\nJacob Farnsworth\n\n");

	if(argc != 2) {
		printf("Syntax: qnes <romfile>\n\n");
		return 0;
	}

	romfile = fopen(argv[1], "rb");
	fseek(romfile, 0L, SEEK_END);
	size = ftell(romfile);
	rewind(romfile);

	if(size > 500000000L) {
		printf("Error: Input file is too large\n\n");
		return 1;
	}

	printf("Loading \"%s\"...\n", argv[1]);

	filedata = (unsigned char *)malloc(size);
	fread(filedata, 1, size, romfile);
	fclose(romfile);

	if(memcmp(filedata, header, 4)) {
		printf("Error: Invalid iNES header.\n\n");
		return 1;
	}

	printf("Valid iNES header.\n");

	prgbanks = filedata[4];
	chrbanks = filedata[5];

	printf("16KB PRG-ROM banks: %d\n8KB CHR-ROM banks: %d\n", prgbanks, chrbanks);

	if(prgbanks < 1 || prgbanks < 2 || chrbanks != 1) {
		printf("Error: This ROM's PRG-ROM/CHR-ROM arrangement is unsupported.\n");
		printf("The supported arrangements are: \n");
		printf("16KB PRG-ROM & 8KB CHR-ROM\n");
		printf("32KB PRG-ROM & 8KB CHR-ROM\n");
		printf("\n");
		return 1;
	}

	printf("PRG-ROM/CHR-ROM arrangement is supported\n");

	memory = (unsigned char*)malloc(0x10000);
	ppumem = (unsigned char*)malloc(0x4000);
	proc = (R2A03*)malloc(sizeof(R2A03));

	if(!memory || !ppumem || !proc) {
		printf("Error: Memory allocation failed.\nSystem out of memory?\n\n");
		return 1;
	}

	memset(memory, 0, 0x10000);
	memset(ppumem, 0, 0x2000);
	memcpy(&memory[0x8000], &filedata[0x10], 0x4000*prgbanks);
	memcpy(&ppumem[0x2000], &filedata[0x10+0x4000*prgbanks], 0x800*chrbanks);
	initialize(proc, memory);
	reset(proc);

	if(SDL_Init(SDL_INIT_VIDEO) != 0) {
		printf("Error: Call to SDL_Init() failed\n\n");
		return 1;
	}

	printf("Initializing...\n", argv[1]);
	{
		int i=0;
		
		SDL_Window *win;

		if(SDL_CreateWindowAndRenderer(512, 480, SDL_WINDOW_SHOWN, &win, &renderer)) {
			printf("Error: SDL window initialization failed.\n\n");
			return 1;
		}

		while(1) {
			//main execution loop
			SDL_Event sEvent;

			if(fetch_byte(proc, proc->pc.word) == 0) {
				printf("Processor encountered BRK at PC=$%0.4X.\nHalted.\n\n", proc->pc.word);
				break;
			}
			if(proc->critical == 1) {
				printf("PC=$%0.4X\nA=#$%0.2X X=#$%0.2X Y=#$%0.2X\n", proc->pc.word, proc->a.byte, proc->x.byte, proc->y.byte);
				printf("%d%d%d%d%d%d%d%d\nSV?BDIZC\n\n", proc->p.bit.negative, proc->p.bit.overflow, proc->p.bit.unused, proc->p.bit.brkflag, proc->p.bit.decimal, proc->p.bit.irq, proc->p.bit.zero, proc->p.bit.carry);

				printf("Processor encountered invalid/unimplemented instruction. Halted.\n\n");
			}
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderClear(renderer);
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			cycle(proc);
			SDL_RenderPresent(renderer);
			if(SDL_PollEvent(&sEvent) && sEvent.type == SDL_QUIT) {
				break;
			}
		}

		free(proc);
		free(memory);

		SDL_DestroyWindow(win);
	}
	SDL_Quit();

	free(filedata);
	
	return 0;
}