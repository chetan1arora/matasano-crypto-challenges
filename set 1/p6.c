#include "util.h"

/*
* Break Repeat key xor cipher
*/

int main(){
	// Input base64 from a file
	int fd = open("6.txt",O_RDONLY);
	char * encoded_str = (char *)malloc(BUFSIZE);

	read(fd,encoded_str,BUFSIZE);
	
	// Remove crlf from string

	
	// Remove newline characters from the string
	int k = 0;
	int j;
	while(k< BUFSIZE){
		if(*(encoded_str+k) == '\n'){
			for(j=k;j<BUFSIZE-1;j++)
				*(encoded_str + j) = *(encoded_str+j+1);
		}
		k++;
	}


	printf("encoded string is %s\n",encoded_str);
	

	char * byte_str = (char *)malloc(BUFSIZE);
	base64tohex(byte_str,encoded_str);
	
	printf("hex string is %s\n",byte_str);
	
	free(encoded_str);
	

	char * key = (char *)malloc(50);
	break_repkey_xor(key,byte_str);


	printf("Key found is %s\n",key);
	bytestrtoascii(key);

	printf("key is :%s\n",key);

	printf("Decypting now\n");


	char * decrypt_str = (char *)malloc(BUFSIZE);


	repeatkeyxor(decrypt_str,byte_str,key);

	//bytestrtoascii(decrypt_str);

	char * test = (char *)malloc(2);
	int p = 0;
	int str_len = strlen(decrypt_str);

	//for(;p<str_len;p=p+2){
	//	test[0] = *(decrypt_str+p);
	//	test[1] = *(decrypt_str+p+1);
	//	test[2] = '\0';
	//	printf("test is %s\n",test);
	//	if(!strcmp(test,"00")){
	//		*(decrypt_str+p+1)= 'a';
	//		printf("test is %s and p is %d\n",test,p);
	//	}

	//}
	printf("Decrypted str is :%s\n",decrypt_str);

	bytestrtoascii(decrypt_str);

	fd = open("decrypted.txt",O_CREAT|O_WRONLY);
	write(fd,decrypt_str,BUFSIZE);
	close(fd);
	free(decrypt_str);
	free(key);
	free(byte_str);


	return 0;

}
