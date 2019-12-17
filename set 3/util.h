#ifndef CRYPTO_H
#define CRYPTO_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/rand.h>


#define BUFSIZE 8096
#define BLOCKSIZE 16
const char base64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
const char hex_table[] = "0123456789abcdef";
const char english_char_freq[] = "ETAOIN SHRDLU";
char aes_key[2*BLOCKSIZE];
char aes_iv[2*BLOCKSIZE];


/*
* Convert ascii string to byte string
*/

void asciitobytestr(char * string, int str_len){
	char ascii_str[str_len];
	int p=0;
	for(;p<str_len;p++){
		*(ascii_str+p) = *(string+p);
	}

	int num;

	for(p=0;p<2*str_len;){
		num = (int)(ascii_str[p/2]);

		*(string+p) = hex_table[(num & 0xf0)>>4];
		*(string +p+1) = hex_table[num & 0x0f];

		p = p+2;
	}	

	return;
}

/*
* Convert byte string to ascii string
*/

void bytestrtoascii(char * string){

	int str_len = strlen(string);
	int p=0;
	char test[2];
	for(;p<str_len;p=p+2){
		*(test) = *(string+p);
		*(test+1) = *(string+p+1);
		string[p/2] = (char)strtoll(test,0,16);

	}
	
	*(string+(str_len/2)) = '\0';

	return;

}


/*
* Convert number to binary represention
*/

void dispBin(int num, char * rstr, int k){
char str[k];
int i=0;
for(;i<k;i++)
    strcpy(str+i,"0");
i = k-1;
while(num != 0){

if(num%2 == 1)
	memcpy(str+i,"1",1);

num = num/2;
i--;
}
strcpy(rstr,str);
return ;
}



/*
* Count set Bits (Brian Kernighan's Algorithm)
*/

int count_set_bits(int num){
	if(num == 0)
		return 0;
	else 
		return 1+ count_set_bits(num & (num -1));

}


/*
* Convert hex into binary representation
*/
void hextobin(char * hexstr,char * binstr,int k){


return;

}



/*
* Convert hex string to base64 encoded string
*/

void hextobase64(char * encoded, char * hex_string){	
	int string_len = strlen(hex_string);
	
	// Encoding defination and length
	int encoding_len = strlen(base64_table) - 1;
	
	// Padding function
	int bin_padding;
	if(string_len%6 == 4)
		bin_padding = 8;
	else if(string_len%6==2)
		bin_padding = 16;
	else
		bin_padding = 0;
	// End Padding function

	int p = 0;
	char * test = (char *)malloc(6);
	long num;
	int a,b,c,d;

	bool last = false;
	for(;p<string_len;){
		// Main function
		strncpy(test,hex_string+p,6);

		num = strtoll(test,0,16);
		if((string_len-p)<= 6){
			last = true;
			num = num << bin_padding;
		}
		
		a = (num & 0xfc0000)>>18;
		
		b = (num & 0x03f000)>>12;
		
		if(last && (bin_padding==16))
			c = encoding_len;
		else
			c = (num & 0x000fc0)>>6;

		if (last && bin_padding)
			d = encoding_len;
		else
			d = (num & 0x00003f);

		*(test) = *(base64_table+a);
		*(test+1) = *(base64_table+b);
		*(test+2) = *(base64_table+c);
		*(test+3) = *(base64_table+d);
		*(test+4) = '\0';
		
		
		strcat(encoded,test);
		// End main function

		p = p+6;
	}
	return;
}


