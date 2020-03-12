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


// type2  = {"head","tail","cat","cp"};
void tail_command(char** argv,int pip){
	int num =10;
	struct stat file_info;
	int file_size;
	int fd;
	/*
						for(int i=0; argv[i] != NULL; i++){
						printf("%d-%s ",i,argv[i]);
					}				printf("\n");
				*/
	printf("pip:%d\n",pip);
	if(argv[2] == NULL){
		printf("tail (pure)\n");
		if(pip != 0){
			fd = pip;
			stat(pip,&file_info);
			file_size = file_info.st_size;
			printf("file_size:%d\n",file_size);
		} else {
			printf("argv[1]\n");
			stat(argv[1],&file_info);
			file_size = file_info.st_size;
			fd = open(argv[1], O_RDONLY);
		}
	} else{
		printf("tail -n num\n");
		if(!strcmp(argv[1],"-n")){
			if(pip !=0){
				fd = pip;
				stat(fd,&file_info);
				file_size = file_info.st_size;
				printf("file_size:%d\n",file_size);
			} else {
				printf("argv[3]\n");
				stat(argv[3],&file_info);
				num = atoi(argv[2]);
				file_size = file_info.st_size;				
				fd = open(argv[3], O_RDONLY);	
			}
				
		} else fprintf(stderr,"%s: wrong option\n", argv[1]);
		
	}

	char* s1 = (char*)malloc(sizeof(char)*file_size);
	int result = read(fd,s1,sizeof(char)*file_size);
	int times=0;
	int sp = lseek(fd,-2,SEEK_END);
	//printf("end:%c\n",s1[sp]);
	if(result != EOF){
	while(1){
		
		for(; s1[sp] != '\n'; sp--);
		if(s1[sp] == '\n') {
			times++; 
			if(times >= num) break;
			else sp--;
		}
	}
	}
	char * p = s1+sp+1;
	*(s1+file_size-1) = '\0';
	//printf("p is:%s\n",p);
	write(1,p,file_size-sp+1);
	//write(1,s1+sp,end-sp+1);
	free(s1);
}
void head_command(char** argv,int pip){
	int num =10;
	struct stat file_info;
	int file_size;
	int fd;
	if(argv[2] == NULL){
		stat(argv[1],&file_info);
		file_size = file_info.st_size;
		if(pip !=0){
			fd = pip;
		} else {
			fd = open(argv[1], O_RDONLY);
		}	
	} else{
		if(!strcmp(argv[1],"-n")){
			stat(argv[3],&file_info);
			num = atoi(argv[2]);
			file_size = file_info.st_size;	
			if(pip !=0){
				fd = pip;
			} else {
				fd = open(argv[3], O_RDONLY);
			}		
		} else fprintf(stderr,"%s: wrong option\n", argv[1]);
		
	}
	

	char* s1 = (char*)malloc(sizeof(char)*file_size);
	int result = read(fd,s1,sizeof(char)*file_size);
	if(result != EOF){
		int sp=0;
		int times=0;
		while((sp<file_size) ){
			char* str = (char*)malloc(sizeof(char)*1024);
			int i;
			for(i=0; (s1[sp] != '\n'); i++,sp++){				
				str[i] = s1[sp];
			} 
			if(times != (num-1)) str[i] = '\n'; // i = str len	
			else { str[i] = '\0'; }
			sp++;
			//assert(write(1,str,sizeof(char)*i) <0);	
			write(1,str,sizeof(char)*(i+1));
			times++;	
			free(str);
			if(times>=num) break;
		}
	} else if(argv[1] == NULL) {
		fprintf(stderr,"%s: Cannot read file.\n", argv[1]);
	}
	//char* buf = "\n";
	//assert( write(1,buf,sizeof(char)) < 0);
	free(s1);	
}
void cat_command(char** argv,int pip){
	struct stat file_info;
	stat(argv[1],&file_info);
	int file_size = file_info.st_size;
	int fd = open(argv[1], O_RDONLY);
	char* s1 = (char*)malloc(sizeof(char)*file_size);
	int result = read(fd,s1,sizeof(char)*file_size);
	if(result != EOF){
		int trash = write(1,s1,sizeof(char)*(file_size-1));
		trash++;
	} else {
		fprintf(stderr,"%s: Cannot read file.\n", argv[1]);
	}
	//char* buf = "\n";
	//assert( write(1,buf,sizeof(char)) < 0);
	free(s1);
}
void cp_command(char** argv){
	int fd1 = open(argv[1], O_RDONLY);
	int fd2 = open(argv[2],O_RDWR | O_CREAT, 0755);

	struct stat file_info;
	stat(argv[1],&file_info);
	int file_size = file_info.st_size;
	char* s1 = (char*)malloc(sizeof(char)*file_size);

	int result = read(fd1,s1,sizeof(char)*file_size);
	if(result != EOF){
		int trash = write(fd2,s1,sizeof(char)*file_size);
		trash++;
	} else {
		fprintf(stderr,"%s: Cannot read file.\n", argv[1]);
	}
	free(s1);
	printf("cp %s to %s\n",argv[1],argv[2]);
}

//type3
void cd_command(char** argv){
	int result = chdir(argv[1]);
	if(result == 0){
		// complete
		printf("move to %s\n",argv[1]);
	} else if(result == -1){
		//failed
		fprintf(stderr,"%s: Directory not found.\n", argv[1]);
	}
}
void rm_command(char** argv){
	//unlink
	int result = unlink(argv[1]);
	if(result == 0){
		// complete
		printf("rm %s\n",argv[1]);
	} else if(result == -1){
		//failed
		fprintf(stderr,"rm %s failed maybe file doesn't exist\n", argv[1]);
	}	
}
void mv_command(char** argv){
	if(argv[2] == NULL) fprintf(stderr,"there's no newfile name\n");
	int result = rename(argv[1],argv[2]);
	if(result == 0){
		// complete
		printf("mv %s to %s\n",argv[1],argv[2]);
	} else if(result == -1){
		//failed
		fprintf(stderr,"mv %s to %s failed\n", argv[1],argv[2]);
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