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
#include "command.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/stat.h>

/* function prototypes */
void eval(char *cmdline);
int parseline(char *buf, char **argv);
int builtin_command(char **argv); 
void al (int sig) {
	printf("\nswsh> ");
	fflush(stdout);
}

/*
	char* type1[6] = {"ls","man","grep","sort","awk","bc"}; //6
	char* type2[4] = {"head","tail","cat","cp"};
	char* type3[3] = {"mv","rm","cd"};
	char* type4[2] = {"pwd","exit"};
*/
void branch(int check, char** argv,int bg,char* cmdline){
	pid_t pid; 
	if(check <= 2){
		if ((pid = fork()) == 0) {   /* Child runs user job */
   			// type1 , Ignore singleton &
   			if(check == 1){
   				execvp(argv[0],argv);
   			} else{
   				printf("type2 begin\n");// type2 
   				if(!strcmp(argv[0],"cat")){
   					cat_command(argv); // branch3
   				} else if(!strcmp(argv[0],"cp")){
   					cp_command(argv);
   				} else if(!strcmp(argv[0],"head")){
   					head_command(argv);
   				} else if(!strcmp(argv[0],"tail")){
   					tail_command(argv);
   				}else {
   					printf("not complete\n");
   				}
   				exit(0);
   			}
   		} else { 
   			//wait(NULL); 
			if (!bg) {
	    	int status;
	    	if (waitpid(pid, &status, 0) < 0)
				printf("waitfg: waitpid error");
			}else	{
				printf("%d %s", pid, cmdline);
   			}

   		}
   	}else {
   		if(check == 3){
   			//printf("type3 begin\n",argv);
   			if(!strcmp(argv[0],"cd")){
   				cd_command(argv); // branch3
   			} else if(!strcmp(argv[0],"mv")){
   				//do mv command
   				mv_command(argv);
   			} else if(!strcmp(argv[0],"rm")){
   				//do rm command
   				rm_command(argv);
   			} else {
   				fprintf(stderr,"cannnot find cmd type3\n");
   			}
   			
   		} else { // type4
   			//printf("type4 begin\n");
   			if(!strcmp(argv[0],"pwd")){
   				pwd_command();
   			} else if(!strcmp(argv[0],"exit")){
   				exit_command(argv);
   			}  else {
   				fprintf(stderr,"cannnot find cmd type4\n");
   			}
   		}
   	  }
}




/* $end shellmain */
  
/* $begin eval */
/* eval - Evaluate a command line */
void eval(char *cmdline) 
{
    char *argv[MAXARGS]; /* Argument list execve() */
    char buf[MAXLINE];   /* Holds modified command line */
    int bg;              /* Should the job run in bg or fg? */
    pid_t pid;           /* Process id */
    
    strcpy(buf, cmdline);
    bg = parseline(buf, argv); 
    if (argv[0] == NULL)  
	return;   /* Ignore empty lines */
    int check = builtin_command(argv);
   if (!check) { // no check : no builtin_command
	if ((pid = fork()) == 0) {   /* Child runs user job */
	    if (execv(argv[0], argv) < 0) {
		fprintf(stderr,"%s: Command not found.\n", argv[0]);
		exit(0);
	    }
	}

	/* Parent waits for foreground job to terminate */
	if (!bg) {
	    int status;
	    if (waitpid(pid, &status, 0) < 0)
			printf("waitfg: waitpid error");
	}
	else
	    printf("%d %s", pid, cmdline);
   } else { // is builtin command
   		branch(check,argv,bg,cmdline); 
   }
    return;
}

/* If first arg is a builtin command, run it and return true */
int builtin_command(char **argv) 
{
	char* type1[6] = {"ls","man","grep","sort","awk","bc"}; //6
	char* type2[4] = {"head","tail","cat","cp"};
	char* type3[3] = {"mv","rm","cd"};
	char* type4[2] = {"pwd","exit"};
    if (!strcmp(argv[0], "quit")) exit(0);  /* quit command */
	
    if (!strcmp(argv[0], "&"))  return 1;  /* Ignore singleton & */
	for(int i=0; i<6; i++){ // type1
		if(!strcmp(argv[0],type1[i])) {
			return 1;
		} 
	}
	for(int i=0; i<4; i++){ // type2
		if(!strcmp(argv[0],type2[i])) {
			return 2;
		} 
	}
	for(int i=0; i<3; i++){ // type3
		if(!strcmp(argv[0],type3[i])) {
			return 3;
		} 
	}	
	for(int i=0; i<2; i++){ // type2
		if(!strcmp(argv[0],type4[i])) {
			return 4;
		} 
	}		

    return 0;                     /* Not a builtin command */
}
/* $end eval */

/* $begin parseline */
/* parseline - Parse the command line and build the argv array */
int parseline(char *buf, char **argv) 
{
    char *delim;         /* Points to first space delimiter */
    int argc;            /* Number of args */
    int bg;              /* Background job? */

    buf[strlen(buf)-1] = ' ';  /* Replace trailing '\n' with space */
    while (*buf && (*buf == ' ')) /* Ignore leading spaces */
	buf++;

    /* Build the argv list */
    argc = 0;
    while ((delim = strchr(buf, ' '))) {
	argv[argc++] = buf;
	*delim = '\0';
	buf = delim + 1;
	while (*buf && (*buf == ' ')) /* Ignore spaces */
	       buf++;
    }
    argv[argc] = NULL;
    
    if (argc == 0)  /* Ignore blank line */
	return 1;

    /* Should the job run in the background? */
    if ((bg = (*argv[argc-1] == '&')) != 0)
	argv[--argc] = NULL;

    return bg;
}
/* $end parseline */


