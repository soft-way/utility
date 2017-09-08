#!/bin/python
#
# Super simple Elliptic Curve Presentation. No imported libraries, wrappers, nothing. 
# For educational purposes only. Remember to use Python 2.7.6 or lower. You'll need to make changes for Python 3.
#
# Based on https://github.com/wobine/blackboard101/blob/master/EllipticCurvesPart4-PrivateKeyToPublicKey.py
#
# Below are the public specs for curve - the prime256v1/secp256r1
# http://www.secg.org/sec2-v2.pdf 2.4.2 Recommended Parameters secp256r1
#

# The proven prime
Pcurve = 0xFFFFFFFF00000001000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFF
N = 0xFFFFFFFF00000000FFFFFFFFFFFFFFFFBCE6FAADA7179E84F3B9CAC2FC632551 # Number of points in the field
Acurve = 0xFFFFFFFF00000001000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFC
Bcurve = 0x5AC635D8AA3A93E7B3EBBD55769886BC651D06B0CC53B0F63BCE3C3E27D2604B # These two defines the elliptic curve. y^2 = x^3 + Acurve * x + Bcurve
Gx = 0x6B17D1F2E12C4247F8BCE6E563A440F277037D812DEB33A0F4A13945D898C296
Gy = 0x4FE342E2FE1A7F9B8EE7EB4A7C0F9E162BCE33576B315ECECBB6406837BF51F5

GPoint = (Gx, Gy) # This is our generator point. Trillions of dif ones possible

#Individual Transaction/Personal Information
privKey = 0xAF0A68BE074839313A25CEEF91F80886A83B7A083A73C1C751DC209CBAE14A6C #replace with any private key

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
