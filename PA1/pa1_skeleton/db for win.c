#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_KEYLEN 1024
typedef struct db {
	/* Hash Table */
	char *key;
	int key_len;
	char *value;
	int val_len;
	struct db *next;	// store memory in start at db->next
    /* Something else */
} db_t;
// db.c
// new->value & value cpy problem -> no \0 in that 
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
	printf("thing to put:%s %d\n",key,*((int*)val));
	// search the key
	int entry = hash(key,key_len,Lsize);
	db_t *cur = db+entry;
	// make new db for key value
	db_t *new = (db_t*)malloc(sizeof(db_t));
	new->next = NULL;
	
	new->key = (char*)malloc(sizeof(char)*(key_len+1));
	cpy(new->key,key,key_len);
	new->key[key_len] = '\0';
	new->key_len = key_len+1;
	
	new->value = (char*)malloc(sizeof(char)*val_len);
	cpy(new->value,val,val_len);
	//strcpy(new->value,val);
	new->val_len = val_len;
	printf("in new:%s %s\n",new->key,new->value);
	
	cur = cur->next;
	int val_int = *((int*)val);
	printf("val:%d\n",val_int);
	if(val_int == 1){ // if there is (val == 1) add the tail
		printf("1\n");
		while(cur!= NULL){
			cur = cur->next;
		} 
		cur = new;
	}  else { 	// if not, find the key & insert the val(&cnt)
		free(new);
		printf("2\n");
		while(strcmp(cur->key,key) == 0) {
			cpy(cur->value,val,val_len);
			printf("cur_val : %s\n",cur->value);
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
	cur = cur->next;
	printf("hash is %d\n",entry);
	if(cur == NULL){ // nothing in the entry
		printf("nothing\n");
		return value;
	} else {
		printf("finding...\n");
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
int main(int argc, char *argv[])
{
	db_t *DB;
	char key[MAX_KEYLEN];
	char *val;
	int size =128;
	int ret, cnt, key_len, val_len;

	//if (argc != 2) {
		//printf("Usage: %s size\n", argv[0]);
	//	return -1;
	//}

	//size = atoi(argv[1]);
	DB = db_open(size);
	if (DB == NULL) {
		printf("DB not opened\n");
		return -1;
	}
	printf("DB opened\n");
	
	while ((ret = scanf("%s", key)) != -1) {
		key_len = strlen(key);
		val = db_get(DB, key, key_len, &val_len);
		if (val == NULL) {
			printf("GET [%s] [NULL]\n", key);
			cnt = 1;
		} else {
			printf("GET [%s] [%d]\n", key, *((int *)val));
			cnt = *((int*)val) + 1;
			free(val);
		}
		printf("%d %s\n",cnt,(char *)&cnt);
		db_put(DB, key, key_len, (char *)&cnt, sizeof(int));
		printf("PUT [%s] [%d]\n", key, cnt);
		printf("\n"); // arbi
	}

	db_close(DB);
	printf("DB closed\n");
	return 0;
}

