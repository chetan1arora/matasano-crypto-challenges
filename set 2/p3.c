#include "util.h"


int main(int argc, char const *argv[])
{
	char * plaintext = (char *)malloc(BUFSIZE);
	char * ciphertext = (char *)malloc(BUFSIZE);
	int plaintext_len;
	if((plaintext_len = read(0,plaintext,BUFSIZE)) < 0){
		printf("Input not clearly given.\n");
		exit(0);
	}
	int ciphertext_len = oracle_encrypt(ciphertext,plaintext,plaintext_len);
	
	if(ciphertext_len == 0){
		printf("some problem with encryption\n");
	}
	printf("In ascii:\n");
	write(1,ciphertext,ciphertext_len);
	printf("\n");
	asciitobytestr(ciphertext,ciphertext_len);
	printf("In hex:\n");
	write(1,ciphertext,ciphertext_len);
	printf("\n");
	return 0;
}

