#include "util.h"


int main(){
	// Set global aes key variable
	//char * key_copy = (char *)malloc(BLOCKSIZE);

	//int kb = RAND_bytes(aes_key,sizeof(aes_key));
	memcpy(aes_key,"1749f4b129b41e9b2033a7c467b9fe9d",2*BLOCKSIZE);

	//memcpy(key_copy,aes_key,BLOCKSIZE);
	bytestrtoascii(aes_key);

	// if(kb != 1){
	// 	printf("aes key generation failed\n");
	// 	exit(0);
	// }
	
	char * plaintext = (char *)malloc(BUFSIZE);
	char * ciphertext = (char *)malloc(BUFSIZE);
	char * pwn = (char *)malloc(BUFSIZE);
	// int plaintext_len;
	// if((plaintext_len = read(0,plaintext,BUFSIZE)) < 0){
	// 	printf("Input not clearly given.\n");
	// 	exit(0);
	// }

	// if(*(plaintext+plaintext_len-1) == '\n'){
	// 	*(plaintext+plaintext_len-1) = '\0';
	// 	plaintext_len = plaintext_len-1;
	// }
	// int ciphertext_len = oracle_encrypt(ciphertext,plaintext,plaintext_len);

	// if(ciphertext_len == 0){
	// 	printf("some problem with encryption\n");
	// }
	
	// Break ecb


	char cipherblock[BLOCKSIZE];
	int p = 153;
	int min = 15;
	int k;
	printf("Initiating ecb breaking\n");

	for(k=0;k<p;k++){
		*(plaintext+k) = 'a';
	}
	int g,e;
	int found = 0;
	printf("Initiating ecb breaking\n");
	int len = 0;
	for(k=p-1;k>min-1;k--){
		// The real value
		*(plaintext+k) = '\0'; 
		g = oracle_encrypt(ciphertext,plaintext,k);
		if(g <=0){
			printf("Problem with oracle function\n");
			exit(0);
		}
		memcpy(cipherblock,ciphertext+(p/16)*BLOCKSIZE,BLOCKSIZE);
 		found = 0;
 		printf("no problem till this\n");
 		for(e=0;e<len;e++){
 				*(plaintext+p-1-len+e) = pwn[e];
 			}
 		e =0;
 		for(g=0;g<256;g++){
 			*(plaintext+p-1) = (char)g;
 			e = oracle_encrypt(ciphertext,plaintext,p);
 			if(memcmp(cipherblock,ciphertext+(p/16)*BLOCKSIZE,BLOCKSIZE) == 0){
 				*(pwn+p-k-1) = (char)g;
 				printf("char is %c\n",(char)g);
 				found = 1;
 				len++;
 				break;
 			}

 		}
 		if(found !=1){
 			printf("Error in logic\n");
 			exit(0);
 		}

	}

	printf("pwn is %s\n",pwn);
	
	return 0;
}
