#include "util.h"


struct profile
{
	char email[150];
	int uid;
	char role[5];
};

void display_user(struct profile user){

// Structure is
printf("Structure is :\n");
printf("email : %s\n",user.email);
printf("uid : %d\n",user.uid);
printf("role: %s\n",user.role);
printf("\n");

}



struct profile profile_for(char * email){

	char * p;
	// Sanitize & and =
	while(p = strchr(email,'=')){
		*p = '\0';
	}
	while(p = strchr(email,'&')){
		*p = '\0';
	}
	
	struct profile user;
	strcpy(user.email,email);
	user.uid = 10;
	strcpy(user.role,"user");

	return user;
}


void url_encode(struct profile user,char * url_encoded){
	char tmp[BUFSIZE];
	int url_len = 0;
	int temp;
	char diu[5];
	char uid[5];

	strcpy(tmp,"email");
	*(tmp+5) = '=';
	strcpy(tmp+6,user.email);
	int str_len = strlen(tmp);
	*(tmp+str_len) = '&';
	memcpy(url_encoded,tmp,str_len+1);
	url_len = str_len+1;

	strcpy(tmp,"uid");
	*(tmp+3) = '=';
	// itoa implement
	temp = user.uid;
	int i=0;
	while(1){
		diu[i] = (char)(48+temp%10);
		i++;
		temp = temp /10;
		if(temp == 0){
			break;
		}
	}
	int k;
	
	for(k=0;k<i;k++){
		uid[k] = diu[i-1-k];
	}
	
	//
	strcpy(tmp+4,uid);
	str_len = strlen(tmp);
	*(tmp+str_len) = '&';
	memcpy(url_encoded+url_len,tmp,str_len+1);
	url_len += str_len+1;

	strcpy(tmp,"role");
	*(tmp+4) = '=';
	strcpy(tmp+5,user.role);
	str_len = strlen(tmp);
	memcpy(url_encoded+url_len,tmp,str_len);
	url_len += str_len;

	*(url_encoded+url_len) = '\0';

	return;
}





int main()
{	
	int n;
	memcpy(aes_key,"1749f4b129b41e9b2033a7c467b9fe9d",2*BLOCKSIZE);

	bytestrtoascii(aes_key);

	char * input = (char *)malloc(BUFSIZE);
	n = 0;
	if((n = read(0,input,BUFSIZE)) < 0){

		printf("Give validated input\n");
		exit(0);
	}
	// Remove newlines from the input
	int k = 0;
	int j;
	while(k< BUFSIZE){
		if(*(input+k) == '\n'){
    for(j=k;j<BUFSIZE-1;j++)
				*(input + j) = *(input+j+1);
		}
    k++;
	}

	struct profile user	= profile_for(input);

	char * url_encoded = (char *)malloc(BUFSIZE);
	char * cipher_text = (char *)malloc(BUFSIZE);
	char * plain_text = (char *)malloc(BUFSIZE);
	display_user(user);

	url_encode(user,url_encoded);

	printf("url_encoded is %s\n",url_encoded);


	// Encryption Routine
	int str_len = strlen(url_encoded);
	int cipher_len = oracle_encrypt(cipher_text,url_encoded,str_len);

	// Print and change

	printf("Encryption text is %d long:\n",cipher_len);
	
	char tmp[BLOCKSIZE];
	int p=0;
	for(;p<cipher_len;p=p+BLOCKSIZE){
		if(p==16){
			memcpy(tmp,cipher_text+p,BLOCKSIZE);
		}
		if(p==48){
			memcpy(cipher_text+p,tmp,BLOCKSIZE);
		}

		write(1,cipher_text+p,BLOCKSIZE);
		
		printf("\n");
	}

	// Decryption Routine 
	aes_ecb128_decrypt(cipher_text,cipher_len,aes_key,0,plain_text);

	printf("Decrypted string is :\n");
	write(1,plain_text,cipher_len);

	printf("\n");

	printf("We can parse it later\n");

	return 0;
}



