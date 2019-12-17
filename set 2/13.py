from Crypto.Cipher import AES

cipher = AES.new(bytes(16),AES.MODE_ECB)

def profile_for(email):
	if("&" in email):
		email = ''.join(email.split("&"))

	plain_text = "email:"+email+"&id:10&role:user"
	temp = len(plain_text)%16
	if(temp != 0):
		temp = 16 - temp
		lol = [ord(i) for i in plain_text]
		for i in range(temp):
			lol.append(temp)
		plain_text = bytes(lol)
	return cipher.encrypt(plain_text)

test = profile_for("root@gmailadmin\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA.com")


temp = int(len(test)/16)
l = []

for i in range(temp):
	l.append(test[i*16:16*(i+1)])


# print(l)

temp =[b'z\xdat\xfc\xeeb\xf8=|\xb4\x0cUF\xf8\x96\x00', b'\xd4?\xab\xb6@a\x0e\xd7#\x05\x16\x85\xa7\xb8\xc2\x05', b'\xb4\x9c\xbf\x19\xd3W\xe6\xe1\xf6\x84\\0\xfd[c\xe3', b'\xb4\x9c\xbf\x19\xd3W\xe6\xe1\xf6\x84\\0\xfd[c\xe3', b'\x9d\xf0-\x84\x8d\x17,\x1e\xae\x06\x988P\xda7d', b'\x0ea\xe2x+D\xacJ\xf0\xc7B\x19\xfeo,\xdb']

test = b'z\xdat\xfc\xeeb\xf8=|\xb4\x0cUF\xf8\x96\x00\x9d\xf0-\x84\x8d\x17,\x1e\xae\x06\x988P\xda7d\xd4?\xab\xb6@a\x0e\xd7#\x05\x16\x85\xa7\xb8\xc2\x05'


a = cipher.decrypt(test)

print(a.decode('utf-8'))
