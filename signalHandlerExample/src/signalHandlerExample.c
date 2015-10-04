/*
 ============================================================================
 Name        : signalHandlerExample.c
 Author      : Dimitar Gueorguiev
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>


void sig_handler(int signo)
{
	if (signo == SIGINT)
		printf("received SIGINT\n");
	else if (signo == SIGKILL)
		printf("received SIGKILL\n");
	else if (signo == SIGSTOP)
		printf("receieved SIGSTOP\n");

}

int main(void) {
	puts("Signal Handler Example"); /* prints Signal Handler Example */

    if (signal(SIGINT, sig_handler) == SIG_ERR)
    	printf("\ncan't catch SIGINT\n");

     // a long wait so that we can easily issue a signal to this process
    while(1)
    	sleep(1);

	return EXIT_SUCCESS;
}
