#include <stdio.h>
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

	
	return 0;
}