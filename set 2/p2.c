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
  int fd = open("10.txt",O_RDONLY);
  
  if((n = read(fd,encoded_text,BUFSIZE)) < 0){
  	printf("Give valid text for decryption\n");
  	printf("Exiting now\n");
  	sleep(3);
  	exit(0);
  }

  
  // Removing backspaces


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
  *(encrypted_text+str_len) = '\0';
  

  unsigned char * plain_text = (unsigned char *)malloc(BUFSIZE);
  unsigned char * plain_block = (unsigned char *)malloc(BLOCKSIZE);
  unsigned char * cipher_block = (unsigned char *)malloc(BLOCKSIZE);
  unsigned char *iv = (unsigned char *)malloc(BUFSIZE);
  strncpy(iv,"00000000000000000000000000000000",2*BLOCKSIZE);
  write(1,iv,2*BLOCKSIZE);
  printf(" is iv.%ld\n",strlen(iv));

  int p;
  for(p=0;p<str_len;p=p+BLOCKSIZE){
    write(1,encrypted_text+p,BLOCKSIZE);
    write(1,key,BLOCKSIZE);

    aes_ecb128_decrypt(encrypted_text+p,BLOCKSIZE,key,iv,plain_block);

    
    asciitobytestr(plain_block,BLOCKSIZE);
    *(plain_block+2*BLOCKSIZE) = '\0';
    fixedxor(plain_block,plain_block,iv);
    

    bytestrtoascii(plain_block);
    

    strncpy(plain_text+p,plain_block,BLOCKSIZE);    

    strncpy(iv,encrypted_text+p,BLOCKSIZE);
    asciitobytestr(iv,BLOCKSIZE);
  }
  *(plain_text+strlen(plain_text)) = '\0';
  printf("Decrypted text is:\n");
  BIO_dump_fp (stdout, (const char *)plain_text, str_len);

  printf("Exiting now\n");
  sleep(3);
  exit(0);
  return 0;
}
