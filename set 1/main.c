#include "util.h"


int main(){
int num;
scanf("%d",&num);
printf("\n");
char * binstr = (char *)malloc(32);
// fUNCTION for display binary
dispBin(num,binstr,32);

printf("The number %d in binary is :%s\n",num,binstr);

return 0;

}