/*
* Base64 to hex byte format
*/
void base64tohex(char * hex_string,char * encoded){
	*(hex_string) = '\0';
	int p=0;
	int str_len = strlen(encoded);
	int i;
	char test[6];
	int num;
	int padding=0;
	if(strstr(encoded,"=="))
			padding = 2;
	else if(strstr(encoded,"="))
			padding = 1;


	for(;p<str_len;){
		num =0;
		num += ((int)(strchr(base64_table,*(char *)(encoded+3)) - base64_table) & 0x3f);
		num += ((int)(strchr(base64_table,*(char *)(encoded+2)) - base64_table) & 0x3f)<<6;
		num += ((int)(strchr(base64_table,*(char *)(encoded+1)) - base64_table) & 0x3f)<<12;
		num += ((int)(strchr(base64_table,*(char *)(encoded)) - base64_table) & 0x3f)<<18;
		

		for(i=5;i>=0;i--){
			test[i] = hex_table[num & 0xf];
			num = num >> 4;
		}
		strcat(hex_string,test);
		
		encoded = (char *)(encoded+4);
		p=p+4;
	}

	str_len = strlen(hex_string);

	if(padding==2){
		*(hex_string+str_len-4) = '\0';
	}
	else if(padding==1){
		*(hex_string+str_len-2) = '\0';
	}

	return;

}



/*
* Fixed xor 
*/

void fixedxor(char * xor_str,char * s1,char * s2){
	int str_len = strlen(s1);
	if(str_len!= strlen(s2)){
		printf("Strings don't match\n");
		exit(0);
	}
	int p=0;
	int num1,num2,xor_num;
	for(;p<str_len;p++){


		// Main function
		num1 = (int)(strchr(hex_table,*(s1+p)) - hex_table);
		num2 = (int)(strchr(hex_table,*(s2+p)) - hex_table);	
		
		xor_num = num1^num2;
		*(xor_str+p) = *(hex_table+xor_num);

		// End Main function
	}
	return;
}


/*
* fixedxor_ascii(char * xor_str,char)
*/

void fixedxor_ascii(char * xor_str,char * s1,char * s2, int str_len){
	int p=0;
	int num1,num2;
	for(;p<str_len;p++){
		num1 = (int) *(s1+p);
		num2 = (int) *(s2+p);
		*(xor_str+p) = (char)(num1^num2);
	}
	return;
}












/*
* Single byte xor
*/ 

void singlebytexor(char * xor_str,char * s1,int key){
	int str_len = strlen(s1);
	long num;
	int p = 0;
	
	char test[2];
	int a,b,xor_num;
	for(;p<str_len-1;){
		*(test) = *(s1+p);
		*(test+1) = *(s1+p+1);

		num = strtoll(test,0,16);

		// Main function
		xor_num = key^((int)num);
		a = (xor_num & 0xf0) >> 4;
		b = xor_num & 0x0f;

		*(xor_str+p) = *(hex_table+a);
		*(xor_str+p+1) = *(hex_table+b);
		
		// End Main function
		p = p+2;
	}
	return;
}




/*
* Get Ordered characters with occurences
*/

void order_chr(char * ordered,char * encrypted_str,int ocr[]){
	char test[2];
	int p=0;
	int str_len = strlen(encrypted_str);
	int last=0;
	char * pointer;
	int pos;
	for(;p<str_len;){
		*(test) = *(encrypted_str+p);
		*(test+1) = *(encrypted_str+p+1);

		if(pointer = strstr(ordered,test)){
			pos =  (int)(pointer - ordered);
			if(pos%2 ==0)
				ocr[pos/2]++;
			else{
				strcat(ordered,test);
				ocr[last] = 1;
				last++;
			}
		}
		else{
			strcat(ordered,test);
			ocr[last] = 1;
			last++;
		}
	
		p = p+2;
	}
	// //Testing
	// p = 1;
	// while(p!=0){

	// 	printf("ocr %d is :%d\n",p-1,ocr[p-1]);
	// 	if(ocr[p-1]==0){
	// 		break;
	// 	}
	// 	p++;
	// }
	// printf("Ordered string is %s\n",ordered);

	// // End Testing



	// Order
	int prev = 0;
	int prev_last;
	int i;
	p=0;
	int ocr_len = last;
	int temp;
	char tmp[2];

	for(;;){
		i=prev+1;
		prev_last = prev;
		while(i<=last){
			if(ocr[i]>ocr[prev]){
				*(tmp) = *(ordered+2*prev);
				*(tmp+1) = *(ordered+2*prev+1);
				*(ordered+2*prev) = *(ordered+2*i);
				*(ordered+2*prev+1) = *(ordered+2*i+1);
				*(ordered+2*i) = *(tmp);
				*(ordered+2*i+1) = *(tmp+1);
				//printf("prev_last value is:%d and ordered :%s\n",prev_last,ordered);
				temp = ocr[prev];
				ocr[prev] = ocr[i];
				ocr[i] = temp;

				prev = i;
			}
			i++;
			}
		
		if(prev==last-1)
			break;
		else if(prev!=prev_last){
			prev = prev_last;
		}
		else
			prev++; 
	}

	// //Testing
	// p = 1;
	// while(p!=0){

	// 	printf("ocr %d is :%d\n",p-1,ocr[p-1]);
	// 	if(ocr[p-1]==0){
	// 		break;
	// 	}
	// 	p++;
	// }
	// printf("Ordered string is %s\n",ordered);

	// // End Testing

	return;
}



