s = 7
def undo0(y):
	y = y^((y<<s) & 0x9d2c5680)
	y = y^((y<<s) & 0x9d2c5680) & 0x3fff
	y = y^((y<<s) & 0x9d2c4000) & 0x1fffff
	y = y^((y<<s) & 0x9d200000) & 0xfffffff
	y = y^((y<<s) & 0x90000000) & 0xffffffff
	return y


def undo1(y):
	y = y^((y<<15)& 0xefc60000)
	y = y^((y<<15)& 0xefc60000) & 0x3fffffff
	y = y^((y<<15)& 0xc0000000) & 0xffffffff
	return y

def undo2(y):
	y = (y^(y>>11))
	y = (y^( (y>>11) & 0xfffffc00))
	y = (y^ ((y>>11) & 0x3ff)) & 0xffffffff
	return y

def undo3(y):
	y = y^(y>>18)
	y = y^(y>>18)
	return y	


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

def undo(y):
	a = []
	a.append(undo0(y))
	a.append(undo1(y))
	a.append(undo2(y))
	a.append(undo3(y))
	return a

print([hex(i) for i in undo(0xdeadbeef)])
