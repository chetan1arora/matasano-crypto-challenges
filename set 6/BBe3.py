# BB e=3 attack

# Message Signing using Pubkey/PrivateKey RSA
# PKCS v1.5 used with mode 01h

from RSA import *
import random
from multiprocessing import Pool,Queue
# We always use byte stream forany data input
def pkcsv15pad(plainText, blockLen):
	# Mode 1 :
	# 00 | M | Pad | 00 | D
	out = bytearray(blockLen)
	out[1] = 1
	for i in range(2,blockLen-len(plainText)-1):
		out[i] = 255
	for i in range(len(plainText)):
		out[-1-i] = plainText[-1-i]
	return out


# Signature decryption and verify by search for PKCS1.5 paddad message pattern
def verifySignature(plainText,signature,pubkey,blockLen):
	signature = int(signature.hex(), 16)
	(e,modn) = pubkey
	# Decrypt Signature
	dec = modExp(signature,e,modn)
	# Convert signature to blocks
	dec = hex(dec).replace('0x','')
	if(len(dec)%2):
		dec = '0'+dec
	dec = '00'+dec
	messageBlock = bytes.fromhex(dec)

	# Verifying decrypted signature for pkcs1.5 padded plainText
	if len(messageBlock) != blockLen:
		return False
	# Starts with 00h 01h ffh
	stByte = [0,1,255]
	for i in range(len(stByte)):
		if messageBlock[i] != stByte[i]:
			return False

	# Should contain with plainTexts
	found = 0
	msgLen = len(plainText)
	for x in range(blockLen):
		found = 1 
		for y in range(msgLen):
			if messageBlock[x+y] != plainText[y]:
				found = 0
				break
		if found:
			return True
	return False

# Forging a new signature using the vuln that signature is not complete
# Attack had been successful on Firefox TLS certificate signing
def forgeSignature(plainText,pubkey,blockLen):
	(e,modn) = pubkey
	# Starts with 00h 01h ffh
	stByte = [0,1,255]
	k = 0
	for i in range(len(stByte)):
		k = k<<8
		k = k|stByte[i]

	for i in range(len(plainText)):
		k = k<<8
		k = k|plainText[i]

	msgSize = len(plainText)*8	
	k = k<<((blockLen*8)-24-msgSize)

	tmp = invPow(k,e)
	kDash = modExp(tmp,e,modn)
	if kDash < k:
		kDash = modExp(tmp+1,e,modn)

	k = kDash

	if k > modn:
		print("Won't be able to forge signature")
		return 0
	forgedSign = invPow(k,e)
	forgedSign = hex(forgedSign).replace('0x','')
	if(len(forgedSign)%2):
		forgedSign = '0'+forgedSign

	return bytes.fromhex(forgedSign)

def sign(paddedBlock, privatekey):
	return decrypt(paddedBlock,privatekey,1)


if __name__=="__main__":
	specs = 1024 # bit
	print("RSA message signing")
	(pubk,privatek) = genKey(specs//2)
	print("Input message",end=":")
	mes = input().encode('utf-8')
	messageBlock = pkcsv15pad(mes,specs//8)
	signature = sign(messageBlock, privatek)
	print("Signature:"+signature.hex())
	print("Is signature valid:"+str(verifySignature(mes,signature,pubk,specs//8)))
	print("Deleting the private key...")
	privatek = 0
	messageBlock = 0
	print("Forging a new signature for the same ")
	forgedSign = forgeSignature(mes,pubk,specs//8)
	print("New Signature:"+forgedSign.hex())
	print("Is new forged valid :"+str(verifySignature(mes, forgedSign, pubk, specs//8)))
