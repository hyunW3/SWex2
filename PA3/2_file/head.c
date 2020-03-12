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
	int num =10;
void print_file(int argc, int fd, int file_size,int num);
int main(int argc, char** argv) {
	
	struct stat file_info;
	int file_size;
	int fd;
	int check = 0;
	int pos =-1;
	if((argv[1] != NULL) && (!strcmp(argv[1],"-n"))){ // option
		num = atoi(argv[2]);
		if(argv[3] == NULL){
			fd =dup(0);
			file_size = MAXARGS*num;
			print_file(argc,fd,file_size,num);
		} else {			
			stat(argv[3],&file_info);
			file_size = file_info.st_size;
			fd = open(argv[3], O_RDONLY);
			if(fd<0){
				perror(argv[3]);
					
			}else {
				print_file(argc,fd,file_size,num);
			}	
		}
	} else { // no option
		if(argv[1] == NULL){
			fd =dup(0);
			file_size = MAXARGS*num;	
			print_file(argc,fd,file_size,num);		
		} else {
			fd = open(argv[1], O_RDONLY);
			if(fd <0){
				perror(argv[1]);
			}else {
				stat(argv[1],&file_info);
				file_size = file_info.st_size;
				print_file(argc,fd,file_size,num);
					
			}				
		}
	}
}

void print_file(int argc, int fd, int file_size,int num){
	char** s1 = (char**)malloc(sizeof(char*)*num);
	for(int i=0; i<num; i++){
		s1[i] = (char*)malloc(sizeof(char)*1024);
	}
	int i=0,j=0;
	char buf[2];
	//printf("fd:%d,num:%d\n",fd,num);
	while(read(fd,buf,1) != EOF ){
		s1[i][j++] = buf[0];
		//printf("%d %d\n",i,j);
		if(buf[0] == '\n'){
			s1[i][j] = '\0';
			j=0;
			write(1,s1[i],strlen(s1[i]));
			if(i>=num) break;
			else {
				 i++;
			}
		}
	}

}