# Hastards Broadcast Attack
# Best source by Jeff Sukuzi
import RSA
from RSA import *
# ciphers list 
# (e,pubkey) for pubkeys
def broadcastAttack(ciphers, pubkeys):
	# Check if all has same e
	l = len(pubkeys)
	if not l:
		print("Empty pubkeys")
		sys.exit(0)
	e = pubkeys[0][0]
	for x in pubkeys:
		if(x[0] != e):
			print("Not encrypted using same e")
			sys.exit(0)
	modN = 1
	for x in pubkeys:
		modN *= x[1]
	numSum = 0
	for i in range(l):
		temp = (modN//pubkeys[i][1])
		numSum += temp*invMul(temp,pubkeys[i][1])*ciphers[i]
	numSum %= modN
	mes = round(math.pow(numSum,1/e))
	return mes

if __name__ == "__main__":
	print("[*]Encrypting using RSA")
	print("[*]Input message",end=":")
	mes = input().encode('utf-8')
	ciphers = []
	pubkeys = []
	e = 7
	for i in range(e):
		# Delete privatekeys
		pubk,_ = genKey(512,e)
		enc = encrypt(mes,pubk)
		enc = int(enc.hex(),16)
		ciphers.append(enc)
		pubkeys.append(pubk)

	_ = 0
	print("[+] Encrypted using different public keys and broadcasting...")
	crackedMessage = broadcastAttack(ciphers, pubkeys)
	crackedMessage = hex(crackedMessage).replace('0x','')
	if(len(crackedMessage)%2):
		crackedMessage = '0'+ crackedMessage
	mes = bytes.fromhex(crackedMessage).decode('utf-8')
	print("[+] Cracked message :"+mes)
