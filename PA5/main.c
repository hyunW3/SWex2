#define _GNU_SOURCE

#include "db.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#define MAX_KEYLEN 1024

int main(int argc, char *argv[])
{
	db_t *DB;
	char key[MAX_KEYLEN];
	char *val;
	int size;
	int ret, cnt, key_len, val_len;
	int log_fd, dir_fd;

	if (argc != 2) {
		printf("Usage: %s size\n", argv[0]);
		return -1;
	}

	size = atoi(argv[1]);
	DB = db_open(size);
	if (DB == NULL) {
		printf("DB not opened\n");
		return -1;
	}
	printf("DB opened\n");

	if ((log_fd = open("./db_log", O_RDWR|O_CREAT|O_DIRECT, 0755)) < 0) {
		perror("open db_log");
		exit(1);
	}
	posix_fallocate(log_fd, 0, 4096*256*256);  // 256MB
	printf("DB log file opened\n");

	/*

	your code here

	parsing input (ex. GET [key] / PUT [Key] [value])
	interpret command and call functions (ex. db_get, db_put)
	repeat until DB_CLOSE

	*/


	db_close(DB);
	printf("DB closed\n");
	return 0;
}
