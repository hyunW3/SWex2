/*-----------------------------------------------------------
 *
 * SSE2033: System Software Experiment 2 (Fall 2019)
 *
 * Skeleton code for PA #1
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
#include <stdlib.h> // arbitory
#include <string.h>
int Lsize;
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
	//cpy(new->key,key,key_len);
	strcpy(new->key,key);
	//new->key[key_len] = '\0';
	new->key_len = key_len+1;
	
	//new->value = (char*)malloc(sizeof(char)*val_len+1);
	new->value = *((int*)val);
	//cpy(new->value,val,val_len);
	
	new->val_len = val_len;
	//printf("in new:%s(%d) %d(%d)\n",new->key,new->key_len,new->value,new->val_len);
	
	//cur = cur->next;
	int val_int = *((int*)val);
	//printf("val:%d\n",val_int);
	if(val_int == 1){ // if there is (val == 1) add the tail
		//printf("phase1\n");
		while(cur->next != NULL){
			cur = cur->next;
		} 
		cur->next = new;
	}  else { 	// if not, find the key & insert the val(&cnt)
	
		//printf("phase2\n");
		while(cur->next != NULL){
			cur = cur->next;
			if(strcmp(cur->key,key) == 0) {
				//cpy(cur->value,val,val_len);
				cur->value = val_int;
				//printf("cur_val : %d\n",cur->value);
				break;
			}
		}
		free(new);
	}
}

/* Returns NULL if not found. A malloc()ed array otherwise.
 * Stores the length of the array in *vallen */
char *db_get(db_t *db, char *key, int key_len,
			int *val_len) // finding if the key in db 
		// val_len == 4 in char size
{
	char *value = NULL;
	int entry = hash(key,key_len,Lsize);
	db_t *cur = db+entry;   


	if(cur->next == NULL){ // nothing in the entry
		//printf("nothing..\n");
	} else {
		//printf("finding...\n");
		
		while(cur->next != NULL){
			cur = cur->next;
			if(strcmp(cur->key,key) == 0 ){ // find the value
				
				value = (char*)malloc(sizeof(char)*(cur->val_len+1));
				*(int*)value = cur->value;
				val_len = &cur->val_len;
				//printf("val_len:%d(%d)\n",val_len,*(int*)value);
			} else {
				
			}
		} 
	}
	// in db, db[entry] and find it
	return value;
}



