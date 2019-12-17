#include "util.h"

int main()
{
  int n;
  printf("Provide 128 bit key for decryption\n");
  unsigned char * key = (unsigned char *)malloc(16);
  if(16 != read(0,key,16)){
  	printf("Exiting now\n");
  	sleep(3);
  	exit(0);
	}


  char * encoded_text  = (char *)malloc(BUFSIZE);
  unsigned char * encrypted_text = (unsigned char *)malloc(BUFSIZE);
  
  // Aes plain_text 
  int fd = open("7.txt",O_RDONLY);
  
  if((n = read(fd,encoded_text,BUFSIZE)) < 0){
  	printf("Give valid text for decryption\n");
  	printf("Exiting now\n");
  	sleep(3);
  	exit(0);
  }

  
  int k = 0;
	int j;
	while(k< BUFSIZE){
		if(*(encoded_text+k) == '\n'){
    for(j=k;j<BUFSIZE-1;j++)
				*(encoded_text + j) = *(encoded_text+j+1);
		}
    k++;
	}

  base64tohex(encrypted_text,encoded_text);


  int str_len;
  str_len = strlen(encrypted_text)/2;

  bytestrtoascii(encrypted_text);

  printf("ascii str len is %d\n",str_len);

  // Allocate memory for plain_text
  unsigned char * plain_text = (unsigned char *)malloc(BUFSIZE);
  /* A 128 bit IV */
  unsigned char *iv = (unsigned char *)"";

  *(encrypted_text+str_len) = '\0';



  printf("the one that is visible is %ld\n",strlen(encrypted_text));
  // str_len = (strlen(encrypted_text)/16)*16;
  // *(encrypted_text+(strlen(encrypted_text)/16)*16) = '\0';
  // printf("the one that is visible is %ld\n",strlen(encrypted_text));

  // Encryption
  int plaintext_len;
  plaintext_len = aes_ecb128_decrypt(encrypted_text, str_len, key,iv,plain_text);	


  printf("Decrypted text is:\n");
  BIO_dump_fp (stdout, (const char *)plain_text, plaintext_len);

  printf("Exiting now\n");
  
  sleep(3);
  exit(0);
  return 0;
}
