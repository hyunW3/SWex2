#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
int main(int argc, char* argv[]) {
        char* op =(char*)malloc(sizeof(char)*3);
	strcpy(op,argv[1]);
	struct stat st;
	int tar_fd = open(argv[2], O_RDWR | O_CREAT , 0755); // | O_TRUNC
        if(argc == 5){ // compress // tar -cf ab.tar a.txt b.txt
		

                if(strcmp(op,"-cf") == 0) {
			//printf("in\n");
                	int fd1 = open(argv[3],O_RDONLY ,0755);
			int fd2 = open(argv[4],O_RDONLY, 0755);
			if(fd1 == -1 || fd2 == -1) {
				perror("cannnot find file");
			}else {
				int fname1_size = strlen(argv[3]);
				int fname2_size = strlen(argv[4]);
				char* buff = (char*)malloc(fname1_size*sizeof(char));
				printf("%s %s %s\n",argv[2],argv[3],argv[4]);
				//sprintf(tar_fd,"%d %s",fname1_size,argv[3]);
			// file 1 write
				// name write
				stat(argv[3], &st);
				int st_size = st.st_size;
				//sprintf(buff,"%d",fname1_size);
				//write(tar_fd,buff,strlen(buff));
				//write(tar_fd,"/",1);
				write(tar_fd,argv[3],fname1_size); // file name
				// file size
				//write(tar_fd,"/",1);
				//sprintf(buff,"%d",st_size); 
				//write(tar_fd,buff,strlen(buff));
				 // file inside
				write(tar_fd,"/",1);
				read(fd1,buff,st.st_size);
				write(tar_fd,buff,st_size);
				write(tar_fd,"/",1);

			// file 2 write
				stat(argv[4], &st);
				st_size = st.st_size;
				//sprintf(buff,"%d",fname2_size);
				//write(tar_fd,buff,strlen(buff));
				//write(tar_fd,"/",1);
				write(tar_fd,argv[4],fname1_size);
				// file size
				//write(tar_fd,"/",1);
				//sprintf(buff,"%d",st_size); 
				//write(tar_fd,buff,strlen(buff));
				// file inside
				write(tar_fd,"/",1);
				read(fd2,buff,st_size); 
				write(tar_fd,buff,st_size);
				write(tar_fd,"/",1);

			}
			close(tar_fd);
			close(fd1);
			close(fd2);
		} else {
			perror("wrong op for compress\n");
		}
        }else if(argc == 3){ // unzip tar -xf ab.tar
                if(strcmp(op,"-xf") == 0) { 
			//printf("in2\n");
			//stat(argv[3], &st);
			//int st_size = st.st_size;
			char buff[1];
			char* s = (char*)malloc(sizeof(char)*1000); 
			//read(tar_fd,buff,1);
			//printf("%s\n",buff);
			while(1){
				read(tar_fd,buff,1);
				if(strcmp(buff,"/") == 0) break;
				//printf("%s\n", buff);
				strcat(s,buff);
			}
			strcat(s,"1");
			printf("%s\n",s);
			
			int fd1 = open(s, O_RDWR | O_CREAT , 0755);
			char* s1 = (char*)malloc(sizeof(char)*1000); 
			while(1){
				read(tar_fd,buff,1);
				if(strcmp(buff,"/") == 0) break;
				strcat(s1,buff);
			}
			write(fd1,s1,strlen(s1));
			// file 2
			char* s2 = (char*)malloc(sizeof(char)*1000); 
			printf("%s\n",buff);
		// name
			while(1){
				read(tar_fd,buff,1);
				if(strcmp(buff,"/") == 0) break;
				strcat(s2,buff);
			}
			strcat(s2,"1");
			printf("%s\n",s2);
			int fd2 = open(s2, O_RDWR | O_CREAT , 0755);
			char* s3 = (char*)malloc(sizeof(char)*1000); 
			while(1){
				read(tar_fd,buff,1);
				if(strcmp(buff,"/") == 0) break;
				strcat(s3,buff);
			}
			write(fd2,s3,strlen(s3));

		
			
			close(tar_fd);
			
		} else {
			perror("wrong op for unzip\n");
		}
        } else {
                perror("wrong parameter!\n");
        }
        
}

