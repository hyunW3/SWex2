typedef struct db {
	int vallen;
	int keylen;
	char *key;
	char *val;
	struct db *next;
}db_t;


db_t* db_open(int size);
void db_close(db_t* db);
void db_put(db_t* db, char* key, int keylen,char* val, int vallen);
/* Returns NULL if not found. A malloc()ed array otherwise.
 * Stores the length of the array in *vallen */
char* db_get(db_t* db, char* key, int keylen, int* vallen);



