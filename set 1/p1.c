#include "util.h"


int main(int argc, char const *argv[])
{
	char * hex_str = (char *)malloc(BUFSIZE);
	char * encoded_str = (char *)malloc(BUFSIZE);
	int n;
	printf("Problem 1\n");
	char *p;
	int action;	
	printf("Press 0 for decoding base64 or 1 to encode to base64\n");
	scanf("%d",&action);

	if(!((action==0)||(action==1))){
		printf("No action selected.\n");
		printf("Exiting Now.\n");
		sleep(2);
		exit(0);
			
	}

	if(!action)
		goto Decode;

	Encode: printf("Give string to encode in base64\n");
			if( (n=read(0,hex_str,BUFSIZE)) < 0){
				write(1,"Give valid argument.\n",BUFSIZE);
			}
		    
			if(p = strstr(hex_str,"\n")){
			    *p = '\0';
			}

			hextobase64(encoded_str,hex_str);

			printf("Encoded string is:\n");
			printf("%s\n",encoded_str);
			
			printf("Exiting Now.\n");
			sleep(2);
			exit(0);



	Decode: printf("Give string to decode in base64\n");
			if( (n=read(0,encoded_str,BUFSIZE)) < 0){
				write(1,"Give valid argument.\n",BUFSIZE);
			}

			if(p = strstr(encoded_str ,"\n")){
			    *p = '\0';
			}	

			base64tohex(hex_str,encoded_str);

			printf("Decoded string is:\n");
			printf("%s\n",hex_str);
			
			printf("Exiting Now.\n");
			sleep(2);
			exit(0);

	return 0;
}
