# Implement DH

Optimised modexp(b is large) with fast multiplication (log n)

# Not implementing MITM attacks

# MITM attack using parameter Injection

If middle man change the public key to p, then
session key becomes 0

# MITM attack using malicious g param
They first agree on p,g before exchanging public keys.

Assuming we dont inject p value, if we change g values to:
1: public key = 1
p: public key = p%p = 0
p-1 public key = (p\*p)%p = 1

So, we change public keys without injecting them.
