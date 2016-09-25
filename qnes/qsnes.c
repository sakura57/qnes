#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ricoh.h"
#include "SDL.h"

SDL_Renderer *renderer;

void draw_pixel(int x, int y)
{
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = rect.h = 2;
	SDL_RenderDrawRect(renderer, &rect);
}

int main(int argc, char **argv)
{
	FILE *romfile;
	unsigned char *filedata;
	long size;

	printf("qnes v0.1a\nJacob Farnsworth\n");

	if(argc != 2) {
		printf("Syntax: qnes <romfile>\n");
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

	filedata = (unsigned char *)malloc(size);

	fclose(romfile);

	if(SDL_Init(SDL_INIT_VIDEO) != 0) {
		printf("Error: Call to SDL_Init() failed\n\n");
		return 1;
	}

	printf("Initializing...\n", argv[1]);
	{
		int i=0;
		void * memory = malloc(0x10000);	//allocate 64 KB to use as system RAM
		R2A03 * proc = (R2A03*)malloc(sizeof(R2A03));
		SDL_Window *win;

		if(!memory || !proc) {
			printf("Error: Memory allocation failed.\nSystem out of memory?\n\n");
			return 1;
		}

		memset(memory, 0, 0x10000);
		initialize(proc, memory);
		set_reset_vector(proc, makeword(0x600));
		reset(proc);

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
	
	return 0;
}