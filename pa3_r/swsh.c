//-----------------------------------------------------------
//
// SSE2033 : System Software Experiment 2 (Fall 2018)
//
// Skeleton Code for PA#3
//
// Nov 1, 2018.
// CSLab, Sungkyunkwan University
//
// Forked by http://csapp.cs.cmu.edu/public/ics2/code/ecf/shellex.c
//
// ID / name : 2017313264 / 홍진기
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

/* function prototypes */
void eval(char *cmdline);
void parseline(char *buf, char **argv);
int builtin_command(char **argv); 
/* my function prototypes */
int redirect_in(char ** myArgv);	/* myArgv should be ended by NULL */
int redirect_out(char ** myArgv);
void loop_pipe(char ** cmd[], int command);
void handler();

int pgid;

int main() 
{
    char cmdline[MAXLINE]; /* Command line */
	char* ret; 

    signal(SIGINT, handler);
    signal(SIGTSTP, handler);  

    while (1) {
	/* Read */  
	printf("swsh> ");     

	ret = fgets(cmdline, MAXLINE, stdin); 
	if (feof(stdin) || ret == NULL)
	    exit(0);   

	/* Evaluate */
	eval(cmdline);
    } 
}
/* $end shellmain */
  
/* $begin eval */
/* eval - Evaluate a command line */
void eval(char *cmdline) 
{
    char *argv[MAXARGS]; /* Argument list execve() */
    char buf[MAXLINE];   /* Holds modified command line */
    int inredir, outredir, command;	/* [inredir : 0-no 1-exist], [outredir : 0-no 1-exist(> or >>)], [command : number of commands] */

    strcpy(buf, cmdline);
    parseline(buf, argv);
    if (argv[0] == NULL)  
	return;   /* Ignore empty lines */	

	/* Check redirection and pipeline */
    inredir = outredir = 0; command = 1;
    for (int i = 0; argv[i] != NULL; i++){	
    	if (!strcmp(argv[i], "<")) inredir = 1;
    	if (!strcmp(argv[i], ">") || !strcmp(argv[i], ">>")) outredir = 1;
    	if (!strcmp(argv[i], "|")) command++; 
    }
    int tempfd_in, tempfd_out;
    if (inredir) tempfd_in = redirect_in(argv);	/* Redirection */
    if (outredir) tempfd_out = redirect_out(argv);

    /* Parse commands for pipeline */
    char ***cmd = malloc(sizeof(char**)*(command+1));
    int ptr, pos = 0;
    cmd[command] = NULL;
    for (int i = 0; i < command; i++){
        ptr = 0;
    	cmd[i] = malloc(sizeof(char*)*50);
    	while(argv[pos] != NULL && strcmp(argv[pos], "|") != 0){
    		cmd[i][ptr] = malloc(sizeof(char)*strlen(argv[pos]+3));
    		strcpy(cmd[i][ptr++], argv[pos++]);	
    	}
    	cmd[i][ptr] = NULL;
    	pos++;
    }

    /* Execute Command! */
    if (!builtin_command(argv)) { 
		loop_pipe(cmd, command);
    }

    /* Backup Redirection */
    if (inredir){	
    	dup2(tempfd_in, STDIN_FILENO);	
    	close(tempfd_in);
    }
    if (outredir){
		dup2(tempfd_out, STDOUT_FILENO);
		close(tempfd_out);   	
    } 

    /* free (char ***)cmd mallocs */
    for (int i = 0; i < command; i++){
        for (int j = 0; cmd[i][j] != NULL; j++){
            free(cmd[i][j]);
        }
        free(cmd[i]);
    }
    free(cmd);

    return;
}

