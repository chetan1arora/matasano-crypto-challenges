import sys
from Crypto import *
from Crypto.Util import *

# invmod implementation
def egcd(a,m):
	# ax + my = gcd(a,m)
	if(a == 0):
		return (m,0,1)
	g,x,y = egcd(m%a,a)
	return (g,(y-(m//a)*x),x)

def invMul(a,m):
	(g,x,_) = egcd(a,m)
	if(g != 1):
		print(a)
		print(m)
		print("numbers must be co-prime..")
		sys.exit(0)
	return x%m

# modexp implementation
def modexp(x,y,p):
	res = 1
	x = x%p
	if(x == 0):
		return 0
	if(x == 1):
		return x
	while (y > 0): 
	    if (y & 1): 
	        res = (res * x) % p
	    y = y >> 1 
	    x = (x * x) % p 
	return res

# Implementing RSA
def genKey(numBits):
	e = 3
	while(True):
		p = number.getPrime(numBits)
		q = number.getPrime(numBits)
		et = (p-1)*(q-1)
		if(et%e != 0):
			break
	modn = p*q
	pubkey = (e,modn)
	d = invMul(e,et)
	privatekey = (d,modn)
	return (pubkey,privatekey)

# Encrypt message (In Bytes)
def encrypt(message,pubkey):
	message = int(message.hex(),16)
	(e,modn) = pubkey
	enc = modexp(message,e,modn)
	enc = hex(enc).replace('0x','')
	if(len(enc)%2):
		enc = '0'+enc
	return bytes.fromhex(enc)

# Decrypt message
def decrypt(enc,privatekey):
	enc = int(enc.hex(),16)
	(d,modn) = privatekey
	mes = modexp(enc,d, modn)
	mes = hex(mes).replace('0x','')
	if(len(mes)%2):
		mes = '0'+mes
	return bytes.fromhex(mes)

# Show Buisness


# Execute only if run directly.
if __name__ == "__main__":
	print("Encrypting using RSA")
	pubk,privatek = genKey(1024)
	print("Input message",end=":")
	mes = input().encode('utf-8')
	enc = encrypt(mes,pubk)
	# showEncrypting()
	print("Encrypted string is :"+enc.hex())
	dec = decrypt(enc,privatek)
	# print("Decrypted string is "+dec.decode('utf-8'))
	print("Decrypted string is :"+dec.decode('utf-8'))



