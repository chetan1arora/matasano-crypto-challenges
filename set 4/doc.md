# First need some practice with crypto library python

Virtual env with crypto installed
pycrypto library used

Also using pwntools for occasional "little" endianess byte strings.

Encryption/Decryptions occurs on immutable data types.

Use byteArray instead of bytes

key==iv is not a question, if the error message returns (not correct) plaintext.

Not using any paddings in these questions, can be revised at a later time.


# Length Extension attack done on HMAC sha1

NO need to guess key length.

Guess the number of block sizes after padding, not difficult.

Using the same state of sha1 algo, we can append any new message to the old message.