/* If first arg is a builtin command, run it and return true */
int builtin_command(char **argv) 
{
    if (!strcmp(argv[0], "quit")) /* quit command */
		exit(0);  
    if (!strcmp(argv[0], "&"))    /* Ignore singleton & */
		return 1;

	if (!strcmp(argv[0], "pwd")){	
		char buff[1024];
		char *trash = getcwd(buff, 1024);
        int trash2 = write(1, buff, strlen(buff));
        trash2 = write(1, &"\n", 1);
		trash++; trash2++;
   		return 1;
	}
	if (!strcmp(argv[0], "exit")){
		fprintf(stderr, "exit\n");
		if (argv[1] != NULL)
			exit(atoi(argv[1]));
		exit(0);
	}
	if (!strcmp(argv[0], "rm")){
		int ret;
        for (int i = 1; argv[i] != NULL; i++){
    		if ((ret = unlink(argv[i])) < 0){
    			perror("rm");
    		}
        }
		return 1;
	}
	if (!strcmp(argv[0], "cd")){
        if (argv[2] != NULL){
            int trash = write(2, &"cd: too many arguments\n", 23);
            trash++;
            return 1;
        }
		int ret = chdir(argv[1]);
		if (ret < 0){
			perror("cd");
		}
		return 1;
	}
	if (!strcmp(argv[0], "mv")){
        if (argv[3] != NULL){
            int trash = write(2, &"cd: too many arguments\n", 23);
            trash++;
            return 1;
        }
		int ret = rename(argv[1], argv[2]);
		if (ret < 0){
			perror("mv");
		}
		return 1;
	}
    return 0;                     /* Not a builtin command */
}
/* $end eval */

/* $begin my functions */

int redirect_in(char ** myArgv) {
	int i;
	int fd;
	int tempfd = dup(STDIN_FILENO);
  
	/* search forward for < */
	for (i = 0; myArgv[i] != NULL; i++) {
		if (!strcmp(myArgv[i], "<")) {
    		break;
   		}
	}

	if (myArgv[i]) {      /* found "<" in vector. */
 
    if (!myArgv[i+1]) { /* File for redirection has not been provided*/
    	return -1;
    } 
    else {            /* Open file. */
    	if ((fd =open(myArgv[i+1], O_RDONLY)) == -1) {
        	perror(myArgv[i+1]);
        	return -1;
    	}
    }

    dup2(fd,STDIN_FILENO);      /* Redirect to use it for input. */
    close(fd);

    for (i = i + 2; myArgv[i] != NULL; i++) { /* close up the vector */
    	myArgv[i-2] = myArgv[i];
    }
    myArgv[i-2] = NULL;      /* NULL pointer at end */
  }
  return tempfd;
}

int redirect_out(char ** myArgv) {
	int i, fd;
	int flag; /* distinguish between '>' and '>>' */ 
	int tempfd = dup(STDOUT_FILENO);  

  /* search forward for > or >> */
	for (i = 0; myArgv[i] != NULL; i++) {
    	if (!strcmp(myArgv[i], ">")) {
    		flag = 0;	
    		break;
    	}
    	else if (!strcmp(myArgv[i], ">>")) {
    		flag = 1;	
    		break;
    	}    
	}

	if (myArgv[i]) {      /* found ">" or ">>" in vector. */
	 
	    if (!myArgv[i+1]) { /* File for redirection has not been provided*/
	    	return -1;
	    } 
	    else {            /* Open file. */
	    	if (flag == 0){
		    	if ((fd =open(myArgv[i+1], O_RDWR | O_TRUNC | O_CREAT, 0644)) == -1) {
		    		perror(myArgv[i+1]);
		        	return -1;
		      	}
	 		}
	 		else if (flag == 1){
	 	    	if ((fd =open(myArgv[i+1], O_RDWR | O_APPEND | O_CREAT, 0644)) == -1) {
		    		perror(myArgv[i+1]);
		        	return -1;
		      	}			
	 		}
	    }

	    dup2(fd,STDOUT_FILENO);      /* Redirect to use it for input. */
	    close(fd);
	 
	    for (i = i + 2; myArgv[i] != NULL; i++) { /* close up the vector */
	    	myArgv[i-2] = myArgv[i];
	    }
	    myArgv[i-2] = NULL;      /* NULL pointer at end */
	}
	return tempfd;
}

