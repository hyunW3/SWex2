/*-----------------------------------------------------------
 *
 * SSE2033: System Software Experiment 2 (Fall 2018)
 *
 * Skeleton code for PA#2
 * 
 * CSLab, Sungkyunkwan University
 *
 * Student Id   : 2017313264
 * Student Name : Hong Jingi
 *
 *-----------------------------------------------------------
 */

#include "db.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

typedef struct Node_{
	struct Node_ *child;
	struct Node_ *neighbor;
	int latest;
	int offset;
	char data;
}Node;

int Gsize;
int element_num;
int file_num;
Node *root = NULL;	//Root(first node) for trie tree

//if there is no word in trie tree, return 0
//else return (Node *)information of the word
Node* find(char *word) {
	int depth = 0;
	Node *now = root;
	while (1) {
		if (now == NULL) return 0;
		if (depth >= (int)strlen(word) - 1 && word[depth] == now->data) return now;
		//for strlen == 1 word, I add condition 'word[depth] == now->data'
		//if strlen == 1, first condition always becomes true
		
		if (now->data != word[depth]) now = now->neighbor;
		else {
			now = now->child;
			depth++;
		}
	}
}

void insert(char *word, int latest, int offset) {
	if (root == NULL) {
		root = (Node *)malloc(sizeof(Node));
		*root = (Node) { NULL, NULL, -1, -1, word[0] };
	} //If there is no data, initialize root

	int depth = 0;
	Node *now = root;
	while (1) {
		if (now->data != word[depth]) {
			if (now->neighbor != NULL) now = now->neighbor;
			else {
				Node *new = (Node *)malloc(sizeof(Node));
				*new = (Node) { NULL, NULL, -1, -1, word[depth] };
				
				now->neighbor = new;
				now = now->neighbor;
			}
		}
		else {
			if (depth >= (int)strlen(word) - 1) {
				now->latest = latest;
				now->offset = offset;
				return;
			}//Termination condition
			
			if (now->child != NULL) {
				now = now->child;
				depth++;
			}
			else {
				Node *new = (Node *)malloc(sizeof(Node));
				*new = (Node) { NULL, NULL, -1, -1, word[++depth] };				
				
				now->child = new;
				now = now->child;
			}
		}
	}
}

void Triefree(Node *now) {
	if (now == NULL) return;
	if (now->child == NULL && now->neighbor == NULL) {
		free(now);
		return;
	}
	Triefree(now->neighbor);
	Triefree(now->child);
	free(now);
}

unsigned int hash_func(char *key){
	unsigned int res = 0;
	for (int i = 0; key[i] != '\0'; i++)
		res += key[i];
	return res%Gsize;
}

db_t* db_open(int size) {
	mkdir("db", 0777);
	Gsize = size;
	db_t *Entry = (db_t*)malloc(sizeof(db_t)*size);
	for (int i = 0; i < size; i++)
		Entry[i].next = NULL;
	return Entry;
}

void db_close(db_t* db) {
	for (int i = 0 ; i < Gsize; i++){
		db_t *pos;
		db_t *tmp;
		if ((db+i)->next ==NULL) continue;
		for (pos = (db+i)->next; pos != NULL; pos = tmp){
			tmp = pos->next;
			free(pos->val);
			free(pos->key);
			free(pos);
		}
	}
	free(db);
	Triefree(root);
}

void db_put(db_t* db, char* key, int keylen, char* val, int vallen) {
	int index = hash_func(key);
	db_t *pos;
	
	//Check in Hash Table
	for (pos = (db+index); pos->next != NULL; pos = pos->next) 
		if (!strcmp(pos->next->key, key)){
			for (int i = 0; i < vallen; i++) pos->next->val[i] = val[i];
			return;
		}
	
	db_t *new = pos->next = (db_t*)malloc(sizeof(db_t));	
	new->next = NULL;
	new->val = (char*)malloc(sizeof(char)*(vallen));
	new->key = (char*)malloc(sizeof(char)*(keylen+1));
	new->vallen = vallen;
	new->keylen = keylen+1;
	for (int i = 0; i < keylen; i++)  new->key[i] = key[i]; 
	new->key[keylen] = '\0';//For strcmp 
	for (int i = 0; i < vallen; i++)  new->val[i] = val[i]; 
	
	//Check the number of element
	//If table is full, make file.
	element_num++;
	if (element_num >= Gsize){
		file_num++;
		char filename[50];
		sprintf(filename, "./db/file-%d", file_num);
		int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);

		for (int i = 0 ; i < Gsize; i++){
			db_t *pos;
			db_t *tmp;
			if ((db+i)->next ==NULL) continue;
			for (pos = (db+i)->next; pos != NULL; pos = tmp){
				insert(pos->key, file_num, lseek(fd, 0, SEEK_CUR));	//lseek return current offset
				int trashcan;
				trashcan = write(fd, &(pos->keylen), 4); 
				trashcan = write(fd, pos->key, pos->keylen); 
				trashcan = write(fd, pos->val, 4);
				trashcan++; 
				tmp = pos->next;
				free(pos->val); free(pos->key); free(pos);
			}
			db[i].next = NULL;
		}
		close(fd);
		element_num = 0;
	}
}

/* Returns NULL if not found. A malloc()ed array otherwise.
 * Stores the length of the array in *vallen */
char* db_get(db_t* db, char* key, int keylen, int* vallen) {
	int index = hash_func(key);
	db_t *pos;
	for (pos = (db+index)->next; pos != NULL; pos = pos->next) 
		if (!strcmp(pos->key, key)) {
			*vallen = pos->vallen;
			char *val = (char*)malloc(sizeof(char)*(pos->vallen));
			for (int i = 0; i < pos->vallen; i++) val[i] = pos->val[i]; 
			return val;
		}//Search in Hash Table
	
	Node *found;
	if ((found = find(key)) != 0){
		if (found->latest == -1) return NULL;
		char filename[50];
		sprintf(filename, "./db/file-%d", found->latest);
		
		int fd = open(filename, O_RDONLY);	
		lseek(fd, found->offset, SEEK_SET);	
		int kl;
		char val[4];
		char temp[1024];
		int trashcan;
		trashcan = read(fd, &kl, 4);
		trashcan = read(fd, temp, kl);
		trashcan = read(fd, val, 4);
		trashcan++;
		
		char *val2 = (char*)malloc(sizeof(char)*4);
		for (int i = 0; i < 4; i++) val2[i] = val[i];		
		close(fd);
		return val2;
	}
	return NULL;
		
}
