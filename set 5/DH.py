# Diffie Hellman Implemented

import random, time
random.seed(time.time())
import sys

# DH parameters
f = open('p','r').read().replace('\n','')
p = int(f,16)
g = 2

# helper

def isPrime(a):
	for i in range(2,a):
		if(a%i == 0):
			return True
		b = i*i
		if(b > a):
			break

	return False


def op(a,p):
	return a

def pow(x,y,p):
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

# DH private keys
print("[*]Generating Private keys")

a = random.randint(0,p-1)
# print(a)
b = random.randint(0,p-1)
# print(b)
# DH public keys

print("[*]Generating Public keys")
A = pow(g,a,p)
print("A",end=':')
print(format(A,'x'))

print("B",end=':')
B = pow(g,b,p)
print(format(B,'x'))

# Session keys
print("[*]Generating Session Keys")

SessionKeyForA = pow(B,a,p)

print("Session Key For A",end=":")
print(format(SessionKeyForA,'x'))

SessionKeyForB = pow(A,b,p)

print("Session Key for B",end=':')
print(format(SessionKeyForB,'x'))