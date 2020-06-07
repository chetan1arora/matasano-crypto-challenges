import sys
from Crypto import *
from Crypto.Util import *
import math
import time

# Invmod implementation
def invMul(a,m):
	stack = []
	save_m = m
	while(a != 0):
		stack.append(m//a)
		a,m = m%a,a
	if(m != 1):
		print("numbers should be coprime to have a inverse")
		sys.exit(0)
	x,y = 0,1
	for v in stack[::-1]:
		x,y = y-v*x, x
	return x%save_m

#GCD stack-based implementation
def gcd(a,b):
	while(a != 0):
		a,b = b%a,a
	return b

#Optimized inverse Power function (For bignums)
def invPow(x,n):
    ###Finds the integer component of the n'th root of x, an integer such that y ** n <= x < (y + 1) ** n.
    high = 1
    while high ** n < x:
        high *= 2
    low = high//2
    while low < high:
        mid = (low + high) // 2
        if low < mid and mid**n < x:
            low = mid
        elif high > mid and mid**n > x:
            high = mid
        else:
            return mid
    return mid+1

# Modexp implementation
def modExp(x,y,p):
	res = 1
	x = x%p
	if(x//2 == 0):
		return x%2
	while (y > 0): 
	    if (y & 1): 
	        res = (res * x) % p
	    y = y >> 1 
	    x = (x * x) % p 
	return res

# Key Generation
def genKey(numBits,e=3):
	while(True):
		p = number.getPrime(numBits)
		q = number.getPrime(numBits)
		et = (p-1)*(q-1)
		if(et%e != 0 and p != q):
			break
	modn = p*q
	pubkey = (e,modn)
	if p>q:
		privatekey = (e,p,q)
	else:
		privatekey = (e,q,p)
	return (pubkey,privatekey)

# Encryption
def encrypt(message,pubkey):
	message = int(message.hex(),16)
	(e,modn) = pubkey
	enc = modExp(message,e,modn)
	enc = hex(enc).replace('0x','')
	if(len(enc)%2):
		enc = '0'+enc
	return bytes.fromhex(enc)

# Decryption
def decrypt(enc,privatekey,optim=0):
	enc = int(enc.hex(),16)
	(e,p,q) = privatekey
	d = invMul(e,(p-1)*(q-1))
	if optim:
		# Adding CRT optimization for fast decryption
		ep = modExp(enc,d%(p-1),p)
		temp = modExp(enc,d%(q-1),q)- ep
		if(temp < 0):
			temp += q*math.ceil(-temp/q)
		div = p-int(p/q)*q
		# Think of div*x = temp + k*q with x and q being integers
		com = gcd(temp,div)
		div //= com
		temp //= com
		sol = (invMul(div,q)*temp)%q
		# Taking the solution
		mes = ep + p*sol
	else:
		mes = modExp(enc,d,p*q)
	mes = hex(mes).replace('0x','')
	if(len(mes)%2):
		mes = '0'+mes
	return bytes.fromhex(mes)

# Show Buisness



# Execute only if run directly.
if __name__ == "__main__":
	print("Encrypting using RSA")
	pubk,privatek = genKey(1024,3)
	print("Input message",end=":")
	mes = input().encode('utf-8')
	enc = encrypt(mes,pubk)
	print("Encrypted string is :"+enc.hex())
	st_time = time.time()
	dec = decrypt(enc,privatek)
	e_time = time.time()-st_time
	print("[-]Time taken: "+str(e_time))
	st_time = time.time()
	dec = decrypt(enc,privatek,1)
	ebar_time = time.time()-st_time
	print("[+]Time taken after optimization: "+str(ebar_time))
	print("[*]Speedup achieved: "+str(100*((e_time/ebar_time)-1))+"%")

	print("[+]Decrypted string is :"+dec.decode('utf-8'))