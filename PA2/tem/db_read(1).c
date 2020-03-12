/*-----------------------------------------------------------
 *
 * SSE2033: System Software Experiment 2 (Fall 2019)
 *
 * Skeleton code for PA #2
 * 
 * CSI Lab, Sungkyunkwan University
 *
 * Student Id   : 2016310932	
 * Student Name : Bae HyunWoong
 *
 *-----------------------------------------------------------
 */

#include "db.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h> // arbitory
#include <string.h>
#include <fcntl.h>

#define MAXSIZE 100000 

int Lsize;
int table_num=0; // count for how many element in table
int num=-1; // for making file number
int infile = 0;
void cpy(char *word1, char* word, int length){
	for(int i=0; i<length; i++){
		word1[i] = word[i];
	}
}
unsigned int hash(char *key,int length, int Lsize){
	// hash func in here
	unsigned int sum=0;
	for(int i=0; i<=length; i++){
		sum += key[i];
	}
	return sum%Lsize;
}
db_t *db_open(int size)
{
	mkdir("./db",0777); // mk file directory
	db_t *db = NULL;
	db = (db_t*)malloc(sizeof(db_t)*size);
	Lsize = size;
	for(int i=0; i<size; i++){ // initializer
		db->next = NULL;
	}
	return db;
}

void db_close(db_t *db)
{
	free(db);
}

void db_put(db_t *db, char *key, int key_len,
			char *val, int val_len) // (char*)&cnt == char* val
{

	// search the key
	int entry = hash(key,key_len,Lsize);
	db_t *cur = db+entry;
	// make new db for key value
	db_t *new = (db_t*)malloc(sizeof(db_t));
	new->next = NULL;
	
	new->key = (char*)malloc(sizeof(char)*(key_len+1));
	strcpy(new->key,key);
	new->key_len = key_len+1;
	new->value = *((int*)val);
	
	new->val_len = val_len;

	
	int val_int = *((int*)val);
	if((val_int == 1) | (infile==1)){ // if there is (val == 1) add the tail
	// or val is not 1, but need to add table(since it is found in file)
		while(cur->next != NULL){
			cur = cur->next;
		} 
		cur->next = new;
		table_num++;
	}  else { 	// if not, find the key & insert the val(&cnt)
	// in this case there is two way of find val - in table, file
		
		while(cur->next != NULL){
			cur = cur->next;
			if(strcmp(cur->key,key) == 0) {
				cur->value = val_int;
				break;
			}
		}
		free(new);
	}
	// table is full
	
	if(table_num >= Lsize){
		num++;		
		char * buf1 = malloc(sizeof(char)*MAXSIZE);
		
		sprintf(buf1,"./db/file-%d",num);
		int fd = open(buf1,O_RDWR | O_CREAT | O_TRUNC,0755); 
		for(int i =0; i<Lsize; i++){
			db_t* cu = (db+i); // to search table
			db_t* tmp = cu->next;
			if(tmp == NULL) continue;
			for(cu = tmp; cu != NULL; cu = tmp) { // hashnum/key/value
				
				sprintf(buf1,"%d",i);
				write(fd,buf1,sizeof(int));
				write(fd,"/",sizeof(char));
				write(fd, cu->key,cu->key_len);
				write(fd,"/",sizeof(char));
				sprintf(buf1,"%d\0",cu->value);
				write(fd,buf1, strlen(buf1));
				write(fd, "\n", sizeof(char));
				tmp = cu->next;

				free(cu->key);  
				free(cu);
			}
			(db+i)->next = NULL;
		}

		table_num=0;
		close(fd);
		
	}

}
char* get(int fd, char* s,char* buf){
	
	while(1){ // read 
		read(fd,buf,1);
		//printf("buf : %c\n",buf[0]);
		if(buf[0] == '/') break;
		if(buf[0] == '\n')  break; //lseek(fd,0,SEEK_CUR);
		strcat(s,buf);
	} 
	//printf("s is %s\n",s);
	//strcat(s,'\0');
	return s;
}
/* Returns NULL if not found. A malloc()ed array otherwise.
 * Stores the length of the array in *vallen */
char *db_get(db_t *db, char *key, int key_len,
			int *val_len) // finding if the key in db 
		// val_len == 4 in char size
{
	infile = 0;
	char *value = NULL;
	int entry = hash(key,key_len,Lsize);
	db_t *cur = db+entry;   
	// in db, db[entry] and find it
	if(cur->next == NULL){ // in db there is data
		//printf("finding...\n");
	}else {
		while(cur->next != NULL){
			cur = cur->next;
			if(strcmp(cur->key,key) == 0 ){ // find the value
				value = (char*)malloc(sizeof(char)*(cur->val_len+1));
				*(int*)value = cur->value;
				val_len = &cur->val_len;

			} 
		} 
		return value;
	}
	// find in the file
	
	for(int i=num; i>=0; i--){
		char *buf1 = malloc(sizeof(char)*MAXSIZE);
		sprintf(buf1,"./db/file-%d",i);
		//struct stat file_info;
		//stat(buf1,&file_info);
		//int file_size = file_info.st_size;
		int fd = open(buf1, O_RDONLY);
		//free(buf1);
		
	  for(int j=0; j<Lsize; j++){ // check line (number:Lsize) 
		char buf[1];
		char* s1 = (char*)malloc(sizeof(char)*MAXSIZE);
		char* s2 = (char*)malloc(sizeof(char)*MAXSIZE);
		char* s3 = (char*)malloc(sizeof(char)*MAXSIZE);
		int k = *(int*)get(fd,s1,buf);
		k = k-48; // make decimal

		//printf("hash num is %d\n",k);
		if(k == entry){
			
			get(fd,s2,buf); // get key
			if(strcmp(s2,key) == 0){
				value = (char*)malloc(sizeof(char)*(cur->val_len+1));
				int value_int = *(int*)get(fd,s3,buf);
				*(int*)value = value_int-48;
				free(s1); free(s2); free(s3);
				infile = 1; // flag for db_put
				return value;
			} else get(fd,s3,buf);
		} else get(fd,s2,buf); get(fd,s3,buf);

		
		
		
	  }close(fd);
		
	

	}
	
	return value;
}



