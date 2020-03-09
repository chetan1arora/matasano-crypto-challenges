from Crypto.Cipher import AES
from pwn import *
import base64

nonce = pack(0x00,8*8,'little')

cipher = base64.b64decode('L77na/nrFsKvynd6HzOoG7GHTLXsTVu9qvY/2syLXzhPweyyMTJULu/6/kXX0KSvoOLSFQ==')

plain = []

for i in range(int(len(cipher)/16)+1):
	iv = nonce + pack(i, 8*8, 'little')
	# print(iv)
	obj = AES.new("YELLOW SUBMARINE",AES.MODE_ECB)
	keystream = obj.encrypt(iv)
	# print(keystream)
	for j in range(16):
		if(i*16+j== len(cipher)):
			break
		plain.append(keystream[j]^cipher[i*16+j])

print(''.join([chr(x) for x in plain]))