void loop_pipe(char *** cmd, int command) 
{
    if ((strcmp((*cmd)[0], "man") || command != 1) && (strcmp((*cmd)[0], "sort") || command != 1)){  //Case of not one man command
        int status;
        pid_t pid;
        int *fds = malloc(sizeof(int)*2*(command-1));
        char *cmd_type1[6] = {"ls", "man", "grep", "sort", "awk", "bc"};
        char *cmd_type2[4] = {"head", "tail", "cat", "cp"};
        char *cmd_type2_2[4] = {"./head", "./tail", "./cat", "./cp"};

        char tmp[200];
        for (int j = 0; j < 4; j++){
            if (!strcmp((*cmd)[0], cmd_type2[j])) {
                    sprintf(tmp, "./%s", (*cmd)[0]);
                    strcpy((*cmd)[0], tmp);
                    break;
                    } 
                }

        for (int i = 0; i < command-1; i++){
            if (pipe(fds + i*2) < 0) {
                perror("pipe");
                exit(1);
            }
        }
        int j = 0;
        pgid = 0; // Global Variable
    	while (*cmd != NULL) {

            if (*(cmd + 1) == NULL && !strcmp((*cmd)[0], "man"))
                goto man;

            pid = fork();
            if (pgid == 0) pgid = pid;
            setpgid(pid, pgid);

            if (pid == 0) {
                if (*(cmd+1) != NULL){  /* if not last command */
                    if (dup2(fds[j+1], 1) < 0){
                        perror("dup2");
                        exit(1);
                    }
                }
                if (j != 0){    /*if not first command && j!=2*command */ 
                    if (dup2(fds[j-2], 0) < 0){
                        perror("dup2");
                        exit(1);
                    }
                }

                for (int i = 0; i < 2*(command-1); i++)
                    close(fds[i]);

                /* Check if input command is in cmd_types list */
                int is_in_type = 0;
                for (int i = 0; i < 6; i++)
                    if (!strcmp(cmd_type1[i], (*cmd)[0]))
                        is_in_type = 1;
                for (int i = 0; i < 4; i++)
                    if (!strcmp(cmd_type2_2[i], (*cmd)[0]))
                        is_in_type = 1;
                if ((*cmd)[0][0] == '.' && (*cmd)[0][1] =='/')
                    is_in_type = 1;

                if (is_in_type == 0 || execvp((*cmd)[0], *cmd) < 0){
                    char tmp[200];
                    sprintf(tmp, "%s: Command not found.\n", (*cmd)[0]);
                    int trash = write(2, tmp, strlen(tmp));
                    trash++;
                    exit(1);
                }
            }
            cmd++;
            j += 2;
        }

        /*Parent closes the pipes and wait for children*/
        for(int i = 0; i < 2 *(command-1); i++){
            close(fds[i]);
        }

        for(int i = 0; i < command; i++)
            waitpid(-1, &status, WUNTRACED);        
        
        pgid = 0;
        free(fds);
    }
    else {  //Case of one man command
        man:;
        int   p[2];
        pid_t pid;
        if (pipe(p) < 0){
            perror("pipe");
            exit(1);
        }   
        if ((pid = fork()) == -1) {
            exit(EXIT_FAILURE);
        }        
        else if (pid == 0) {
            if (*(cmd + 1) != NULL) dup2(p[1], 1);
             close(p[0]);
             if (execvp((*cmd)[0], *cmd) < 0) {
                 fprintf(stderr, "%s: Command not found.\n", (*cmd)[0]);
                 exit(0);
             }
         }
        /* Parent waits for foreground job to terminate */
        else{
             int status;
             waitpid(pid, &status, 0);
             close(p[1]);
        }
        pgid = 0;
    }
}

void handler()
{
    int trash;
    if (pgid != 0) {
        kill(-1*pgid, SIGKILL);
        trash = write(1, &"\n", 1);    
    }
    else trash = write(1, &"\nswsh> ", 7);
    trash++;
}
/* $End my functions */


/* $begin parseline */
/* parseline - Parse the command line and build the argv array */
void parseline(char *buf, char **argv) 
{
    int argc;            /* Number of args */

    buf[strlen(buf)-1] = ' ';  /* Replace trailing '\n' with space */
    while (*buf && (*buf == ' ')) /* Ignore leading spaces */
	buf++;

    /* Build the argv list */
    argc = 0;
    while(1){
        if (*buf == '\'' || *buf == '\"'){
            argv[argc++] = ++buf;
            while (*buf && *buf != '\'' && *buf != '\"')
                buf++;
            if (!*buf) break;
            *(buf++) = '\0';
        }
        else if (*buf == ' '){
            buf++;
        }
        else if (*buf == '\0'){
            break;
        }
        else {
            argv[argc++] = buf;
            while(*buf && *buf != ' ')
                buf++;
            *(buf++) = '\0';
        }
    }
    argv[argc] = NULL;
    
    if (argc == 0)  /* Ignore blank line */
	return;
}
/* $end parseline */