/*
* Scoring procedure
*/


int find_score(int ocr[],char * ordered,int key){
	char * text = (char *)malloc(BUFSIZE);
	*text = '\0';
	singlebytexor(text,ordered,key);
	int score = 0;
	int p = 0;
	int str_len = strlen(ordered);
	char chr[2];
	int num;
	// Since we have ordered we will search for only 20 letters occurences
	if(str_len > 40){
		str_len = 40;
	}

	for(;p<str_len;p=p+2){
		*(chr) = *(text + p); 
		*(chr+1) = *(text + p+1);
		num = (int)strtoll(chr,0,16);
		// if((num<30)||(num>126)){
		// 	score =score -10000;
		// 	break;
		// }
		// Take ocurrence into account
		if(!strcmp(chr,"45") || !strcmp(chr,"65"))
			score += 12*ocr[p/2];
		else if(!strcmp(chr,"54") || !strcmp(chr,"74"))
			score += 9*ocr[p/2];
		else if(!strcmp(chr,"41") || !strcmp(chr,"61"))
			score += 8*ocr[p/2];
		else if(!strcmp(chr,"54") || !strcmp(chr,"74"))
			score += 7*ocr[p/2];
		else if(!strcmp(chr,"4f") || !strcmp(chr,"6f"))
			score += 6*ocr[p/2];
		else if(!strcmp(chr,"49") || !strcmp(chr,"69"))
			score += 6*ocr[p/2];
		else if(!strcmp(chr,"4e") || !strcmp(chr,"6e"))
			score += 5*ocr[p/2];
		else if(!strcmp(chr,"53") || !strcmp(chr,"73"))
			score += 5*ocr[p/2];
		else if(!strcmp(chr,"48") || !strcmp(chr,"68"))
			score += 4*ocr[p/2];
		else if(!strcmp(chr,"52") || !strcmp(chr,"72"))
			score += 4*ocr[p/2];
		else if(!strcmp(chr,"44") || !strcmp(chr,"64"))
			score += 4*ocr[p/2];
		else if(!strcmp(chr,"4c") || !strcmp(chr,"6c"))
			score += 4*ocr[p/2];
		else if(!strcmp(chr,"55") || !strcmp(chr,"75"))
			score += 4*ocr[p/2];
		else if(!strcmp(chr,"20"))
			score += 7*ocr[p/2];
	}
	// Free before leaving
	free(text);

	return score;	
}



/*
* Character frequency analysis
*/

int freqdecrypt(char * decrypt_str,char * encrypted_str){
	
// Order the appearing letters in decresing order of their occurences
char * ordered = (char *)malloc(BUFSIZE);
*(ordered) = '\0';
int ocr[BUFSIZE] = {0};

order_chr(ordered,encrypted_str,ocr);

// Main function
int max_score = 0;
int max_score_key = 0;


// Guess keys
int len = strlen(english_char_freq);
int p;
int key;
int num1,num2;
char test[2];
int score =0;

int k;

for(p=0;p<len;p++){
	num1 = (int)english_char_freq[p];
	for(k=0;k<2*10;k=k+2){
		*(test) = *(ordered+k);
		*(test+1) = *(ordered+k+1);
		num2 = (int)strtoll(test,0,16);

		key = num1^num2;
		score = find_score(ocr,ordered,key);
		if(score > max_score){
			max_score = score;
			max_score_key = key;

		}
		// Also use the case of smaller alphabet 
		key = (num1+32)^num2;
		score = find_score(ocr,ordered,key);
		if(score > max_score){
			max_score = score;
			max_score_key = key;
		}


	}
}



printf("max score found %d with key %x \n",max_score,max_score_key);

// End Main Function


// Got key
singlebytexor(decrypt_str,encrypted_str,max_score_key);

free(ordered);

return max_score_key;

}




