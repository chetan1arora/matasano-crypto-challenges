from Crypto.Cipher import AES
import base64
f = open('10.txt','r').read().split('\n')

text = base64.b64decode(''.join(f))

num_blocks = int(len(text)/16)

iv = bytes(16)

cipher = AES.new(b'YELLOW SUBMARINE',AES.MODE_ECB)

plain = []

for i in range(num_blocks-1):
	if(i==num_blocks-2):
		break
	a = cipher.decrypt(text[(num_blocks-(2+i))*16:(num_blocks-1-i)*16])
	b = text[(num_blocks-(3+i))*16:(num_blocks-2-i)*16]
	print(len(a))
	print(len(b))
	for j in range(16):
		plain.append(a[15-j]^b[15-j])

plain = plain[::-1]

a = cipher.decrypt(text[0:16])

for i in range(16):
	plain[i]=a[i]^iv[i]

print(''.join(chr(x) for x in plain))
