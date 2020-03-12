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
	db_t *Endb = db+entry;
	// make new db for key value
	db_t *new = (db_t*)malloc(sizeof(db_t));
	new->next = NULL;
	
	new->key = (char*)malloc(sizeof(char)*(key_len+1));
	cpy(new->key,key,key_len);
	new->key[key_len+1] = '\0';
	new->key_len = key_len+1;
	
	new->value = (char*)malloc(sizeof(char)*val_len);
	cpy(new->value,val,val_len);
	new->val_len = val_len;
	
	Endb = Endb->next;
	// if there is (val == 1) add the tail
	if((*(int*)val) == 1) {
		printf("1\n");
		while(Endb != NULL){
			Endb = Endb->next;
		}
		Endb = new;
	} else { 	// if not, find the key & insert the val(&cnt)
		free(new);
		while(strcmp(Endb->key,key) == 0) {
			*((int*)val) = *((int*)val)+1;
			*((int*)Endb->value) =();
			printf("Endb_val : %s\n",Endb->value);
		}
	}
}

/* Returns NULL if not found. A malloc()ed array otherwise.
 * Stores the length of the array in *vallen */
char *db_get(db_t *db, char *key, int key_len,
			int *val_len) // finding if the key in db
{
	char *value = NULL;
	int entry = hash(key,key_len,Lsize);
	db_t *cur = db+entry;
	if(cur->next == NULL){ // nothing in the entry
		return value;
	} else {
		cur = cur->next;
		while(cur != NULL){
			if(strcmp(cur->key,key) == 0 ){ // find the value
				*val_len = cur->val_len;
				value = (char*)malloc(sizeof(char)*(cur->val_len));
				cpy(value,cur->value,cur->val_len);
			} else {
				cur = cur->next;
			}
		} 
	}
	// in db, db[entry] and find it
	return value;
}