/*
* Repeating Key Xor
*/

void repeatkeyxor(char * xor_str,char * s1,char * repeat_key){
	
	int key_len = strlen(repeat_key)/2;
	int key[key_len];
	char test[2];
	int p = 0;
	for(;p< 2*key_len;){
		*(test) = *(repeat_key+p);
		*(test+1) = *(repeat_key+p+1);
		key[p/2] = (int)strtoll(test,0,16);
		p=p+2;
	}
	int num,xor_num,a,b;
	p = 0;
	int str_len = strlen(s1);
	
	for(;p<str_len;){
		*(test) = *(s1+p);
		*(test+1) = *(s1+p+1);
		num = (int)strtoll(test,0,16);
		xor_num = num^key[(p/2)%3];
		
		a = (xor_num & 0xf0) >> 4;
		b = xor_num & 0x0f;

		*(xor_str+p) = *(hex_table+a);
		*(xor_str+p+1) = *(hex_table+b);
		
		p=p+2;
	}

	*(xor_str+str_len) = '\0';
	return;
}

/*
* Hamming distance between two byte strings
*/


int hamming_dist(char * s1,char * s2){
	int str_len = strlen(s1);
	if(str_len != strlen(s2)){
		printf("Requires equal size strings for hamming distance.\n");
		exit(0);
	}

	int padding;

	if(str_len%6 == 2)
		padding = 4;
	else if(str_len%6 == 4)
		padding = 2;
	else
		padding = 0;

	str_len = str_len+padding;
	char xor_str[str_len];

	fixedxor(xor_str,s1,s2);

	while(padding!=0){
		*(xor_str+str_len-padding) = '0';
		padding--;
	}
	int p;
	int num=0;
	int hamm_dist = 0;
	char test[6];

	for(p=0; p<str_len; p =p+6){
		strncpy(test,xor_str+p,6);
		*(test+6) = '\0';
		num = (int)strtoll(test,0,16);
		hamm_dist += count_set_bits(num);
	}
	return hamm_dist;

}

/*
* Break repeat key xor
*/
void break_repkey_xor(char * keystr,char * byte_str){

	float hamm_dist=0;
	float n_hamm_dist = 0;

	float minn_hamm_dist = 100.0;
	int minn_hamm_dist_keysize = -1;
	// Taking the first 3 keysize blocks
	int keysize = 2;
	char * block1 = (char *)malloc(128);
	char * block2 = (char *)malloc(128);
	char * block3 = (char *)malloc(128);
	char * block4 = (char *)malloc(128);	

	int testing =0;
	

	if(testing){

		printf("byte str is %s\n",byte_str);
	//	testing =0;

	}


	// Keysize is in ascii size
	for(;keysize<40;keysize++){
		strncpy(block1,byte_str,2*keysize);
		*(block1+2*keysize) ='\0';
		strncpy(block2,byte_str+2*keysize,2*keysize);
		*(block2+2*keysize) ='\0';
		strncpy(block3,byte_str+4*keysize,2*keysize);
		*(block3+2*keysize) ='\0';
		strncpy(block4,byte_str+6*keysize,2*keysize);
		*(block4+2*keysize) ='\0';


		// Testing
		if(testing && keysize==10){

		printf("testing block1:%s\n",block1);
		printf("testing block2:%s\n",block2);
		printf("testing block3:%s\n",block3);
		printf("testing block4:%s\n",block4);
		testing = 0;
		}
		// End Testing
		hamm_dist = 0.0;
		hamm_dist +=  (float)hamming_dist(block1,block2);
		hamm_dist += (float) hamming_dist(block2,block3);
		hamm_dist += (float) hamming_dist(block3,block4);
		
		n_hamm_dist = (float)hamm_dist/(3*keysize);

		printf("hamming distance with keysize %d is %f\n",keysize,n_hamm_dist);

		if(n_hamm_dist < minn_hamm_dist){
			minn_hamm_dist = n_hamm_dist;
			minn_hamm_dist_keysize = keysize;
		}

	}


	// Making a list of decreasing edit distance







	printf("Minimum hamm_dist keysize is %d\n",minn_hamm_dist_keysize);


	keysize = 2*29;
	// Taking keysize to be length of byte key

	char enc_block[BUFSIZE];
	char * temp = (char *)malloc(BUFSIZE);

	char key[keysize];
	int p=0;
	int q=0;
	int keya=0;
	int str_len = strlen(byte_str);
	int i=0;
	for(;q<keysize;q=q+2){
		i=0;
		for(p=q;p<str_len;p=p+keysize){
			enc_block[i] = *(byte_str+p);
			enc_block[i+1] = *(byte_str+p+1);
			i = i+2;
		}
		printf("enc_block is: %s\n",enc_block);
		keya = freqdecrypt(temp,enc_block);

		key[q] = hex_table[(keya & 0xf0)>>4];
		key[q+1] = hex_table[keya & 0xf];
		printf("key till here is %s\n",key);
	}


	// Got the key
	strncpy(keystr,key,keysize);
	*(keystr+keysize) = '\0';
	free(temp);

	return;
}


