/*
 ============================================================================
 Name        : forkExample.c
 Author      : Dimitar Gueorguiev
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>

int var_glb; /* a global variable */




int main(void) {
	puts("Fork example"); /* prints Fork example */

	pid_t childPID;

	int var_lcl = 0;

	childPID = fork();

	if (childPID >= 0) /* fork was successful */
	{
		if (childPID == 0) /* child process */
		{
			var_lcl++;
			var_glb++;
			printf("\n Child process :: var_lcl = [%d], var_glb = [%d]",var_lcl,var_glb);

		}
		else /* parent process */
		{
			var_lcl = 10;
			var_glb = 20;
			printf("\n Parent process :: var_lcl = [%d], var_glb = [%d]",var_lcl,var_glb);
		}

	}
	else
	{
		printf("\n Fork failed! quiting...\n");
		return 1;
	}

	return EXIT_SUCCESS;
}
