#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/stat.h>
// tail
int main(int argc,char** argv){
	int num =10;
	struct stat file_info;
	int file_size;
	int fd;
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
	//printf("%d\n",file_size);
	char* s1 = (char*)malloc(sizeof(char)*file_size);
	int result = read(fd,s1,sizeof(char)*file_size);
	int times=0;
	int sp = lseek(fd,-2,SEEK_END);
	//printf("end:%c\n",s1[sp]);
	while(1){
		
		for(; s1[sp] != '\n'; sp--);
		if(s1[sp] == '\n') {
			times++; 
			if(times >= num) break;
			else sp--;
		}
	}
	char * p = s1+sp+1;
	*(s1+file_size-1) = '\0';
	//printf("p is:%s\n",p);
	write(1,p,file_size-sp+1);
	//write(1,s1+sp,end-sp+1);
	free(s1);
}	
