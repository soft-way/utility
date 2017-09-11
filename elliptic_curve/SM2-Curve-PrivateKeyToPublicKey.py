#!/bin/python
#
# Super simple Elliptic Curve Presentation. No imported libraries, wrappers, nothing. 
# For educational purposes only. Remember to use Python 2.7.6 or lower. You'll need to make changes for Python 3.
#
# Based on https://github.com/wobine/blackboard101/blob/master/EllipticCurvesPart4-PrivateKeyToPublicKey.py
#
# Below are the public specs for curve - the prime256v1/secp256r1
# http://www.secg.org/sec2-v2.pdf 2.4.2 Recommended Parameters secp256k1
#

# The proven prime
Pcurve = 0xFFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFF
N = 0xFFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFF7203DF6B21C6052B53BBF40939D54123 # Number of points in the field

# These two defines the elliptic curve. y^2 = x^3 + Acurve * x + Bcurve
Acurve = 0xFFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFC
Bcurve = 0x728E9FA9E9D9F5E344D5A9E4BCF6509A7F39789F515AB8F92DDBCBD414D940E93

Gx = 0x32C4AE2C1F1981195F9904466A39C9948FE30BBFF2660BE1715A4589334C74C7
Gy = 0xBC3736A2F4F6779C59BDCEE36B692153D0A9877CC62A474002DF32E52139F0A0

GPoint = (Gx,Gy) # This is our generator point. Trillions of dif ones possible

#Individual Transaction/Personal Information
privKey = 0xE04B838562E045B0A141D1933B4AF2830873CBF7ED758FA9277D9D20835AC714 #replace with any private key

def modinv(a, n=Pcurve): # Extended Euclidean Algorithm/'division' in elliptic curves
    lm, hm = 1, 0
    low, high = a%n, n
    while low > 1:
        ratio = high/low
        nm, new = hm - lm*ratio, high - low*ratio
        lm, low, hm, high = nm, new, lm, low
    return lm % n

def ECadd(a, b): # Not true addition, invented for EC. Could have been called anything.
    LamAdd = ((b[1] - a[1]) * modinv(b[0] - a[0], Pcurve)) % Pcurve
    x = (LamAdd*LamAdd - a[0] - b[0]) % Pcurve
    y = (LamAdd*(a[0] - x) - a[1]) % Pcurve
    return (x,y)

def ECdouble(a): # This is called point doubling, also invented for EC.
    Lam = ((3*a[0]*a[0] + Acurve) * modinv((2*a[1]), Pcurve)) % Pcurve
    x = (Lam*Lam - 2*a[0]) % Pcurve
    y = (Lam*(a[0]-x) - a[1]) % Pcurve
    return (x,y)

def EccMultiply(GenPoint, ScalarHex): # Double & add. Not true multiplication
    if ScalarHex == 0 or ScalarHex >= N: raise Exception("Invalid Scalar/Private Key")
    ScalarBin = str(bin(ScalarHex))[2:]
    Q = GenPoint
    for i in range (1, len(ScalarBin)): # This is invented EC multiplication.
        Q = ECdouble(Q); # print "DUB", Q[0]; print
        if ScalarBin[i] == "1":
            Q = ECadd(Q, GenPoint); # print "ADD", Q[0]; print
    return (Q)

print; print "******* Public Key Generation *********"; 
print
PublicKey = EccMultiply(GPoint, privKey)
print "The private key:"
print privKey
print "0x%064X" % privKey; print
print PublicKey; print
print "The public Key - compressed:"; 
if PublicKey[1] % 2 == 1: # If the Y value for the Public Key is odd.
    print "03"+str(hex(PublicKey[0])[2:-1]).zfill(64).upper()
else: # Or else, if the Y value is even.
    print "02"+str(hex(PublicKey[0])[2:-1]).zfill(64).upper()
