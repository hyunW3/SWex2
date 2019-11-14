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
//void head_command(char** argv)
// just head & read 10 line doesn't success
int main(int argc, char** argv) {
	int num =10;
	struct stat file_info;
	int file_size;
	int fd;
	int check = 0;
	int pos =-1;
	// i think this should be in shell code
	/*
	for(int i=0; i< argc; i++){
		if(!strcmp(argv[i],"<")){
			check =1; pos =i; break;
		} 
	}
	if(check){ // find redirection
		if(!strcmp(argv[1],"-n")){
			num = atoi(argv[2]);
		} 
		stat(argv[pos+1],&file_info);
		file_size = file_info.st_size;
		fd = open(argv[pos+1], O_RDONLY);	
			
	}else */
	if(argc <= 2){ 
		if(argc == 1){ // ls | head 
			fd = dup(0);
			file_size = MAXARGS;
			//printf("stdin\n");
			fflush(stdout);
		} else {
			stat(argv[1],&file_info);
			file_size = file_info.st_size;
			fd = open(argv[1], O_RDONLY);			
		}
	
	} else{
		if(!strcmp(argv[1],"-n")){
			num = atoi(argv[2]);
			if(argc == 3){
				fd = dup(0);
			//	printf("2stdin\n");
				file_size = MAXARGS;
			} else if(argc == 4){
				stat(argv[3],&file_info);
				file_size = file_info.st_size;	
				fd = open(argv[3], O_RDONLY);				
			} else {
				file_size = MAXLINE;
			}
		
		} else fprintf(stderr,"%s: wrong option\n", argv[1]);
		
	}
	

	char* s1 = (char*)malloc(sizeof(char)*file_size);
	int result = read(fd,s1,sizeof(char)*file_size);
	if(result != EOF){
		int sp=0;
		int times=0;
		while((sp<result) ){
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
	} else  {
		if(argc <=2){
			fprintf(stderr,"%s: Cannot read file.", argv[1]);
		} else { // head- n 3 wrongfile
			fprintf(stderr,"%s: Cannot read file.", argv[3]);
		}
		
	}/*
	else if(argc == 1) {
		fprintf(stderr,"%s: Cannot read file.\n", argv[1]);
	}
	*/
	//char* buf = "\n";
	//assert( write(1,buf,sizeof(char)) < 0);
	free(s1);	
	if(argc != 1 || argc != 3) write(1,"\n",sizeof(char));
	//write(1,"\n",sizeof(char));
}

