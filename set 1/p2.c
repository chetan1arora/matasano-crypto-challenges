#include "util.h"


int main(int argc, char const *argv[])
{
	printf("Problem 2\n");
	char * s1 = (char *)malloc(BUFSIZE);
	char * s2 = (char *)malloc(BUFSIZE);
	char * xor_str = (char *)malloc(BUFSIZE);
	int n;
	printf("Give string s1\n");
	if( (n=read(0,s1,BUFSIZE)) < 0){
		write(1,"Give valid argument.\n",BUFSIZE);
		exit(0);
	}
	printf("Give string s2\n");
	if( (n=read(0,s2,BUFSIZE)) < 0){
		write(1,"Give valid argument.\n",BUFSIZE);
		exit(0);
	}
	char *p;
	if(p = strstr(s1,"\n")){
	    *p = '\0';
	}

	if(p = strstr(s2,"\n")){
	    *p = '\0';
	}
	
	fixedxor(xor_str,s1,s2);

	printf("xor'ed string is:\n");
	printf("%s\n",xor_str);
	
	return 0;
}