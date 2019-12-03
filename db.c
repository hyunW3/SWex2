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
#include <wait.h>

#define MAXSIZE 100000
int* reader_num;
pthread_mutex_t read_lock = PTHREAD_MUTEX_INITIALIZER; 
//pthread_mutex_t write_lock = PTHREAD_MUTEX_INITIALIZER; 
pthread_mutex_t file_lock= PTHREAD_MUTEX_INITIALIZER;

int table_num=0; // count for how many element in table
int num=-1; // for making file number
//int infile = 0;
void cpy(char *word1, char* word, int length){
	for(int i=0; i<length; i++){
		word1[i] = word[i];
	}
}
unsigned int hash(char *key,int length, int Lsize){
	// hash func in here
	unsigned int sum=0;
	sum = key[0];
	for(int i=1; i<=length; i++){
		sum += key[i];
	}
	return sum%Lsize;

}
db_t *db_open(int size)
{
	mkdir("./db",0777); // mk file directory
	
	int fd[2];
	if(pipe(fd));
	if(fork()==0){ // child
		close(fd[0]);
		dup2(fd[1],fileno(stdout));
		execl("/bin/ls","ls","-t","./db/",NULL);
	} else {
		wait(NULL);
		close(fd[1]);
		char * cat = (char*)malloc(sizeof(char)*200);
		if(read(fd[0],cat,200));
		if(cat == NULL) fprintf(stderr,"no DB found\n");
		if(strlen(cat) != 0) {
			int i=0;
			for(; cat[i] != '-'; i++);
			i++;
			int start = i;
			for(; cat[i] != '\n'; i++){
				//printf("%c",cat[i]);
			}
			//char p[140];
			strncpy(cat,cat+start,i-start);
			num = atoi(cat);
			//printf("num:%d\n",num);			
		}
		//close(fd[0]);
	}

	db_t *db = NULL;
	db = (db_t*)malloc(sizeof(db_t)*size);
	Lsize = size;	
	entry_lock = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t)*size);
	reader_num =(int*)malloc(sizeof(int)*size);
	for(int i=0; i<size; i++){ // initializer
		(db+i)->next = NULL;
		reader_num[i]=0;
		pthread_mutex_init(&entry_lock[i],NULL);
	}
	pthread_mutex_init(&file_lock,NULL);
	
	return db;
}

void db_close(db_t *db)
{
	//free(db);
	// insert loading table
	
		//printf("laoding last%d\n",num);
	// should be check when write is end
	for(int i=0; i<Lsize; i++){
		pthread_mutex_lock(&entry_lock[i]);
	}
	if(table_num==0) { // no date to store
		return;
		free(db);
	}
	for(int i=0; i<Lsize; i++){
		pthread_mutex_unlock(&entry_lock[i]);
	}
		char * buf1 = malloc(sizeof(char)*(1300));
	pthread_mutex_lock(&file_lock);
		num++;	
		sprintf(buf1,"./db/file-%d",num);
		int fd = open(buf1,O_RDWR | O_CREAT | O_TRUNC,0755); 
		for(int i =0; i<Lsize; i++){
			db_t* cu = (db+i); // to search table
			db_t* tmp = cu->next;
			if(tmp == NULL) continue;
			for(cu = tmp; cu != NULL; cu = tmp) { // key/value
				int trash;
			//	if(cu->key == NULL || cu->value == NULL) continue;
				sprintf(buf1,"%s/%d\n",cu->key,cu->value);
				//printf("buf1:%s\n",buf1);
				trash = write(fd,buf1,strlen(buf1));
				trash++;

				tmp = cu->next;
				//cu->key=NULL;

				free(cu->key);  
				//free(cu);
			}
			(db+i)->next = NULL;
		}
		free(buf1);
		close(fd);
		free(db);
		free(reader_num);
	pthread_mutex_unlock(&file_lock);
	pthread_mutex_destroy(&entry_lock);	
	pthread_mutex_destroy(&read_lock);
	pthread_mutex_destroy(&file_lock);
}

