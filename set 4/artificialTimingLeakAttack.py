from pwn import *
import time


p = process(['./test.py','Howdy'])

#timeout=0.75
# skew = 0.025

msgDigest = pack

hexArray = ['0' for x in range(40)]

for i in range(40):
	p.recvline(timeout=0.05)
	print(i)
	found = False
	while(not found):
		for j in range(16):
			hexArray[i] = format(j,'x')
			p.sendline(''.join(hexArray))
			out = p.recvline(timeout = 0.0055*(i+1))
			print(''.join(hexArray))
			print(out)
			if(out == b'Nope\n'):
				continue
			else:
				found = True
				if(out != b''):
					print(out)
				break
		p.recvline(timeout=0.5)
