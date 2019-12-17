#include "util.h"


int main(int argc, char const *argv[])
{
	printf("Problem 5\n");
	char * s1 = (char *)malloc(BUFSIZE);
	int n;
	printf("Taking key from repeat_encrypt\n");
	int fd = open("repeat_encrypt",O_RDONLY);
	if( (n=read(fd,s1,BUFSIZE)) < 0){
		write(1,"Nothing in the file.\n",BUFSIZE);
		exit(0);
	}

	int str_len = strlen(s1);
	if(*(s1+str_len-1) == '\n')
		*(s1+str_len-1) = '\0';


	//asciitobytestr(s1);


	char * key = (char *)malloc(BUFSIZE);
	char * xor_str = (char *)malloc(BUFSIZE);

	printf("\nProvide the key \n");
	//printf("If you want to give key in ascii please press enter\n");	
	if((n=read(0,key,BUFSIZE)) < 0){
		write(1,"Give valid argument.\n",BUFSIZE);
		exit(0);
	}

	str_len = strlen(key);
	if(*(key+str_len-1) == '\n')
		*(key+str_len-1) = '\0';

	printf("Key taken is %s\n",key);

	asciitobytestr(key);
	
	printf("string is %s\n",s1);
	printf("key is %s\n",key);
	
	
	repeatkeyxor(xor_str,s1,key);

	printf("xor'ed string is:\n");
	printf("%s\n",xor_str);
	
	return 0;
}

