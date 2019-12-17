#include "util.h"


int main(){
	// hex encoded strings
	char * line = (char *)malloc(BUFSIZE) ;
	int n;
	size_t size;
	int ecb =0;

	FILE *fp = fopen("8.txt","r");

	while((n = getline(&line,&size,fp)) != -1){

	*(line+n-1) = '\0';
	//printf("line is %s\n",(char *)line);

	ecb = detect_ecb(line);

	if(ecb == 1)
		printf("ecb encryption used with %s\n",line);
	}
	

	return 0;

}
