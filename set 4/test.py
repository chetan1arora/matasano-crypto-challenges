#!/usr/bin/env python
from struct import pack, unpack
import time, sys

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


key =  bytes("YELLOW SUBMARINE",'utf-8')

if(len(sys.argv) != 2):
    print("Usage: ./test.py message")
    sys.exit()

message = bytes(sys.argv[1],'utf-8')

digest = sha1(key+message)

def checkMAC(digest, testDigest):
    # Digit wise compare
    if(len(digest) != len(testDigest)):
        print("Digest Length mismatch")
    for i in range(len(digest)):
        time.sleep(.05)
        if(digest[i] != testDigest[i]):
            return 500

    return 200

while(1):
    testDigest = input()
    out = checkMAC(digest, testDigest)
    if(out == 500):
        print("Nope")
    else:
        print("Access Granted")