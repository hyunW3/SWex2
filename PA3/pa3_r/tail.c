//2017313264 홍진기

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){
	int line;
	int trash;
	if (strcmp(argv[1], "-n")){
		for (int i = 1; i < argc; i++){
			line = 1;
			if (argc > 2) {
				char tmp[100];
				sprintf(tmp, "==> %s <==\n", argv[i]);
				if (i > 1) trash = write(1, &"\n", 1);
				trash = write(1, tmp, strlen(tmp));
			}
			int fd_front = open(argv[i], O_RDONLY);
			int fd_rear = open(argv[i], O_RDONLY);
			char buf[2];
			while(read(fd_front, buf, 1) > 0){
				if (buf[0] == '\n') {
					line++;
					if (line > 10+1) {
						while(read(fd_rear, buf, 1)){
							if (buf[0] == '\n'){
								break;
							}
						}
					}
				}
			}
			while(read(fd_rear, buf, 1) > 0){
				trash = write(1, buf, 1);
			}
			close(fd_front);
			close(fd_rear);
		}
	}
	else{
		for (int i = 3; i < argc; i++){
			line = 1;
			if (argc > 4) {
				char tmp[100];
				sprintf(tmp, "==> %s <==\n", argv[i]);
				if (i > 3) trash = write(1, &"\n", 1);
				trash = write(1, tmp, strlen(tmp));
			}
			int fd_front = open(argv[i], O_RDONLY);
			int fd_rear = open(argv[i], O_RDONLY);
			char buf[2];
			while(read(fd_front, buf, 1) > 0){
				if (buf[0] == '\n') {
					line++;
					if (line > atoi(argv[2])+1) {
						while(read(fd_rear, buf, 1)){
							if (buf[0] == '\n'){
								break;
							}
						}
					}
				}
			}
			while(read(fd_rear, buf, 1) > 0){
				trash = write(1, buf, 1);
			}
			close(fd_front);
			close(fd_rear);
		}
	}
	trash++;
	return 0;
}