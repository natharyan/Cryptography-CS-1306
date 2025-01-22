import random
from math import sqrt
from random import randrange, getrandbits
import sys

def is_prime(n, k=128):
    """ Test if a number is prime
        Args:
            n -- int -- the number to test
            k -- int -- the number of tests to do
        return True if n is prime
    """
    # test if n is not even.
    # 2 is prime
    if n == 2 or n == 3:
        return True
    if n <= 1 or n % 2 == 0:
        return False
    # find r and s
    s = 0
    r = n - 1
    while r & 1 == 0:
        s += 1
        r //= 2
    # do k tests
    for _ in range(k):
        a = randrange(2, n - 1)
        x = pow(a, r, n)
        if x != 1 and x != n - 1:
            j = 1
            while j < s and x != n - 1:
                x = pow(x, 2, n)
                if x == 1:
                    return False
                j += 1
            if x != n - 1:
                return False
    return True
def gen_prime_candidate(length):
    # generate random bits
    p = getrandbits(length)
    # apply a mask to set MSB and LSB to 1
    p |= (1 << length - 1) | 1
    return p
def gen_prime_number(length=1024):
    """ Generate a prime
        Args:
            length -- int -- length of the prime to generate, in          bits
        return a prime
    """
    p = 4
    # keep generating while the primality test fail
    while not is_prime(p, 128):
        p = gen_prime_candidate(length)
    return p

def fastexpmod(b, e, m):
    """
    Fast modular exponentiation
    Args:
        b -- int -- base
        e -- int -- exponent
        m -- int -- modulus
    return b^e mod m
    """
    result = 1
    while e > 0:
        if e % 2 == 1:
            result = (result * b) % m
        e = e // 2
        b = (b * b) % m
    return result

def primiteroot(p):
    """
    Find a primitive root modulo p
    Args:
        p -- int -- prime number
    return a primitive root modulo p
    """
    if p == 2:
        return 1
    p1 = 2
    p2 = (p-1) // p1
    while(1):
        g = random.randint(2, p-1)
        if not fastexpmod(g, (p-1)//p1, p) == 1:
            if not fastexpmod(g, (p-1)//p2, p) == 1:
                return g

class Alice(): 
    def generate_private_key(self,p):
        phip = p-1
        self.private_key = random.randint(0, phip-1)
        return self.private_key
    x = None

class Bob():
    def generate_private_key(self,p):
        phip = p-1
        self.private_key = random.randint(0, phip-1)
        return self.private_key
    y = None

if __name__ == "__main__":
    length = int(sys.argv[1])
    p = gen_prime_number(length)
    print("p:",p)
    alice = Alice()
    alice.x = alice.generate_private_key(p)
    bob = Bob()
    bob.y = bob.generate_private_key(p)
    g = primiteroot(p)
    print("primitive root g:",g)
    a = fastexpmod(g, alice.x, p)
    b = fastexpmod(g, bob.y, p)
    print("Alice sends to Bob:",a)
    print("Bob sends to Alice:",b)
    print("Alice computes key:",fastexpmod(b, alice.x, p))
    print("Bob computes key:",fastexpmod(a, bob.y, p))