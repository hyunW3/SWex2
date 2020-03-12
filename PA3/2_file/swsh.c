//-----------------------------------------------------------
//
// SSE2033 : System Software Experiment 2 (Fall 2019)
//
// Code for PA#3
// 2016310932 Bae HyunWoong
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
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/stat.h>

/* function prototypes */
void eval(char ** argv,char* cmdline,int bg);
int parseline(char *buf, char **argv);
int builtin_command(char **argv); 
<<<<<<< HEAD
int gpid;
char pwd[1024];
void al (int sig) {
	if(gpid != 0){
		kill(-1*gpid ,SIGKILL);
	}
	char cwd[1024];
	char* result = getcwd(cwd,sizeof(cwd));	
	if(result != NULL){
		printf("\n%s$swsh> ",cwd);
	} else {
		printf("\n$swsh> ");
	}
=======

void (*sigstop)(int);
int gpid;
char pwd[1024];
void al (int sig) {
	// reaping child process
	//printf("gpid:%d\n",gpid);
	if(gpid != 0){
		//kill(gpid,SIGKILL);
		kill(-1*gpid,SIGKILL);
	}
	//write(1,"\n",sizeof(char));
>>>>>>> 8902b40f7c58bd14df0bf8c55111e6a472f8af44
	fflush(stdout);
}
<<<<<<< HEAD
=======
void CHLD_al (int sig) {
	// reaping child process
	int status;
	//printf("gpid:%d\n",gpid);
	while(waitpid(-1,&status, WNOHANG | WUNTRACED) != -1){
		if(WIFSTOPPED(status)){
			if(kill(-1*status,SIGKILL) <0){
				kill(status,SIGKILL);
			}
		}
	}
	
	if(gpid != 0){
		//kill(-1*gpid,SIGKILL);
	}
	fflush(stdout);
>>>>>>> 8902b40f7c58bd14df0bf8c55111e6a472f8af44

}
void shellstart(){
	//int in=0;
	while (1) {
		char cmdline[MAXLINE]; /* Command line */
		char* ret;
		
		/*
		if(in == 0){
			printf("swsh> "); 
		} else {
			printf("\nswsh> ");
		}
		*/
		char cwd[1024];
		char* result = getcwd(cwd,sizeof(cwd));	
		if(result != NULL){
			printf("%s$swsh> ",cwd+1);
		} else {
			printf("$swsh> ");
		}
		          
		ret = fgets(cmdline, MAXLINE, stdin); 
		if (feof(stdin) || ret == NULL)
		    exit(0);
		char **argv;
		argv = (char**)malloc(sizeof(char*)*MAXARGS); /* Argument list execve() */
		for(int i=0; i< MAXARGS; i++){
			argv[i] = (char*)malloc(sizeof(char)*MAXARGS);
		}
		/* Evaluate */
		gpid=0;
   		char buf[MAXLINE];   /* Holds modified command line */
   		strcpy(buf, cmdline);
   		int bg = parseline(buf, argv); /* Should the job run in bg or fg? */
		eval(argv,cmdline,bg);
		fflush(stdout);
		gpid=0;
		//in =1;
		free(argv);
	}
}
int main() 
{

	signal(SIGINT,al);
	//sigstop = signal(20,al); // ctrl+z sigstop(20)
	signal(20,al);
	signal(SIGCHLD,CHLD_al);
	getcwd(pwd,sizeof(pwd));
	//write(1,pwd,strlen(pwd));
	//write(1,"\n",sizeof(char));
	/* Read */
    shellstart();
   return 0;
}
/* $end shellmain */

void branch(int check, char** argv,int bg){
	pid_t pid;
		if(check == 1){
<<<<<<< HEAD
			pid=fork();
			int status;
			if(gpid == 0) {
				gpid = pid;
				printf("set gpid:%d\n",gpid);
			}
			setpgid(pid,gpid);			
			if(pid ==0){
				execvp(argv[0],argv);
			}
			waitpid(-1, &status, WNOHANG | WUNTRACED);
			if(WIFSTOPPED(status)){
				kill(-1*gpid ,SIGKILL);
			}
=======
			if(strcmp(argv[0],"man")){
				pid=fork();
				if(gpid == 0) gpid = pid;
				if(pid==0) gpid = getpgid(getpid());
				//setpgid(pid,gpid);
				if(pid ==0){
					execvp(argv[0],argv);
				}
				setpgid(pid,gpid);
				wait(NULL);
			} else {
				if((pid=fork() == 0)){
					execvp(argv[0],argv);
				}
				wait(NULL);
			}
			
			

>>>>>>> 8902b40f7c58bd14df0bf8c55111e6a472f8af44
		}else if(check == 3){
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

/* $end shellmain */
char** divide(char** argv,int location,char* a){
	int i;
	//printf("-------divide func-------\n");
	//printf("location:%d\n",location);
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
		return front;
	}
	
	return NULL;
}
/* $begin eval */
/* eval - Evaluate a command line */
void eval(char ** argv,char* cmdline,int bg){        
    pid_t pid;           /* Process id */
    if (argv[0] == NULL) return;   /* Ignore empty lines */
    int in =0;
	int out =0; 
	int pos_in =-1;
	int pos_out =-1;
	int command=1;
    for(int i=0; argv[i] != NULL; i++){
		if(!strcmp(argv[i],"<")) {
			in=1; pos_in =i;
		}
		if(!strcmp(argv[i],">")) {
			out=1; pos_out = i;
		}
		if(!strcmp(argv[i],">>")) {
			out=2; pos_out =i;
		}
		if(!strcmp(argv[i],"|")) command++; 

	} // whether in,out exist, how many command in argv
	if(command > 1){
		int location=-1;
		for(int i=1; argv[i] != NULL; i++){
			if(!strcmp(argv[i],"|")) {
				location = i; break;
			}
		}
		//printf("location:%d\n",location);
		int fd[2];
		if(pipe(fd) <0){
			perror("pipe error!");
			exit(-1);
		}else {
			char** front = malloc(sizeof(char*)*(100));
			for(int i=0; argv[i] != NULL; i++){
				front[i] = malloc(sizeof(char*)*50);
			}
			front = divide(argv,location,"|");
			pid = fork();

			if(gpid == 0) gpid = pid;
			setpgid(pid,gpid);
   		
   		fflush(stdout);
			if (pid == 0) { // front command
				gpid = getpgid(getpid());
				setpgid(pid,gpid);
				close(fd[0]);
				dup2(fd[1],1);
				eval(front,cmdline,bg);
				free(front);
				exit(0);					
				
			} else {
				setpgid(pid,gpid);
				int status;
				char** back = malloc(sizeof(char*)*(100));
	  			if (waitpid(-1, &status, WUNTRACED) < 0){
					printf("waitfg: waitpid error");
	  			}else{
	  				// reaping child process
	  				//kill(status,SIGKILL);
	  				if(WIFSTOPPED(status)){
	  					kill(-1*gpid,SIGKILL);
	  				}
	  				//printf("------back start\n");
					for(int i=0; argv[i] != NULL; i++){
						back[i] = malloc(sizeof(char*)*50);
					}
					
					int i;
					for(i=location+1; argv[i] != NULL; i++){
						//printf("%s ",argv[i]);
						strcpy(back[i-location-1],argv[i]);
						//printf("%s ",back[i-location-1]);
					}	//			printf("\n");
					//fflush(stdout);
					back[i-location-1] = '\0';
				}
		// back command
				close(fd[1]);

				int temp_fd = dup(STDIN_FILENO);
				dup2(fd[0],0);
				char * ptr = strchr(cmdline,'|')+1;
				eval(back,ptr,bg);
				dup2(temp_fd,STDIN_FILENO); // restore	
				free(back);				
			//}

			
			}

		}

		fflush(stdout);
	} else {
		int temp_fdin = dup(STDIN_FILENO);
		int temp_fdout = dup(STDIN_FILENO);
	if((pos_in+pos_out+1)>=0 ){ // redirection
		int fd;
		if(in > 0){		
			if((fd = open(argv[pos_in+1], O_RDONLY)) == -1){
				perror(argv[pos_in+1]);
			} else {
				dup2(fd,STDIN_FILENO);
				close(fd);
				argv[pos_in] ='\0';
			}
		}
		if(out > 0){
			if(out == 1){
				if ((fd =open(argv[pos_out+1], O_RDWR | O_TRUNC | O_CREAT, 0644)) == -1){
					perror(argv[pos_out+1]);
				} else {
					dup2(fd,STDOUT_FILENO);
					close(fd);
					argv[pos_out] ='\0';					
				}
			} else if(out==2){ // out == 2
				if ((fd =open(argv[pos_out+1], O_RDWR | O_APPEND | O_CREAT, 0644)) == -1){
					perror(argv[pos_out+1]);
				} else {
					dup2(fd,STDOUT_FILENO);
					close(fd);
					argv[pos_out] ='\0';					
				}
			} else { // error
				fprintf(stderr,"wrong out data %d\n",out );
			}
		} 
	}

	int check = builtin_command(argv);
		//printf("execute %s \n",argv[0]);
		//printf("argv[0]:%s\n",argv[0]);
   fflush(stdout);
   if (!check) { // no check : no builtin_command
<<<<<<< HEAD
   		pid = fork();
   		if(gpid == 0) gpid = pid;
		setpgid(pid,gpid);
		if (pid == 0) {   /* Child runs user job */
=======
   			pid = fork();
   		if(gpid == 0) gpid = pid;
   		setpgid(pid,gpid);
 /*  		for(int i=0; argv[i] != NULL; i++){
   			printf("%s ",argv[i]);
   		} printf("\n");
   		fflush(stdout);

 */		
   		fflush(stdout);
		if ( pid== 0 ) {   /* Child runs user job */
   			//printf("here is in child\n");
   		//gpid = getpgid(getpid());
   		setpgid(pid,gpid);
   			fflush(stdout);
>>>>>>> 8902b40f7c58bd14df0bf8c55111e6a472f8af44
   			char * newAr = (char*)malloc(sizeof(char)*(strlen(pwd)+strlen(argv[0])));
   			char *p;
   			if( (p = strstr(argv[0],"./"))) {
   				strcpy(newAr,pwd);
   				strcat(newAr,p+1);
   			} else {
   				strcpy(newAr,pwd);

   				strcat(newAr,"/");
   				strcat(newAr,argv[0]);
   			}
   			argv[0] = newAr;
   			fflush(stdout);
   			
   			if (execv(argv[0], argv) < 0) { // execute the file on folder
				fprintf(stderr,"%s: Command not found.\n", argv[0]);
				exit(0);
	 	   	} 
	 	   	//printf("pid:%d gpid:%d(%d)\n",pid,gpid,getpgid(pid));
	 	   	fflush(stdout);
	 	   free(newAr);
		}
		setpgid(pid,gpid);
		
   		fflush(stdout);
		int status;

		/* Parent waits for foreground job to terminate */
		if (!bg) {
	  	  if (waitpid(pid, &status, WUNTRACED) < 0){
				printf("waitfg: waitpid error");
	  	  }

		}
		else
	 	   printf("%d %s", pid, cmdline);
   	} else { // is builtin command
   		branch(check,argv,bg); 
   	}
   	//printf("end here\n");
	fflush(stdout);
	if((pos_in+pos_out+1) != -1){ // restore redirection
		dup2(temp_fdout,STDIN_FILENO);
		dup2(temp_fdout,STDOUT_FILENO);
	}
	close(temp_fdin);
	close(temp_fdout);
	}
    return;
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
    while ((delim = strchr(buf, ' '))) { // not space enter, then go next

    	if((*buf == '\"')){
    		if((delim =strchr(buf+1,'\"'))){
    			//printf("delim:%c\n",delim[0]);
    			argv[argc++] = buf+1;
    		}
    	} else if(*buf =='\''){
   			if((delim =strchr((buf+1),'\''))){
   				//printf("delim:%s\n",delim);
    			argv[argc++] = buf+1;
    		}
    	} else {
			argv[argc++] = buf;
			
    	}
			*delim = '\0';
			buf = delim + 1;
			while (*buf && (*buf == ' ')) /* Ignore spaces */
	     	  buf++;  

    }
    //char* lim;
    argv[argc] = NULL;
    
    if (argc == 0) return 1; /* Ignore blank line */
    
    /* Should the job run in the background? */
    if ((bg = (*argv[argc-1] == '&')) != 0) argv[--argc] = NULL;
    /*
	for(int i=0; i<argc; i++){
		printf("%s ",argv[i]);
	}
			printf("\n");
			fflush(stdout);
			*/
			
    return bg;
}

  