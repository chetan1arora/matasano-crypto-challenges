#include "util.h"


int fun1(char * cipher_text){
	
	int p;	
	char * line = (char *)malloc(BUFSIZE);
	FILE *fp = fopen("cbc_text.txt","r");
	if(!fp){
		//printf("Some error opening file:%m\n");
		exit(0);
	}

	// Giving no. of lines manually
	p = rand()%1;
	int k=0;
	size_t delim;
	while(k!=p+1){
		if(getline(&line,&delim,fp) == -1){
			//printf("error in readline %m\n");
	}
		k++;
	}
	int str_len = strlen(line);
	if(*(line+str_len-1) == '\n'){
		*(line + str_len-1) = '\0';
		str_len--;
	}
	//printf("str len is %d\n",str_len);
	char * plain_text = (char *)malloc(BUFSIZE);
	base64tohex(plain_text,line);
	//printf("hex string len is %ld\n",strlen(plain_text));

	bytestrtoascii(plain_text);
	str_len = strlen(plain_text);
	int padded_len = pkcs7padding(plain_text,str_len,BLOCKSIZE);	

	return aes_cbc128_encrypt(plain_text,padded_len,aes_key,aes_iv,cipher_text);

}

int fun2(char * cipher_text,int cipher_len){
	char text[cipher_len];
	aes_cbc128_decrypt(cipher_text,cipher_len,aes_key,aes_iv,text);
	
	return is_valid_pkcs7(text,cipher_len);
}


int main()
{	
	memcpy(aes_key,"1749f4b129b41e9b2033a7c467b9fe9d",2*BLOCKSIZE);
	bytestrtoascii(aes_key);

	memcpy(aes_iv ,"29ff850f0559ac21f502eed3ce236b38",2*BLOCKSIZE);
	bytestrtoascii(aes_iv);

	char * cipher_text = (char *)malloc(BUFSIZE);

	int cipher_len = fun1(cipher_text);
	int p;
	char * tmp = (char *)malloc(BUFSIZE);
	// Finding the padding 

	int init_padding;
	int always_valid = 0;
	int k;
	char real;
	tmp = cipher_text +cipher_len - 2*BLOCKSIZE;
	for(p = BLOCKSIZE-2;p >=0;p--){
		real = *(tmp+p);
		always_valid = 0;
		for(k=0;k<256;k++){
			*(tmp+p) = (char)k;
			if(!fun2(cipher_text,cipher_len))
				break;
			if(k==255)
				always_valid = 1;
		}
		*(tmp+p) = real;
		if(always_valid){
			init_padding = (BLOCKSIZE-1)-p;
			break;
		}

	}
	// padding to cipher text is init_padding
	
	printf("padding here is %d\n",init_padding);
	k = 0;
	tmp = NULL;
	int new_cipher_len = cipher_len;
	int new_padding;
	int i;
	int phi=0;
	char * text = (char *)malloc(BUFSIZE);
	// We will decrement the cipherlen BLOCK BY BLOCK
	// We will be changing the main cipher_text pointer

	char prev[BLOCKSIZE];

	for (;new_cipher_len>=BLOCKSIZE;new_cipher_len= new_cipher_len- BLOCKSIZE)
	{
		if(new_cipher_len == BLOCKSIZE){
			tmp = aes_iv;
		}
		else{
			memcpy(prev,cipher_text+new_cipher_len-2*BLOCKSIZE,BLOCKSIZE);
			tmp = cipher_text+new_cipher_len-2*BLOCKSIZE;
		}
		for(i=BLOCKSIZE-init_padding;i<BLOCKSIZE;i++){
			*(tmp + i) = (char)(*(tmp +i) ^ init_padding) ;
		}


		for(new_padding=init_padding+1;new_padding<BLOCKSIZE+1;new_padding++){
			for(i=BLOCKSIZE-(new_padding-1);i<BLOCKSIZE;i++)
				*(tmp + i) = (char)(*(tmp +i) ^ new_padding);
			
			real  = *(tmp + BLOCKSIZE-new_padding);

			for(i=0;i<256;i++){
				*(tmp + BLOCKSIZE-new_padding) = (char)i; 
				if(fun2(cipher_text,new_cipher_len)==1){
					*(text+phi) = i^(new_padding^real);
					break;	
				}
			}
					
			phi++;

			for(i=BLOCKSIZE-new_padding;i<BLOCKSIZE;i++)
				*(tmp + i) = (char)(*(tmp +i) ^ new_padding);
			
		}



		if(new_cipher_len!= BLOCKSIZE)
			memcpy(tmp,prev,BLOCKSIZE);
		
		init_padding = 0;
		*(tmp + BLOCKSIZE) = '\0';
	}

	char correct_text[phi];
	for(i=0;i<phi;i++){
		correct_text[i] = *(text+phi-i-1);
	}


	write(1,"text is:\n",9);
	write(1,correct_text,phi);
	write(1,"\n",1);

	return 0;
}