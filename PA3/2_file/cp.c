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
////void cp_command(char** argv){

int main(int argc, char** argv){
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