void db_put(db_t *db, char *key, int key_len,
			char *val, int val_len) // (char*)&cnt == char* val
{
write(1,key,strlen(key));
write(1,"\n",1);
write(1,val,16);
write(1,"\nkey-",6);
	// search the key
	int entry = hash(key,key_len,Lsize);
	db_t *cur = db+entry;
	// make new db for key value
	db_t *new = (db_t*)malloc(sizeof(db_t));
	new->next = NULL;
	
	new->key = (char*)calloc(sizeof(char),(key_len+1));
	new->key_len = key_len+1;
	cpy(new->key,key,key_len+1);
	//new->value = *((int*)val); // nothing in new-value
	//new->value = atoi(val);
	new->val_len = val_len;
	//int val_int = *((int*)val); // this one cval
	
	int val_int = atoi(val);
	new->value = val_int;
	write(1,new->key,new->key_len);
	//write(1,new->key,new->key_len);
	write(1,"\nvalue-",7);
	
	char buf[MAXSIZE];
	sprintf(buf,"%d",new->value);
	write(1,buf,strlen(buf));
	write(1,"\n",1);
	int add=0;
	printf("entry:%d, %d\n",entry,reader_num[entry]);
	pthread_mutex_lock(&entry_lock[entry]);
	while(cur->next != NULL){ 
		cur = cur->next;
		if(strcmp(cur->key,key) == 0) { // if in the table
			cur->value = val_int;
			add=1; //table_num++;
			break;
		}
	}	
	if(add ==0){ // not in table
		//memset(buf,'\0',sizeof(buf));
		cur->next = new;
		sprintf(buf,"%s %d is add\n",new->key,new->value);
		write(1,buf,strlen(buf));
		table_num++;
	}
	pthread_mutex_unlock(&entry_lock[entry]);
	// table is full
	printf("table_num:%d, Lsize:%d\n",table_num,Lsize);
	if(table_num >= Lsize){
		for(int i=0; i<Lsize; i++){
			pthread_mutex_lock(&entry_lock[i]);
		}
		char * buf1 = malloc(sizeof(char)*1300);
	pthread_mutex_lock(&file_lock);	
		num++;
		sprintf(buf1,"./db/file-%d",num);
		printf("db_put.buf1:%s\n",buf1);
		int fd = open(buf1,O_RDWR | O_CREAT | O_TRUNC,0755);
		int trash=0;
		for(int i =0; i<Lsize; i++){
			db_t* cu = (db+i); // to search table
			db_t* tmp = cu->next;
			if(tmp == NULL) continue;
			for(cu = tmp; cu != NULL; cu = tmp) { // key/value
				sprintf(buf1,"%s/%d\n",cu->key,cu->value);
				trash += write(fd,buf1,strlen(buf1));
				tmp = cu->next;
				//cu->key=NULL;

				free(cu->key);  
				//free(cu);
			}
			(db+i)->next = NULL;
		}
		close(fd);
		if(trash <=0) {// write nothing but create
			unlink(buf1);
			num--;
		}
		free(buf1);
		table_num=0;
	pthread_mutex_unlock(&file_lock);	
	for(int i=0; i<Lsize; i++){
		pthread_mutex_unlock(&entry_lock[i]);
	}		
	}

}
/* Returns NULL if not found. A malloc()ed array otherwise.
 * Stores the length of the array in *vallen */
char *db_get(db_t *db, char *key, int key_len,
			int *val_len) // finding if the key in db 
		// val_len == 4 in char size
{
	//infile = 0;
	char *value = NULL;
	pthread_mutex_lock(&read_lock);
	int entry = hash(key,key_len,Lsize);
	reader_num[entry]++;	
	if(reader_num[entry] ==1) pthread_mutex_lock(&entry_lock[entry]);
	pthread_mutex_unlock(&read_lock);
	printf("get_entry:%d, %d\n",entry,reader_num[entry]);
	db_t *cur = db+entry;   
	// in db, db[entry] and find it
	int flag=0;
	if(cur->next != NULL){ // in db there is no data
		//printf("find in table\n");
		while(cur->next != NULL){
			cur = cur->next;
			if(strcmp(cur->key,key) == 0 ){ // find the value
				value = (char*)malloc(sizeof(char)*(cur->val_len+1));
				*(int*)value = cur->value;
				val_len = &cur->val_len;
				flag=1;
				//return value;
			} 
		} 		

	}else {
		//printf(" nothing...\n");
		
	}
	pthread_mutex_lock(&read_lock);
	reader_num[entry]--;
	if(reader_num[entry] ==0) pthread_mutex_unlock(&entry_lock[entry]);
	printf("after get_entry:%d, %d\n",entry,reader_num[entry]);
	pthread_mutex_unlock(&read_lock);

	if(flag==1) return value; //
	// find in the file
		printf("num:%d\n",num);
	for(int k=num; k>=0; k--){
		char *buf1 = malloc(sizeof(char)*MAXSIZE);
		sprintf(buf1,"./db/file-%d",k);
		//printf("k:%d,buf1:%s\n",k,buf1);
		struct stat file_info;
		stat(buf1,&file_info);
		int file_size = file_info.st_size;
		if(file_size <=0) {
			free(buf1);
			continue;
		}
		char* s1 = (char*)malloc(sizeof(char)*(file_size));
	pthread_mutex_lock(&file_lock);	// to avoid num change
		int fd = open(buf1, O_RDONLY);
		int trash = read(fd,s1,sizeof(char)*file_size);
	pthread_mutex_unlock(&file_lock);
		trash++;
		int sp =0;
	  while(sp<file_size) { // check line (number:Lsize) 
		char bar = '/';		
		char enter = '\n';
		int i;
		if(s1[sp] == key[0]){
			char* str = (char*)malloc(sizeof(char)*1024);
			for(i=0; (s1[sp] != bar); sp++){
				str[i] = s1[sp];
				i++;
			} str[i] ='\0'; // i = str len
			sp++;
			if(strcmp(str,key)==0){	
				// num value 
				char* num1 = (char*)malloc(sizeof(char)*100);
				for(i=0; s1[sp] != enter; sp++){
					 num1[i] =  s1[sp]; 
					 i++;
				} num1[i] = '\0';
				sp++;
				// num value to VALUE
					value = (char *)malloc(sizeof(char)*strlen(str));
					*(int*)value = atoi(num1);
			
					//infile =1;
					free(num1); free(str); close(fd); free(s1);
					return (value);	
			}else {
				for(; (s1[sp] != enter); sp++);
				sp++;				
			}			// this else necessary???
			
			free(str);
		} else {
			for(; (s1[sp] != enter); sp++);
			sp++;
		}
		
	  }
	close(fd);
	free(s1);
	free(buf1);
	}
		
	if(value != NULL) printf("VALUE:%s\n",value);
	else printf("VALUE:NULL\n");
	return value;
}