/*
 * Handle Cryptographic errors
 */

void handleErrors(void)
{
  ERR_print_errors_fp(stderr);
  abort();
 }






// void openssl_lib(unsigned char * lol){

//   ERR_load_crypto_strings();
//   OpenSSL_add_all_algorithms();
//   OPENSSL_config(NULL);

//   // We will be using Envelope mode( High level api)
  
//   err:
//   ERR_print_errors_fp(stderr);

//   EVP_cleanup();
//   CRYPTO_cleanup_all_ex_data();
//   ERR_free_strings();




// }


// /*
// * Encryption using aes ecb 128 bit key
// */



 int aes_ecb128_encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
  unsigned char *iv, unsigned char *ciphertext)
{
  EVP_CIPHER_CTX *ctx;

  int len;
  int ciphertext_len;

  
  if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

  if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, key, iv))
    handleErrors();

  /* Provide the message to be encrypted, and obtain the encrypted output.
   * EVP_EncryptUpdate can be called multiple times if necessary
   */
  if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
    handleErrors();
  ciphertext_len = len;

  //if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) handleErrors();
  //ciphertext_len += len;

  EVP_CIPHER_CTX_free(ctx);
  
  return ciphertext_len;
}
 



// /*
// * Encryption using aes cbc 128 bit key
// */



 int aes_cbc128_encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
  unsigned char *iv, unsigned char *ciphertext)
{
  EVP_CIPHER_CTX *ctx;

  int len;
  int ciphertext_len;

  
  if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

  if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv))
    handleErrors();

  /* Provide the message to be encrypted, and obtain the encrypted output.
   * EVP_EncryptUpdate can be called multiple times if necessary
   */
  if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
    handleErrors();
  ciphertext_len = len;

  //if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) handleErrors();
  //ciphertext_len += len;

  EVP_CIPHER_CTX_free(ctx);
  
  return ciphertext_len;
}


/*
* Decryption using aes ecb 128 bit key
*/
int aes_ecb128_decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,unsigned char *iv, unsigned char *plaintext)
{
  EVP_CIPHER_CTX *ctx;

  int len;

  int plaintext_len;

  /* Create and initialise the context */
  if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

  /* Initialise the decryption operation. IMPORTANT - ensure you use a key
   * and IV size appropriate for your cipher
   * In this example we are using 256 bit AES (i.e. a 256 bit key). The
   * IV size for *most* modes is the same as the block size. For AES this
   * is 128 bits */
  if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, key, iv))
    handleErrors();

  /* Provide the message to be decrypted, and obtain the plaintext output.
   * EVP_DecryptUpdate can be called multiple times if necessary
   */

  printf("cipher text is %s\n",ciphertext);

  if(1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
    handleErrors();

  plaintext_len = len;
  
   /* Finalise the decryption. Further plaintext bytes may be written at
   * this stage.
   */
  //if(1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len)) handleErrors();
  //plaintext_len += len;

  /* Clean up */
  EVP_CIPHER_CTX_free(ctx);

  return plaintext_len;
}



