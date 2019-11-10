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
			return 0;
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
void eval(char** argv){
	pid_t pid;
	int command=1;
    for(int i=0; argv[i] != NULL; i++){
		if(!strcmp(argv[i],"|")) command++; 
	}	
	if(command > 1){
		int fd[2];
		char** front = malloc(sizeof(char*)*(100));
		if(pipe(fd) <0){
			perror("pipe error!");
			exit(-1);
		}else {
			
			for(int i=0; argv[i] != NULL; i++){
				front[i] = malloc(sizeof(char*)*50);
			}

		front = divide(argv,"|");
		}
		if ((pid = fork()) == 0) { // front command
				printf("------first start\n");
				for(int i=0; front[i] != NULL; i++){
					printf("%s ",front[i]);
				}
				printf("\n");
				fflush(stdout);
				close(fd[0]);
				dup2(fd[1],1);
				eval(front);

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
			close(fd[1]);
			int temp_fd = dup(STDIN_FILENO);
			dup2(fd[0],0);	
			eval(argv);	
			dup2(temp_fd,STDIN_FILENO);
			}
	} else {
		int check = builtin_command(argv);
  		printf("check:%d,argv[0]:%s\n",check,argv[0]);
		if (!check) { // no check : no builtin_command
		if ((pid = fork()) == 0) {   /* Child runs user job */
		    if (execv(argv[0], argv) < 0) {
				fprintf(stderr,"%s: Command not found.\n", argv[0]);
				exit(0);
	 	   		}
		}

		} else if(check == 1){
		if ((pid = fork()) == 0) {   /* Child runs user job */
			printf("by execvp\n");
		    if (execvp(argv[0], argv) < 0) {
				fprintf(stderr,"%s: Command not found.\n", argv[0]);
				exit(0);
	 	   		}
		}	
		wait(NULL);		
		fflush(stdout);
		}
	}

}
int main(int argc, char** argv){
	pid_t pid;

	eval(argv+1);
}