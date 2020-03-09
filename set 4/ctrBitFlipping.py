from Crypto.Cipher import AES
from Crypto import Random
from pwn import *

nonce = Random.new().read(8)

keystream = []

payload = bytes('A'*16,'utf-8')
s1 = b'comment1=cooking%20MCs;userdata='
s2 = b';comment2= like a pound of bacon'

plainText = s1 + payload + s2

for i in range(int(len(plainText)/16)):
	key = nonce + pack(i,8*8,'little')
	obj = AES.new(key,AES.MODE_ECB)
	keystream.append(obj.encrypt(plainText[i*16:(i+1)*16]))

print(keystream)

cipherText = []

for i in range(len(keystream)):
	for j in range(16):
		cipherText.append(keystream[i][j]^plainText[i*16+j])


print("CipherText", end=":")
print(bytes(cipherText))


# Modify cipher text:
print("Modifying CipherText....")
offset = len(s1)+16

replaceStr = ";admin=true"

for i in range(len(replaceStr)):
	temp = cipherText[offset-i-1]
	temp = temp^ord(replaceStr[-1-i])^ord('A')
	cipherText[offset-i-1] = temp

plainText = []

for i in range(len(keystream)):
	for j in range(16):
		plainText.append(keystream[i][j]^cipherText[i*16+j])

print("PlainText:")
print(''.join([chr(x) for x in plainText]))