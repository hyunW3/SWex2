// 2016310932 Bae HyunWoong
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
void print_file(char** argv, int fd, int file_size);
int main(int argc, char** argv){
	int fd;
	int file_size;
	if(argc == 1){
		fd = dup(0);
		file_size = MAXARGS*10;	
		print_file(argv,fd,file_size);
	} else {
		int i=1;
		for(; argv[i] != NULL; i++){
			struct stat file_info;
			stat(argv[i],&file_info);
			file_size = file_info.st_size;
			fd = open(argv[i], O_RDONLY);
			print_file(argv,fd,file_size);
		}
	}

}

void print_file(char** argv, int fd, int file_size){
	char* s1 = (char*)malloc(sizeof(char)*file_size);
	int result = read(fd,s1,sizeof(char)*file_size);
	if(result != EOF){
		int trash = write(1,s1,sizeof(char)*(result));
		trash++;
	} else {
		fprintf(stderr,"%s: Cannot read file.\n",argv[1]);
	}
	free(s1);
}