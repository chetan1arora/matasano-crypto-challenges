from Crypto.Cipher import AES
from Crypto import Random


s1 = b'comment1=cooking%20MCs;userdata='
s2 = b';comment2= like a pound of bacon'

rand = Random.new()
key = rand.read(16)
iv = rand.read(16)

obj = AES.new(key,AES.MODE_CBC, iv)

payload = b'A'*32

cipher = obj.encrypt(s1+payload+s2)

print("CipherText",end=':')
print(cipher)

cipherArray = [x for x in cipher]

offset = len(s1)+16

replaceStr = ";admin=true"

for i in range(len(replaceStr)):
	temp = cipherArray[offset-i-1]
	temp = temp^ord(replaceStr[-1-i])^ord("A")
	cipherArray[offset-i-1] = temp

cipher = bytes(cipherArray)

obj = AES.new(key,AES.MODE_CBC,iv)

plain = obj.decrypt(cipher)

print("Modified Plain Text",end=":")
print(plain)
