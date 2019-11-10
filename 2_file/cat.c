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
//void cat_command(char** argv){
int main(int argc, char** argv){
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

