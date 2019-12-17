#include "util.h"

int main()
{
	int n,fd;
	char * encoded_text = (char *)malloc(BUFSIZE);
	char * key = "YELLOW SUBMARINE";
	char * nounce = (char *)malloc(BLOCKSIZE);
	int i=0;
	for(;i<BLOCKSIZE;i++){
		*(nounce+i) = '\0';
	}

	if((fd=open("ctr_text.txt",O_RDONLY))< 0){
		printf("Error opening file\n");
		exit(0);
	}
	
	n = read(fd,encoded_text,BUFSIZE);
	write(1,encoded_text,n);

	if(*(encoded_text+n-1) == '\n'){
		*(encoded_text+n-1) = '\0';
		n--;
	}
	char * cipher_text = (char *)malloc(BUFSIZE);
	base64tohex(cipher_text,encoded_text);
	n = strlen(cipher_text)/2;
	bytestrtoascii(cipher_text);

	// Give data for the key

	printf("decryption happened :%d\n",aes_128_ctr_decrypt(key,nounce,cipher_text,n));

	printf("plain text is:\n");
	write(1,cipher_text,n);
	write(1,"\n",1);

	return 0;
}