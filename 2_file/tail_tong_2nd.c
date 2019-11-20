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
			for(int i =3; argv[i] != NULL; i++){ // to get multiple input
				stat(argv[i],&file_info);
				file_size = file_info.st_size;
				fd = open(argv[i], O_RDONLY);	
				print_file(argc,fd,file_size,num);
			}
		}
	} else { // no option
		if(argv[1] == NULL){
			fd =dup(0);
			file_size = MAXARGS*num;	
			print_file(argc,fd,file_size,num);		
		} else {
			for(int i =1; argv[i] != NULL; i++){ // to get multiple input
				stat(argv[i],&file_info);
				file_size = file_info.st_size;
				fd = open(argv[i], O_RDONLY);	
				print_file(argc,fd,file_size,num);
			}			
		}
	}

}	

void print_file(int argc, int fd, int file_size,int num){
	char* s1 = (char*)malloc(sizeof(char)*file_size);
	int result = read(fd,s1,sizeof(char)*file_size);	//printf("file_size is:%d\n",result);
	//printf("in file,\n%s\n",s1);
	fflush(stdout);
	int times=0;
	int sp = result-1;

	//printf("start\n");
	while(sp>=0){
		
		for(; s1[sp] != '\n'; sp--) {
			if(sp<0) break;
		}
		if(s1[sp] == '\n') {
			//printf("\n");
			times++; 
			if(times >num) break;
			else sp--;
		}
	}
	char * p = s1+sp+1;
	*(s1+result-1) = '\0';
	fflush(stdout);
	if(fd != 0){
		write(1,p,file_size-sp);
	} else {
		write(1,p,file_size-sp-1);
	}
	
	//write(1,s1+sp,end-sp+1);
	free(s1);
	write(1,"\n",sizeof(char));
	//
}