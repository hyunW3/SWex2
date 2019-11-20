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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/stat.h>
#include <errno.h>

void error(char* cmd,int error,char* argv){
	switch(error){
		case 13 :
			fprintf(stderr,"%s: %s: Permisson denied\n",cmd,argv);
			break;
		case 21 :
			fprintf(stderr,"%s: %s: Is a Directory\n",cmd,argv);
			break;
		case 2:
			fprintf(stderr,"%s: %s: No such file or Directory\n",cmd,argv);
			break;
		case 20:
			fprintf(stderr,"%s: %s: Not a Directory\n",cmd,argv);
			break;
		case 1 :
			fprintf(stderr,"%s: %s: Permission denied\n",cmd,argv);
			break;	
		default:
			fprintf(stderr,"%s: %s: Error occurred <%d>\n",cmd,argv,error);					
	}
}
//type3
void cd_command(char** argv){
	int result = chdir(argv[1]);
	if(result == 0){
		// complete
		//printf("move to %s\n",argv[1]);
	} else if(result == -1){
		//failed
		error(argv[0],errno,argv[1]);
	}
}
void rm_command(char** argv){
	//unlink
	int result;
    for (int i = 1; argv[i] != NULL; i++){
    	if ((result = unlink(argv[i])) < 0){
    		error(argv[0],errno,argv[1]);
    	}
    }

}
void mv_command(char** argv){
	//if(argv[2] == NULL || argv[1] == NULL) fprintf(stderr,"there's no newfile name\n");
	if(argv[3] == NULL){
		int result = rename(argv[1],argv[2]);
		if(result == 0){
			// complete
			printf("mv %s to %s\n",argv[1],argv[2]);
		} else if(result == -1){
			//failed
			error(argv[0],errno,argv[1]);
		}			
	} else {
		int last;
		int i=0;
		for(; argv[i]!=NULL; i++);

	}

}
//type4
void pwd_command(){
	char cwd[1024];
	char* result = getcwd(cwd,sizeof(cwd));
	if(result != NULL){
		printf("%s\n",cwd);
	} else {
		fprintf(stderr,"Cannot find pwd.\n");
	}
	
}
void exit_command(char** argv){
	if(argv[1] !=NULL){
		fprintf(stderr,"exit %s\n",argv[1]);
		exit(atoi(argv[1]));
	} else {
		fprintf(stderr,"exit 0\n");
		exit(0);
	}
}
/*
	char* type1[6] = {"ls","man","grep","sort","awk","bc"}; //6
	char* type2[4] = {"head","tail","cat","cp"};
	char* type3[3] = {"mv","rm","cd"};
	char* type4[2] = {"pwd","exit"};
*/

