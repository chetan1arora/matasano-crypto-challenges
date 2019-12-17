#include "util.h"

int main(){

	
	char * padded_text = (char *)malloc(BUFSIZE);
	strcpy(padded_text,"hellbuddy!!");
	int str_len = strlen(padded_text);

	int padded_len = pkcs7padding(padded_text,str_len,BLOCKSIZE);

	*(padded_text+padded_len-1) = '\5';

	printf("is padding pkcs7padding %d\n",is_valid_pkcs7(padded_text,padded_len));

	return 0;

}