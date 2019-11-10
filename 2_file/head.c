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
int main(int argc, char** argv) {
	int num =10;
	struct stat file_info;
	int file_size;
	int fd;
	if(argv[1] == NULL){
		fprintf(stderr,"no file to read\n");
	}
	if(argv[2] == NULL){
		stat(argv[1],&file_info);
		file_size = file_info.st_size;
		fd = open(argv[1], O_RDONLY);		
	} else{
		if(!strcmp(argv[1],"-n")){
			stat(argv[3],&file_info);
			num = atoi(argv[2]);
			file_size = file_info.st_size;	
			fd = open(argv[3], O_RDONLY);		
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

