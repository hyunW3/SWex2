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
			int fd = open(argv[i], O_RDONLY);
			if (fd < 0){
				perror("head");
				exit(1);
			}
			if (argc > 2) {
				char tmp[100];
				sprintf(tmp, "==> %s <==\n", argv[i]);\
				if (i > 1) trash = write(1, &"\n", 1);
				trash = write(1, tmp, strlen(tmp));
			}
			char buf[2];
			while(read(fd, buf, 1) > 0 && line <= 10){
				if (buf[0] == '\n') line++;
				trash = write(1, buf, 1);
			}
			close(fd);
		}
	}
	else{
		for (int i = 3; i < argc; i++){
			line = 1;
			int fd = open(argv[i], O_RDONLY);
			if (fd < 0){
				perror("head");
				exit(1);
			}
			if (argc > 4) {
				char tmp[100];
				sprintf(tmp, "==> %s <==\n", argv[i]);
				if (i > 3) trash = write(1, &"\n", 1);
				trash = write(1, tmp, strlen(tmp));
			}
			char buf[2];
			while(read(fd, buf, 1) > 0 && line <= atoi(argv[2])){
				if (buf[0] == '\n') line++;
				trash = write(1, buf, 1);
			}
			close(fd);
		}
	}
	trash++;
	return 0;
}