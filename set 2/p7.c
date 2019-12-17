#include "util.h"


int fun1(char * input,int input_len,char * cipher_text){

	char tmp[5*BLOCKSIZE];
	char *p;


	memcpy(tmp,input,input_len);
	*(tmp+input_len) = '\0';
	while(p = strchr(tmp,'=')){
		*(p) = '\0';
	}
	while(p = strchr(tmp,';')){
		*(p) = '\0';
	}
	char tmp2[5*BLOCKSIZE];

	while(p = strchr(tmp,' ')){
		strcpy(tmp2,p+1);
		strcpy(p,"%20");
		strcpy(p+3,tmp2);
	}

	int str_len = strlen("comment1=cooking%20MCs;userdata=");
	memcpy(input,"comment1=cooking%20MCs;userdata=",str_len);
	strcat(input,tmp);
	strcat(input,";comment2=%20like%20a%20pound%20of%20bacon");

	str_len = strlen(input);
	*(input + str_len) = '\0';
	printf("len is %d\n",str_len);
	int padded_len = pkcs7padding(input,str_len,BLOCKSIZE);	

	return aes_cbc128_encrypt(input,padded_len,aes_key,aes_iv,cipher_text);

}

int fun2(char * cipher_text,int cipher_len){
	char text[cipher_len];
	aes_cbc128_decrypt(cipher_text,cipher_len,aes_key,aes_iv,text);	

	int p;
	for(p=0;p<cipher_len;p=p+BLOCKSIZE){
		write(1,text+p,BLOCKSIZE);
		write(1,"\n",1);		
	}


	write(1,"\n",1);
	if(memmem(text,cipher_len,";admin=true;",strlen(";admin=true;"))){
		return 1;
	}
	return 0;
}



int main(int argc, char const *argv[])
{

	memcpy(aes_key,"1749f4b129b41e9b2033a7c467b9fe9d",2*BLOCKSIZE);
	bytestrtoascii(aes_key);

	memcpy(aes_iv ,"29ff850f0559ac21f502eed3ce236b38",2*BLOCKSIZE);
	bytestrtoascii(aes_iv);

	int n;
	char * input = (char *)malloc(BUFSIZE);
	char * cipher_text = (char *)malloc(BUFSIZE);
	if((n=read(0,input,BUFSIZE))<0){
		printf("Give valid input\n");
		exit(0);
	}
	if(*(input+n-1) == '\n'){
		*(input+n-1) = '\0';
		n--;
	}
	int cipher_len = fun1(input,n,cipher_text);
	//*(input + padded_len) = '\0';
	int p ;
	printf("Final string is\n");

	for(p=0;p<cipher_len;p=p+BLOCKSIZE){
		write(1,input+p,BLOCKSIZE);
		write(1,"\n",1);		
	}

	printf("Encrypted string is\n");
	char needed[BLOCKSIZE] = "aaaaa;admin=true";
	char tmp[BLOCKSIZE] = "aaaaaaaaaaaaaaaa";
	for(p=0;p<cipher_len;p=p+BLOCKSIZE){
		write(1,cipher_text+p,BLOCKSIZE);
		write(1,"\n",1);
		if(p==32){
			fixedxor_ascii(tmp,cipher_text+p,tmp,BLOCKSIZE);
			fixedxor_ascii(cipher_text+p,tmp,needed,BLOCKSIZE);
		}

	}

	int admin = fun2(cipher_text,cipher_len);
	if(admin == 1){
		printf("Welcome admin\n");
	}
	else{
		printf("No access allowed\n");
	}


	//printf("\n");
	return 0;
}