from struct import pack, unpack
import time

def retPadding(data,dataLen=-1):
    if(dataLen == -1):
        dataLen = len(data)

    # 62 bytes = 64 bytes - char(128)-dataLen
    padding = chr(128) + chr(0) *(62 - (dataLen + 8) % 64)
    return (data + bytes(padding,'utf-8') + pack('>Q',8*dataLen))

def sha1(data,dataLen = -1,h0 = 0x67452301,h1 = 0xEFCDAB89,h2 = 0x98BADCFE,h3 = 0x10325476,h4 = 0xC3D2E1F0):
    def rol(n, b):
        return ((n << b) | (n >> (32 - b))) & 0xffffffff

    # block size = 64 bytes
    # Digest size = 40 bytes
    padded_data = retPadding(data,dataLen)

    thunks = [padded_data[i:i+64] for i in range(0, len(padded_data), 64)]
    for thunk in thunks:
        w = list(unpack('>16L', thunk)) + [0] * 64
        for i in range(16, 80):
            w[i] = rol((w[i-3] ^ w[i-8] ^ w[i-14] ^ w[i-16]), 1)

        a, b, c, d, e = h0, h1, h2, h3, h4

        # Main loop
        for i in range(0, 80):
            if 0 <= i < 20:
                f = (b & c) | ((~b) & d)
                k = 0x5A827999
            elif 20 <= i < 40:
                f = b ^ c ^ d
                k = 0x6ED9EBA1
            elif 40 <= i < 60:
                f = (b & c) | (b & d) | (c & d) 
                k = 0x8F1BBCDC
            elif 60 <= i < 80:
                f = b ^ c ^ d
                k = 0xCA62C1D6

            a, b, c, d, e = rol(a, 5) + f + e + k + w[i] & 0xffffffff, \
                            a, rol(b, 30), c, d

        h0 = h0 + a & 0xffffffff
        h1 = h1 + b & 0xffffffff
        h2 = h2 + c & 0xffffffff
        h3 = h3 + d & 0xffffffff
        h4 = h4 + e & 0xffffffff

    return '%08x%08x%08x%08x%08x' % (h0, h1, h2, h3, h4)

# Hmac SHA1 implementation

from Crypto import Random
import random

random.seed(time.time())
message = bytes("comment1=cooking%20MCs;userdata=foo;comment2=%20like%20a%20pound%20of%20bacon",'utf-8')

newMessage = bytes(";admin=true;",'utf-8')

key = Random.new().read(int(random.random()*64))

mac = sha1(key+message)

print("Message Intercepted",end=':')
print(message)

print("MAC",end=':')
print(mac)


# New Message to be intercepted
print("New Message to be Intercepted",end=":")
print(retPadding(key+message,-1)+newMessage)

newMac = sha1(retPadding(key+message,-1)+newMessage)

print("New MAC to be produced ",end=":")
print('X'*40)

# Length Extension attack
# We don't need to assume any key Len because the old message hashed would be padded 

messageLen = len(message)
numOfBlocks = int(messageLen/64)

# New state Variables
[p,q,r,s,t] = [int(mac[8*i:(8*i)+8],16) for i in range(5)]

for i in range(1,10):
    newMessageLen = (numOfBlocks+i)*64 + len(newMessage)
    crackedMac = sha1(newMessage,dataLen = newMessageLen, h0=p, h1=q, h2=r, h3=s, h4=t)
    if(crackedMac == newMac):
        print("[*] Cracked MAC",end=":")
        print(crackedMac)


