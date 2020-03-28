from pwn import *
import time


p = process(['./test.py','Howdy'])

#timeout=0.75
# skew = 0.025

msgDigest = pack

hexArray = ['0' for x in range(40)]

for i in range(40):
	p.recvline(timeout=1)
	print(i)
	while(1):
		p.sendline(''.join(hexArray))
		out = p.recvline(timeout = 0.051*(i+1))
		if(out == b'Nope\n'):
			hexArray[i] = format((int(hexArray[i],16) +1)%16,'x')
		else:
			if(out != b''):
				print(out)
			break