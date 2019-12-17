#include <stdio.h>
#include <stdlib.h>
#include <string.h>
unsigned int lower_mask,upper_mask;
int in_dex;
unsigned int w, n, m, r, u, d, s, t, l,f;
unsigned long int a,b,c;

// Generate the next n values from the series x_i 
int twist(unsigned int * MT) {
     int i;
     int xA,x;
	for(i=0;i<n;i++){
         x = (MT[i] & upper_mask)+(MT[(i+1) % n] & lower_mask);
         xA = x >> 1;
         if(x%2 != 0) { // lowest bit of x is 1
             xA = xA ^ a;
         }
         MT[i] = MT[(i + m) % n] ^ xA;
     }
     in_dex = 0;
     return 1;
 }

 // Initialize the generator from a seed
int seed_mt(int seed,unsigned int * MT) {
     in_dex = n;
     MT[0] = seed;
     int i;

     for(i=1;i<n;i++){ // loop over each element
     	MT[i] = (unsigned int)(f * (MT[i-1]^(MT[i-1] >> (w-2))) + i);
     }
     return 1;
 }
 
 // Extract a tempered value based on MT[in_dex]
 // calling twist() every n numbers
 int extract_number(unsigned int * MT) {
     if(in_dex >= n){
         if(in_dex > n){
           printf(" Generator was never seeded\n");
           exit(0);
           // Alternatively, seed with constant value; 5489 is used in reference C code[46]
         }
        printf("twist returns :%d\n",twist(MT));
     }
 
     int y = MT[in_dex];
     y = y ^ ((y >> u) & d);
     y = y ^ ((y << s) & b);
     y = y ^ ((y << t) & c);
     y = y ^ (y >> l);
 
     in_dex++;
     return (unsigned int)y;
 }
 

int main(int argc, char const *argv[])
{
int seed;
w = 32; // 32 bit prng
n=624;
m =  397;
r = 31;
a = strtoll("9908B0DF16",0,16);
u = 11;
d = strtoll("FFFFFFFF16",0,16);
s= 7;
b = strtoll("9D2C568016",0,16);
t = 15;
c = strtoll("EFC6000016",0,16);
t = 15;
l = 18;
f = 1812433253;

if(argc < 2)
	seed = 1;
else
	seed = atoi(argv[1]);

unsigned int * MT =(unsigned int *)malloc(n*sizeof(int)); 
in_dex = n+1;

lower_mask = (1 << r) - 1 ;
upper_mask = (unsigned int)(~lower_mask);

seed_mt(seed,MT);
srand(seed);
int i;
int num;

for(i=0;i<100;i++){
	num = extract_number(MT);
	//printf("number is :%d\n",rand(),num);
}


return 0;
}
