//------------------------------------------------------------------
// SSE2033: System Software Experiment 2 (Fall 2019)
//
// Skeleton code for PA #0
//
// Computer Systems and Intelligence Laboratory
// Sungkyunkwan University
//
// Student Name  : Bae HyunWoong 
// Student ID No.: 2016310932
//------------------------------------------------------------------

//buffer - malloc realloc
//printf -> write
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define buff_size 80

int sp=0;
int length =0;
//main function
void trim(char word[]);
int getinput(char* buff,char* word);
void add(char *word);

// sub func
char tolow(char ch);
void cpy(char *word1, char* word);
int Check(char a);
int isCap(char word);

// compare func
int com(char * word1, char *word2);
int comp_inlower(char word_1,char word_2); // incase cap vs low

//linked list print
typedef struct node{	
	char* word;
	int number;
	struct node* next;
}Node;
Node *list=NULL;

// ASCII CODE
// A(65) - Z(90) / a(97)-z(122)
// -(45) , '(39)
// 65(A) , 97(a) , 66(B), 98(b) ... like this
//dataset
int main()
{
	char buff[2];
	char word[buff_size];
	char num[80]; // for sprintf
	while(read(0,buff,1) > 0){
		if(getinput(buff,word)&&length !=0 ){
			add(word);	
		}
	}
	// printf list
	Node *p = list;
	while(p != NULL){
	//	printf("%s %d\n",p->word,p->number);
		sprintf(num," %d\n",p->number);	
		// print format : word num	
		write(1,p->word,strlen(p->word));
		write(1,num,strlen(num));
		p = p->next;
	}
	return 0;
}

//char* word;
//node->word = (char*)malloc(sizeof(char)*100);



char tolow(char ch){
	if(ch>='A' && ch<='Z'){
		return (char)(ch+32);
	} else {
		return ch;
	}
}
int Check(char ch){ 
        if(ch == '\'' || ch == '-'){
                return 2;
        } else if((ch>='a' && ch<='z')||(ch>='A' && ch<='Z')  ){  
                return 0;
        } else  return 1;  
}
int getinput(char* buff,char* word) { // whether -,', getinput anyway
        int stat = Check(buff[0]);
//      char* word_trim;

        if(stat == 1 ) { // not alphabet & two
                if(sp == 0) return 0;
                word[sp] = '\0';
		length = sp; // null 포함 sp+1(0~sp)
	//	printf("len is %d(%d)\n",strlen(word),length);
		
                trim(word);
                return 1;
        } else {
		if(stat == 2 && sp == 0) { // first intput ' & -
			return 0;
		}else {
                	word[sp] = buff[0];
			sp++;
		}
        }
        return 0;
}
void trim(char word[]){
        int start =0;
//	printf("%s\nlength :%d\n",word,length);
	if(length > 0) { // back check
      	  	if(Check(word[length-1]) == 2 && word[length-1] !='\0' ) {
//			printf("back cleaning in \n");
         	       for(int i=length-1; i>=0; i--){
                	        int stat = Check(word[i]);
//				printf("%c:%d\n",word[i],stat);
                	        if(stat == 2 ) {
                	                sp--;
					word[i] = '\0';
                	        } else {
                	                break;
        	                }
        	       	 }
        	}
	} // insert
	for(int i=0; i<sp; i++){
		word[i] = word[start+i];
        }

	length = sp+1;
        sp =0;
}
int isCap(char word) {
	if(word >='A' && word <='Z'){
		return 1;
	} else {
		return -1;
	}
}
int comp_inlower(char word_1,char word_2){
	int flag1 = isCap(word_1);
	int flag2 = isCap(word_2);
	
	int word1 = tolow(word_1);
	int word2 = tolow(word_2);
	int com = word1-word2;
	if(com == 0){
		if(flag1 == flag2) {
			return 2;
		}else if(flag1 > flag2) {
			return 0;
		} else { // flag1< flag2
			return 1;
		}
		
	} else if(com<0){
		return 0;
	} else { // com >0
		return 1;
	}
}

int com(char * word1, char *word2){
	int i=0;
	// same 2 / word1 < word2 0 / word1 > word2 1
	int len1 = strlen(word1);
	int len2 = strlen(word2);
	while(1) {
		int check = (int)(word1[i] - word2[i]);
		if(check == 0) { // same in i size
			if(len1 > i && len2 > i){
				i++;
			} else { // i > len1 or len2
					//flag = 1; // len1 != len2
 					if(len1 > len2) {
						return 1;
					} else if(len1 < len2) {  
						return 0;
					} else {
						if(strcmp(word1,word2) == 0) {
							return 2;
						} else {
							return -1; // error	
						}	
					}		
			}	
	
		} else {  // check != 0 word1 > word2
			return comp_inlower(word1[i],word2[i]);
		}

	}
			
}

void cpy(char *word1, char* word){
	for(int i=0; i<length; i++){
		word1[i] = word[i];
	}
}
void add(char *word){
	Node *new = (Node*)malloc(sizeof(Node));
	new->word = (char*)malloc(sizeof(char)*length+10);
	cpy(new->word,word);
	new->number = 1;
	new->next = NULL;

	Node *cur = list;
	Node *pre = NULL; 
while(1){
	if(cur == NULL) { 
		list = new; 
		break;
	}else {
	
	int a = com(cur->word,new->word); 
	if(a == 2) { 
		cur->number++;
		free(new->word);
		free(new);
		break;
	} else if(a == 0){ // word1 < word2
		if(cur->next != NULL){ // cur - new check further
			pre = cur;
			cur = cur->next; 
			continue;
		} else { // attach tail
			cur->next = new;
			break;
		}	
	} else if(a== 1){ // word1 > word2
		if(pre != NULL){  // mainly insert in here	
			pre->next = new;
			new->next = cur;
				
		} else { // fore front
			list = new;
			new->next = cur;
		}
		break;
	}
	}
}
length=0;
}

