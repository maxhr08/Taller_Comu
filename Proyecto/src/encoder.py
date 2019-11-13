import numpy as np
from pyldpc import make_ldpc, encode, decode, get_message
n = 15
d_v = 4
d_c = 5
snr = 10
H, G = make_ldpc(n, d_v, d_c, systematic=True, sparse=True)
k = G.shape[1]
v = np.random.randint(2, size=k)
y = encode(G, v, snr)
d = decode(H, y, snr, maxiter=100, log=True)
x = get_message(G, d)
assert abs(x - v).sum() == 0


"""
================================================
Coding - Decoding simulation of a random message
================================================

This example shows a simulation of the transmission of a binary message
through a gaussian white noise channel with an LDPC coding and decoding system.
"""


