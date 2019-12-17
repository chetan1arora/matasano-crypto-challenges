#include "util.h"

int main()
{
	char * text = (char *)malloc(BUFSIZE);
	int n;
	
	if((n=read(0,text,BUFSIZE)) < 0){
		printf("Give some argument\n");
		printf("Exiting now\n");
		sleep(3);
		exit(0);
	}
	
	if(*(text+n-1)=='\n'){
		*(text+n-1) = '\0';
		n = n-1;
		printf("text is %s\n",text);
	}


	int text_len = n;
	
	text_len = pkcs7padding(text,text_len,BLOCKSIZE);
	

	

	printf("Padded string is.\n");

	write(1,text,text_len);

	printf("\n");
	asciitobytestr(text,text_len);

	write(1,text,2*text_len);
		return 0;
}