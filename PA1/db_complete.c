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

void cpy(char *word1, char* word, int length){
	for(int i=0; i<length; i++){
		word1[i] = word[i];
	}
}

int Lsize;
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
	strcpy(new->key,key);
	new->key_len = key_len+1;
	
	new->value = *((int*)val);
	new->val_len = val_len;
	int val_int = *((int*)val);

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
				cur->value = val_int;
				break;
			}
		}
		free(new);
	}
}

/* Returns NULL if not found. A malloc()ed array otherwise.
 * Stores the length of the array in *vallen */
char *db_get(db_t *db, char *key, int key_len,
			int *val_len) // finding if the key in db 	// val_len == 4 in char size
{
	char *value = NULL;
	int entry = hash(key,key_len,Lsize);
	db_t *cur = db+entry;   // in db, db[entry] and find it


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
			} else {
				
			}
		} 
	}
	
	return value;
}



