//-----------------------------------------------------------
//
// SSE2033 : System Software Experiment 2 (Fall 2019)
//
// Skeleton Code for PA#3
//
// Oct 28, 2019.
// CSI, Sungkyunkwan University
//
// Forked by http://csapp.cs.cmu.edu/public/ics2/code/ecf/shellex.c
//-----------------------------------------------------------


/* $begin shellmain */
#define MAXARGS   128
#define MAXLINE	  256
#include "swsh.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/stat.h>



void shellstart(){
	int in=0;
	while (1) {
		char cmdline[MAXLINE]; /* Command line */
		char* ret;
		if(in == 0){
			printf("swsh> "); 
		} else {
			printf("\nswsh> ");
		}
		
		//printf("swsh> ");          
		ret = fgets(cmdline, MAXLINE, stdin); 
		if (feof(stdin) || ret == NULL)
		    exit(0);
		char *argv = malloc(sizeof(char*)*MAXARGS); /* Argument list execve() */
		for(int i=0; i< MAXARGS; i++){
			argv[i] = malloc(sizeof(char)*MAXARGS);
		}
		/* Evaluate */
   		char buf[MAXLINE];   /* Holds modified command line */
   		strcpy(buf, cmdline);
   		int bg = parseline(buf, argv); /* Should the job run in bg or fg? */
		eval(argv,cmdline,bg);
		fflush(stdout);
		in =1;
		free(argv);
	}
}
int main() 
{

	signal(SIGINT,al);
	signal(20,al); // ctrl+z sigstop(20)

	/* Read */
    shellstart();
   return 0;
}
/* $end shellmain */
  