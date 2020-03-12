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
void eval(char ** argv,char* cmdline,int bg,int pip);
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
void branch(int check, char** argv,int bg,char* cmdline,int pip){
	pid_t pid; 
	if(check <= 2){
		if ((pid = fork()) == 0) {   /* Child runs user job */
   			// type1 , Ignore singleton &
   			if(check == 1){
   				execvp(argv[0],argv);
   			} else{
   				printf("type2 begin\n");// type2 
   				if(!strcmp(argv[0],"cat")){
   					cat_command(argv,pip); // branch3
   				} else if(!strcmp(argv[0],"cp")){
   					cp_command(argv);
   				} else if(!strcmp(argv[0],"head")){
   					head_command(argv,pip);
   				} else if(!strcmp(argv[0],"tail")){
   					tail_command(argv,pip);
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
/*
int redi_in(char** argv){
	int i;
	int fd;
	
}
*/
/* $end shellmain */
char** divide(char** argv,char* a){
	int location=-1;
	int i;
	for(int i=1; argv[i] != NULL; i++){
		if(!strcmp(argv[i],a)) {
			location = i; break;
		}
	}
	printf("-------divide func-------\n");
	printf("location:%d\n",location);
	if(location <0){
		return NULL;
	} else {
		char** front = malloc(sizeof(char*)*(location+1));
	//	printf("front:\n");
		for(i=0; i < location; i++){
			front[i] = malloc(sizeof(char*)*50);
			strcpy(front[i],argv[i]);
	//		printf("%s ",argv[i]);
		}
		front[location] ='\0';
		//argv[location] ='\0';
		printf("back:\n");
		for(i= location+1; argv[i] != NULL; i++){
			strcpy(argv[i-location-1],argv[i]);
			argv[i]='\0';
			printf("%s ",argv[i-location-1]);
		}	printf("\n");
		printf("%d/%s %d/%s\n",location,argv[location],i-location-1,argv[i-location-1]);
		if( !strcmp(argv[location],"|")) {
			argv[location] ='\0';
		}
		if(location >(i-location-1) ) argv[i-location-1] ='\0';
		
		//argv[location] ='\0';
		//argv[i-location-1] = '\0';
		printf("front:");
		for(i=0; front[i] != NULL; i++){
			printf("%s ",front[i]);
		}		
		printf("\nback:");
		for(i=0; argv[i] != NULL; i++){
			printf("%s ",argv[i]);
		}
		printf("\n");
		printf("-------divide func end---\n");
		return front;
	}
	
	return NULL;
}
/* $begin eval */
/* eval - Evaluate a command line */
void eval(char ** argv,char* cmdline,int bg,int pip){        
    pid_t pid;           /* Process id */
    if (argv[0] == NULL) return;   /* Ignore empty lines */
    int in =0;
	int out =0; 
	int command=1;
    for(int i=0; argv[i] != NULL; i++){
		if(!strcmp(argv[i],"<")) in=1;
		if(!strcmp(argv[i],">") || !strcmp(argv[i],">>")) out=1;
		if(!strcmp(argv[i],"|")) command++; 

	} // whether in,out exist, how many command in argv
	if(command > 1){
		int fd[2];
		if(pipe(fd) <0){
			perror("pipe error!");
			exit(-1);
		}else {
			char** front = malloc(sizeof(char*)*(100));
			for(int i=0; argv[i] != NULL; i++){
				front[i] = malloc(sizeof(char*)*50);
			}
			front = divide(argv,"|");
			if ((pid = fork()) == 0) { // front command
				printf("------first start\n");
				for(int i=0; front[i] != NULL; i++){
					printf("%s ",front[i]);
				}
				printf("\n");
				fflush(stdout);
				close(fd[0]);
				//int temp_fd = dup(1);
				dup2(fd[1],1);
				//eval(front,strchr(cmdline,'|')+1,bg);
				eval(front,cmdline,bg,0);
				//printf("front eval end\n");
				//free(front);
				exit(0);					
				
			} else {
				int status;
	  			if (waitpid(pid, &status, 0) < 0){
					printf("waitfg: waitpid error");
	  			}else{
	  				printf("------back start\n");
					printf("back is ");
					for(int i=0; argv[i] != NULL; i++){
						printf("%s ",argv[i]);
					}				printf("\n");
				}
				
		// front command
				close(fd[1]);
				int temp_fd = dup(STDIN_FILENO);
				//dup2(fd[0],0);
				//execvp(argv[0],argv); // success
				char * ptr = strchr(cmdline,'|')+1;
				//printf("ptr:%s\n",ptr);
				eval(argv,ptr,bg,fd[0]);
				dup2(temp_fd,STDIN_FILENO); // restore					
			//}
			
			}

		}

		
	}else {
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
   		branch(check,argv,bg,cmdline,pip); 
   	}
    return;
}
}

/* If first arg is a builtin command, run it and return true */
int builtin_command(char **argv) {
	char* type1[6] = {"ls","man","grep","sort","awk","bc"}; //6
	char* type2[4] = {"head","tail","cat","cp"};
	char* type3[3] = {"mv","rm","cd"};
	char* type4[2] = {"pwd","exit"};
	//printf("argv[0] is %s\n",argv[0]);
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
/*
    int in =0;
    int out=0; 
    int command=1;
    for(int i=0; argv[i] != NULL i++){
		if(!strcmp(argv[i],"<")) in=1; location = i; break;
		if(!strcmp(argv[i],">") || !strcmp(argv[i],">>")) out=1; location = i; break;
		if(!strcmp(argv[i],"|")) command++; 

	}
*/