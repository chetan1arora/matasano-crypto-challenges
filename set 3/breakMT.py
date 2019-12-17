w, n, m, r = (32, 624, 397, 31)
a = 0x9908B0DF
(u, d) = (11, 0xFFFFFFFF)
(s, b) = (7, 0x9D2C5680)
(t, c) = (15, 0xEFC60000)
l = 18
f= 1812433253

MT = [0 for x in range(n)]
index = n+1
lower_mask = (1<<r) -1 # That is, the binary number of r 1's
upper_mask = ((1 << w) - 1) & (~lower_mask)

# Initialize the generator from a seed
def seed_mt(seed):
    global index
    index = n
    MT[0] = seed
    for i in range(1,n):
        # // loop over each element
        MT[i] = ((1 << w) - 1) & (f * (MT[i-1] ^ (MT[i-1] >> (w-2))) + i)

# Extract a tempered value based on MT[index]
# calling twist() every n numbers

def extract_number():
    global index
    if(index >= n):
        if(index > n):
            seed_mt(1)
        twist()

    y = MT[index]
    y = y ^ ((y >> u) & d)
    y = y ^ ((y << s) & b)
    y = y ^ ((y << t) & c)
    y = y ^ (y >> l)

    index = index + 1
    return ((1 << w) - 1) & (y)


# Generate the next n values from the series x_i 
def twist():
    global index
    for i in range(n):
        x = (MT[i] & upper_mask) + (MT[(i+1) % n] & lower_mask)
        xA = x >> 1
        if((x % 2) != 0): # lowest bit of x is 1
            xA = xA ^ a
        MT[i] = MT[(i + m) % n] ^ xA
    index = 0


def untemper(y):
    y = y^(y>>18)
    y = y^((y<<15)& 0xefc60000) & 0x3fffffff
    y = y^((y<<15)& 0xc0000000) & 0xffffffff
    y = y^((y<<7) & 0x9d2c5680) & 0x3fff
    y = y^((y<<7) & 0x9d2c4000) & 0x1fffff
    y = y^((y<<7) & 0x9d200000) & 0xfffffff
    y = y^((y<<7) & 0x90000000) & 0xffffffff
    y = (y^( (y>>11) & 0xfffffc00))
    y = (y^ ((y>>11) & 0x3ff)) & 0xffffffff
    return y

import time,math


seed_mt(int(time.time()))

cloneMT = [0 for x in range(n)]

for i in range(n):
    a = extract_number()
    cloneMT[i] = untemper(a)


idx = 0
def clone_twist():
    global idx
    for i in range(n):
        x = (cloneMT[i] & upper_mask) + (cloneMT[(i+1) % n] & lower_mask)
        xA = x >> 1
        if((x % 2) != 0): # lowest bit of x is 1
            xA = xA ^ a
        cloneMT[i] = cloneMT[(i + m) % n] ^ xA
    idx = 0

clone_twist()

def clone_extract_number():
    global idx
    y = cloneMT[idx]
    y = y ^ ((y >> u) & d)
    y = y ^ ((y << s) & b)
    y = y ^ ((y << t) & c)
    y = y ^ (y >> l)

    idx = idx + 1
    return ((1 << w) - 1) & (y)


for i in range(7):
    print(hex(extract_number()))
    print(hex(clone_extract_number()))

