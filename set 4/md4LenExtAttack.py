from struct import pack, unpack
import time

def retPadding(data,dataLen=-1):
    if(dataLen == -1):
        dataLen = len(data)

    # 62 bytes = 64 bytes - char(128)-dataLen
    padding = chr(128) + chr(0) *(62 - (dataLen + 8) % 64)
    return (data + bytes(padding,'utf-8') + pack('<Q',8*dataLen))


def lrot(x,n):
    return (x << n) | (x >> (32 - n))
def _F(x,y,z):
    return ((x & y) | (~x & z))
def _G(x,y,z):
    return ((x & y) | (x & z) | (y & z))
def _H(x,y,z):
    return (x ^ y ^ z)

def md4(data,dataLen = -1,h0 = 0x67452301,h1 = 0xEFCDAB89,h2 = 0x98BADCFE,h3 = 0x10325476):
    
    # block size = 16 bytes
    # Digest size = 40 digits
    padded_data = retPadding(data,dataLen)

    thunks = [padded_data[i:i+64] for i in range(0, len(padded_data), 64)]
    for thunk in thunks:
        X = list(unpack('<' + 'I' * 16, thunk))
        A, B, C, D = h0, h1, h2, h3

        # MD4 Rounds 
        for i in range(16):
            k = i
            if i % 4 == 0:
                A = lrot((A + _F(B, C, D) + X[k]) & 0xffffffff, 3)
            elif i % 4 == 1:
                D = lrot((D + _F(A, B, C) + X[k]) & 0xffffffff, 7)
            elif i % 4 == 2:
                C = lrot((C + _F(D, A, B) + X[k]) & 0xffffffff, 11)
            elif i % 4 == 3:
                B = lrot((B + _F(C, D, A) + X[k]) & 0xffffffff, 19)

        for i in range(16):
            k = (i // 4) + (i % 4) * 4
            if i % 4 == 0:
                A = lrot((A + _G(B, C, D) + X[k] + 0x5a827999) & 0xffffffff, 3)
            elif i % 4 == 1:
                D = lrot((D + _G(A, B, C) + X[k] + 0x5a827999) & 0xffffffff, 5)
            elif i % 4 == 2:
                C = lrot((C + _G(D, A, B) + X[k] + 0x5a827999) & 0xffffffff, 9)
            elif i % 4 == 3:
                B = lrot((B + _G(C, D, A) + X[k] + 0x5a827999) & 0xffffffff, 13)

        order = [0, 8, 4, 12, 2, 10, 6, 14, 1, 9, 5, 13, 3, 11, 7, 15]
        for i in range(16):
            k = order[i]
            if i % 4 == 0:
                A = lrot((A + _H(B, C, D) + X[k] + 0x6ed9eba1) & 0xffffffff, 3)
            elif i % 4 == 1:
                D = lrot((D + _H(A, B, C) + X[k] + 0x6ed9eba1) & 0xffffffff, 9)
            elif i % 4 == 2:
                C = lrot((C + _H(D, A, B) + X[k] + 0x6ed9eba1) & 0xffffffff, 11)
            elif i % 4 == 3:
                B = lrot((B + _H(C, D, A) + X[k] + 0x6ed9eba1) & 0xffffffff, 15)


        h0 = h0 + A & 0xffffffff
        h1 = h1 + B & 0xffffffff
        h2 = h2 + C & 0xffffffff
        h3 = h3 + D & 0xffffffff

    return '%08x%08x%08x%08x' % (h0, h1, h2, h3)

# Hmac md4 implementation

from Crypto import Random
import random

random.seed(time.time())
message = bytes("comment1=cooking%20MCs;userdata=foo;comment2=%20like%20a%20pound%20of%20bacon",'utf-8')

newMessage = bytes(";admin=true;",'utf-8')

key = Random.new().read(int(random.random()*64))

mac = md4(key+message)

print("Message Intercepted",end=':')
print(message)

print("MAC",end=':')
print(mac)


# New Message to be intercepted
print("New Message to be Intercepted",end=":")
print(retPadding(key+message,-1)+newMessage)

newMac = md4(retPadding(key+message,-1)+newMessage)

print("New MAC to be produced ",end=":")
print('X'*32)
# print(newMac)

# Length Extension attack
# We don't need to assume any key Len because the old message hashed would be padded 

messageLen = len(message)
numOfBlocks = int(messageLen/64)

# New state Variables
[p,q,r,s] = [int(mac[8*i:(8*i)+8],16) for i in range(4)]

for i in range(1,10):
    newMessageLen = (numOfBlocks+i)*64 + len(newMessage)
    crackedMac = md4(newMessage,dataLen = newMessageLen, h0=p, h1=q, h2=r, h3=s)
    if(crackedMac == newMac):
        print("[*] Cracked MAC",end=":")
        print(crackedMac)