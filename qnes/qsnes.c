#include <stdio.h>
#include <stdlib.h>
#include "ricoh.h"
#include "SDL.h"

int main(int argc, char **argv)
{
	printf("qnes v0.1a\nJacob Farnsworth\n");

	if(argc != 2) {
		printf("Syntax: qnes <romfile>\n");
		return 0;
	}

	printf("Loading \"%s\"...\n", argv[1]);

	{
		void * memory = malloc(0x10000);	//allocate 64 KB to use as system RAM
		R2A03 * proc = (R2A03*)malloc(sizeof(R2A03));

		initialize(proc, memory);

		while(1) {
			//main execution loop
			cycle(proc);
		}

		free(proc);
		free(memory);
	}
	
	return 0;
}