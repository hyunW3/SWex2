void failure(char* key, int* fail, int key_len){
	int i =1;
	int j=0;
	fail[0]=0;
while(i < key_len){ 
	if(key[i] == key[j]){ // pattern same
		fail[i]= j+1;
		i++; j++;
	} else if(j>0){ // pattern is diff but since same
		j = fail[j-1];
	} else { // totallu different
		fail[i]=0;
		i++;
	}
}

int KMP(char* s1, int s1_size,char* key, int key_len,int*fail){
	int j=0;
	for(int i=0; i<s1_size; i++){
		if(s1[i] == key[j]){
			if(j== key_len-1){
				return 1; // find value
			}else {
				i++;
				j++;
			} 
		} else if(j>0) { // 이전까지 같ㅇ았다면
			j = fail[j-1];
		} else {
			i++''
		}
	}
	return -1;

}