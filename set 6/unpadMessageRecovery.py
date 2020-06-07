from RSA import *
import random


def mutateMessage(enc,pubkey):
	(e,modn) = pubkey
	param = random.randint(2,modn-1)
	s = modExp(param,e,modn)
	enc = int(enc.hex(),16)
	enc = (enc*s)%modn
	enc = hex(enc).replace('0x','')
	if(len(enc)%2):
		enc = '0'+enc
	return (param,bytes.fromhex(enc))

def extractMessage(dec,pubkey, param):
	(_,modn) = pubkey
	dec = int(dec.hex(), 16)
	s = invMul(param,modn)
	dec = (dec*s)%modn
	dec = hex(dec).replace('0x','')
	if(len(dec)%2):
		dec = '0'+dec
	return bytes.fromhex(dec)


if __name__ == "__main__":
	e = 47
	print("Unpadded message recovery oracle by differenting message.")
	pubk,privatek = genKey(1024,e)
	print("Input message",end=":")
	mes = input().encode('utf-8')
	enc = encrypt(mes,pubk)
	print("Encrypted string is :"+enc.hex())
	print("Mutating message...")
	(param,edash) = mutateMessage(enc,pubk)
	print("Mutated encrypted string is :"+edash.hex())
	ddash = decrypt(edash,privatek,1)
	dec = extractMessage(ddash,pubk,param)
	print("[+]Decrypted string is :"+dec.decode('utf-8'))