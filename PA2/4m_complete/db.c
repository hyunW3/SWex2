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
//796

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
		(db+i)->next = NULL;
	}
	return db;
}

void db_close(db_t *db)
{
	free(db);
	// insert loading table
		num++;		
		char * buf1 = malloc(sizeof(char)*100);
		
		sprintf(buf1,"./db/file-%d",num);

		int fd = open(buf1,O_RDWR | O_CREAT | O_TRUNC,0755); 
		for(int i =0; i<table_num; i++){
			db_t* cu = (db+i); // to search table
			db_t* tmp = cu->next;
			if(tmp == NULL) continue;
			for(cu = tmp; cu != NULL; cu = tmp) { // key/value
				int trash;
				sprintf(buf1,"%s/%d\n",cu->key,cu->value);
				trash = write(fd,buf1,strlen(buf1));
				trash++;

				tmp = cu->next;
				cu->key=NULL;

				free(cu->key);  
				//free(cu);
			}
			(db+i)->next = NULL;
		}
		free(buf1);
		table_num=0;
		close(fd);
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
		infile =0;
	}  else { 	// if not, find the key & insert the val(&cnt)
	// in this case there is two way of find val - in table, file
		
		while(cur->next != NULL){
			cur = cur->next;
			if(strcmp(cur->key,key) == 0) {
				cur->value = val_int;
				break;
			}
		}
		free(new->key);
		free(new);
	}
	// table is full
	
	if(table_num >= Lsize){
		num++;		
		char * buf1 = malloc(sizeof(char)*100);
		
		sprintf(buf1,"./db/file-%d",num);

		int fd = open(buf1,O_RDWR | O_CREAT | O_TRUNC,0755); 
	
		for(int i =0; i<Lsize; i++){
			db_t* cu = (db+i); // to search table
			db_t* tmp = cu->next;
			if(tmp == NULL) continue;
			for(cu = tmp; cu != NULL; cu = tmp) { // key/value
				int trash;
				sprintf(buf1,"%s/%d\n",cu->key,cu->value);
				trash = write(fd,buf1,strlen(buf1));
				trash++;
				tmp = cu->next;
				cu->key=NULL;

				free(cu->key);  
				//free(cu);
			}
			(db+i)->next = NULL;
		}
		free(buf1);
		table_num=0;
		close(fd);
		
	}

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
	if(cur->next == NULL){ // in db there is no data
		//printf("nothing...\n");
	}else {
		//printf("find in table\n");
		while(cur->next != NULL){
			cur = cur->next;
			if(strcmp(cur->key,key) == 0 ){ // find the value
				value = (char*)malloc(sizeof(char)*(cur->val_len+1));
				*(int*)value = cur->value;
				val_len = &cur->val_len;
				return value;
			} 
		} 
		
	}
	// find in the file
	for(int k=num; k>=0; k--){
		char *buf1 = malloc(sizeof(char)*MAXSIZE);
		sprintf(buf1,"./db/file-%d",k);
		struct stat file_info;
		stat(buf1,&file_info);
		int file_size = file_info.st_size;
		int fd = open(buf1, O_RDONLY);

		char* s1 = (char*)malloc(sizeof(char)*(file_size));
		int trash = read(fd,s1,sizeof(char)*file_size);
		trash++;
		int sp =0;
	  while(sp<file_size) { // check line (number:Lsize) 
		char* num1 = (char*)malloc(sizeof(char)*100);
		char* str = (char*)malloc(sizeof(char)*1024);
		char bar = '/';		
		char enter = '\n';
		int i;

		for(i=0; (s1[sp] != bar); sp++){
				str[i] = s1[sp];
				i++;
		} str[i] ='\0'; 
		sp++;

		for(i=0; s1[sp] != enter; sp++){
			 num1[i] =  s1[sp]; 
			 i++;
		} num1[i] = '\0';
		sp++;
		
		if(strcmp(str,key)==0){
			value = (char *)malloc(sizeof(char)*strlen(str));
			*(int*)value = atoi(num1);
			
			infile =1;
			sp =0; free(num1); free(str); close(fd); free(s1);
			return (value);	
		} 
		
		free(num1);
		free(str);
		
	  }
	close(fd);
	free(s1);
	free(buf1);

	}
	
	return value;
}




