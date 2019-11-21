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
#include <sys/mman.h>
// tail
// continous input .
// ex) tail
// a b c -> a,b,c
void print_file(int argc, int fd, int file_size,int num);
int main(int argc,char** argv){
	int num =10;
	struct stat file_info;
	int file_size;
	int fd;
	if((argv[1] != NULL) && (!strcmp(argv[1],"-n"))){ // option
		num = atoi(argv[2]);
		if(argv[3] == NULL){
			fd =dup(0);
			file_size = MAXARGS*num;
			print_file(argc,fd,file_size,num);
		} else {
			int i=3;
			for(; argv[i] != NULL; i++){ // to get multiple input			
				stat(argv[i],&file_info);
				file_size = file_info.st_size;
				fd = open(argv[i], O_RDONLY);
				if(fd<0){
					perror(argv[i]);
					
				}else {
					print_file(argc,fd,file_size,num);
				}	
			}
		}
	} else { // no option
		if(argv[1] == NULL){
			fd =dup(0);
			file_size = MAXARGS*num;	
			print_file(argc,fd,file_size,num);		
		} else {
			int i=1;
			for(; argv[i] != NULL; i++){ // to get multiple input
				fd = open(argv[i], O_RDONLY);
				if(fd <0){
					perror(argv[i]);
				}else {
					stat(argv[i],&file_info);
					file_size = file_info.st_size;
					print_file(argc,fd,file_size,num);
					
				}	
			}			
		}
	}

}	

void print_file(int argc, int fd, int file_size,int num){

	char** s1 = (char**)malloc(sizeof(char*)*num);
	for(int i=0; i<num; i++){
		s1[i] = (char*)malloc(sizeof(char)*1024);
	}
	int line=0;
	int i=0,j=0;
	char buf[2];
	while(read(fd,buf,1) >0 ){
		s1[i][j++] = buf[0];
		if(buf[0] == '\n'){
			s1[i][j] ='\0';
			//printf("%s\n",s1[i]);
			line++;
			i++;
			if(i >=num){
				i -= num;
			}
			j=0;
		}
	}
	j=0;
	while(j<num){
		if(i<0) i = num-1;
		if(i >=num) i -= num;
		if(s1[i] != NULL)	write(1,s1[i],strlen(s1[i]));
		i++; j++;
	}
	fflush(stdout);
}
	