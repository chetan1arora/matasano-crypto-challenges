#include "util.h"


int main()
{
	char * encrypted_str = (char *)malloc(BUFSIZE);
	int n;
	printf("Problem 3\n");
	printf("Give string to decrypt using character frequency\n");
	if( (n=read(0,encrypted_str,BUFSIZE)) < 0){
		write(1,"Give valid argument.\n",BUFSIZE);
	}
    char *p;
	if(p = strstr(encrypted_str,"\n")){
	    *p = '\0';
	}	

	char * decrypt_str = (char *)malloc(BUFSIZE);
	int key = freqdecrypt(decrypt_str,encrypted_str);
	
	printf("String decrypted with key:%x",key);
	printf("\n");
	printf("%s\n",decrypt_str);

	return 0;
}
