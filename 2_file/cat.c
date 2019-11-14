#define MAXARGS  128
#define MAXLINE	 256
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/stat.h>
//void cat_command(char** argv){
int main(int argc, char** argv){
	int fd;
	int file_size;
	if(argc == 1){
		fd = dup(0);
		file_size = MAXARGS*10;	
	} else {
		struct stat file_info;
		stat(argv[1],&file_info);
		file_size = file_info.st_size;
		fd = open(argv[1], O_RDONLY);	
	}
	char* s1 = (char*)malloc(sizeof(char)*file_size);
	int result = read(fd,s1,sizeof(char)*file_size);
	if(result != EOF){
		//int trash = write(1,s1,sizeof(char)*(file_size-1));
		int trash = write(1,s1,sizeof(char)*(result));
		trash++;
	} else {
		fprintf(stderr,"%d %s: Cannot read file.\n", fd,argv[1]);
	}
	//char* buf = "\n";
	//assert( write(1,buf,sizeof(char)) < 0);
	free(s1);
	//write(1,"\n",sizeof(char));
}