/*
* Decryption using aes cbc 128 bit key
*/

int aes_cbc128_decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,unsigned char *iv, unsigned char *plaintext)
{
  EVP_CIPHER_CTX *ctx;

  int len;

  int plaintext_len;

  /* Create and initialise the context */
  if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

  /* Initialise the decryption operation. IMPORTANT - ensure you use a key
   * and IV size appropriate for your cipher
   * In this example we are using 256 bit AES (i.e. a 256 bit key). The
   * IV size for *most* modes is the same as the block size. For AES this
   * is 128 bits */
  if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv))
    handleErrors();

  /* Provide the message to be decrypted, and obtain the plaintext output.
   * EVP_DecryptUpdate can be called multiple times if necessary
   */

  //printf("cipher text is %s\n",ciphertext);

  if(1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
    handleErrors();

  plaintext_len = len;
  
   /* Finalise the decryption. Further plaintext bytes may be written at
   * this stage.
   */
  //if(1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len)) handleErrors();
  //plaintext_len += len;

  /* Clean up */
  EVP_CIPHER_CTX_free(ctx);

  return plaintext_len;
}




/*
* Detect acb just same block after encryption
*/


int detect_ecb(char * ciphertext)
{
	int blocks = strlen(ciphertext)/32;
	int p;
	int q;
	char * block = (char *)malloc(32);
	char * block2 = (char *)malloc(32);
	for(p=0;p<blocks;p++){
		strncpy(block,ciphertext+p*32,32);
		*(block+32) = '\0';
		for(q=p+1;q<blocks;q++){
			strncpy(block2,ciphertext+q*32,32);
			*(block2+32) = '\0';

			if(!strcmp(block,block2))
				return 1;
			}
		}

	return 0;
	} 

/*
* Text in ascii, PKCS #7 padding
*/


int pkcs7padding(char * text,int text_len,int blocksize){
	int pad_num = blocksize - text_len%blocksize;

	int p=text_len;
	for(;p<text_len+pad_num;p++){
		*(text+p) = (char)pad_num;
	}
	*(text+pad_num+text_len) = '\0';

	return text_len+pad_num;

}





/*
* Return ciphertext_length
*/


int oracle_encrypt(char * ciphertext, char * plaintext,int plaintext_len){
	char iv[BLOCKSIZE];
	int p=0;
	
	// char aes_key[BLOCKSIZE];
	// int kb = RAND_bytes(aes_key,sizeof(aes_key));
	// if(kb != 1){
	// 	printf("aes key generation failed\n");
	// 	exit(0);
	// }


	int ivb = RAND_bytes(iv,sizeof(iv)); 
	if(ivb != 1){
		printf("iv generation failed\n");
		exit(0);
	}

	/* Question 5

	// Extra plain text constant in the whole encryption
	char extconst[BUFSIZE];
	char byteextconst[BUFSIZE];
	int fd = open("extconst.txt",O_RDONLY);
	if(fd<0){
		printf("opening file failed\n");
		exit(0);
	}
	if((p=read(fd,extconst,BUFSIZE))<0){
		printf("Error in reading\n");
		exit(0);
	}
	close(fd);

 	int k = 0;
	int j;
	while(k< BUFSIZE){
		if(*(extconst+k) == '\n'){
		    for(j=k;j<BUFSIZE-1;j++)
				*(extconst + j) = *(extconst+j+1);
		}
    k++;
	}
	k = 0;
	j=0;

	// Base64 to hex to ascii
	base64tohex(byteextconst,extconst);
	p = strlen(byteextconst)/2;
	bytestrtoascii(byteextconst);
	

	memcpy(plaintext+plaintext_len,byteextconst,p);
	plaintext_len =plaintext_len+p;
	p = 0;

	*/

	// Encryption Starts

	int ipadding = 7;//(int)rand()%16;
	//printf("ipadding is %d\n",ipadding);
	int len = ipadding+plaintext_len;
	int lpadding = BLOCKSIZE - len%BLOCKSIZE;
	len += lpadding;
	
	char ipad[ipadding];

	p = RAND_bytes(ipad,sizeof(ipad));
	if(p != 1){
		printf("padding generation failed\n");
		exit(0);
	}
	p = 0;

	char lpad[lpadding];
	p = RAND_bytes(lpad,sizeof(lpad));
	if(p != 1){
		printf("padding generation failed\n");
		exit(0);
	}
	p=0;


	char ppl[len];
	memcpy(ppl,ipad,ipadding);
	memcpy(ppl+ipadding,plaintext,plaintext_len);
	memcpy(ppl+ipadding+plaintext_len,lpad,lpadding);

	//*(ppl+len) = '\0';
	
	int op = 0;//(int)rand()%2;
	//printf("op is %d\n",op);

	//char plain_block[BLOCKSIZE] = (char *)malloc(BLOCKSIZE);
	// Aes ecb or cbc
	for(p=0;p<len;p=p+BLOCKSIZE){

		if(op==1){
			fixedxor_ascii(ppl+p,ppl+p,iv,BLOCKSIZE);
		}
		aes_ecb128_encrypt(ppl+p,BLOCKSIZE,aes_key,iv,ciphertext+p);

		if(op==1){
			memcpy(iv,ciphertext+p,BLOCKSIZE);
		}
		//write(1,ciphertext+p,BLOCKSIZE);
	}

	return len;

}



