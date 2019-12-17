#include "util.h"

int main(){

printf("Provide string s1\n");
char * s1 = (char *)malloc(BUFSIZE);
read(0,s1,BUFSIZE);
int str_len = strlen(s1);
if(*(s1+str_len-1)=='\n'){
	*(s1+str_len-1) = '\0';
}
asciitobytestr(s1);


printf("Provide string s2\n");
char * s2 = (char *)malloc(BUFSIZE);
read(0,s2,BUFSIZE);
str_len = strlen(s2);
if(*(s2+str_len-1)=='\n'){
	*(s2+str_len-1) = '\0';
}
asciitobytestr(s2);

int hamming_distance = hamming_dist(s1,s2);

printf("Hamming distance :%d\n",hamming_distance);

}