int is_valid_pkcs7(char * padded_text,int padded_len){

	if(padded_len%BLOCKSIZE != 0)
		return 0;
	int last = (int)*(padded_text+padded_len-1);
	// if((int)*(padded_text+padded_len-1-last)<16){
	// 	return 0;
	// }
	int k=0;
	while((int)*(padded_text+padded_len-1-k) == last){
		k++;
		if(k==last){
			return 1;
		}
	}

	return 0;
}

/*
* Aes ctr mode encryption routine
*/

int aes_128_ctr_encrypt(char *key, char *nonce, char *data, int data_len){
	EVP_CIPHER_CTX *ctx;
	int j, len, left = data_len;
	int i;
	char *pos = data;
	char counter[BLOCKSIZE], keystream[BLOCKSIZE];
	int temp;
	/* Create and initialise the context */
  if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();


  if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_ctr(), NULL, key, NULL))
    handleErrors();

	memcpy(counter, nonce, BLOCKSIZE);

	while (left > 0) {
		if(1 != EVP_EncryptUpdate(ctx, keystream,&temp, counter,BLOCKSIZE))
    	handleErrors();

		len = (left < BLOCKSIZE) ? left : BLOCKSIZE;
		for (j = 0; j < len; j++)
			pos[j] ^= keystream[j];
		pos += len;
		left -= len;

		for (i = BLOCKSIZE - 1; i >= 0; i--) {
			counter[i]++;
			if (counter[i])
				break;
		}
	}
	EVP_CIPHER_CTX_free(ctx);
	return 0;
}


int aes_128_ctr_decrypt(char *key, char *nonce, char *data, int data_len){
	EVP_CIPHER_CTX *ctx;
	int j, len, left = data_len;
	int i;
	char *pos = data;
	char counter[BLOCKSIZE], keystream[BLOCKSIZE];
	int temp;
	/* Create and initialise the context */
  if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();


	memcpy(counter, nonce, BLOCKSIZE);

	if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, key, NULL))
    	handleErrors();

	while (left > 0) {
		

		if(1 != EVP_EncryptUpdate(ctx, keystream,&temp, counter,BLOCKSIZE))
    	handleErrors();

		len = (left < BLOCKSIZE) ? left : BLOCKSIZE;
		for (j = 0; j < len; j++)
			pos[j] ^= keystream[j];
		pos += len;
		left -= len;

		for (i = 8; i < BLOCKSIZE; i++) {
			counter[i]++;
			if (counter[i])
				break;
		}
		printf("counter should be incrementing\n");
		write(1,counter+8,1);
		write(1,"\n",1);

	}
	EVP_CIPHER_CTX_free(ctx);
	return 1;
}

#endif
