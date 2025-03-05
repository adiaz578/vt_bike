#!/usr/bin/env python3

import numpy as np
import math

VECTOR_LEN = 120
BLOCKS = 4


W3 = 0x00C00009
W2 = 0x00000004
W1 = 0x00000002
W0 = 0x00000001

E3 = 0x00400007
E2 = 0x00000002
E1 = 0x00000011
E0 = 0x000F0001

M3 = 0x00CF0000
M2 = 0x00CF00CD
M1 = 0x00000000
M0 = 0x00000000

#M3 = 0x00345345
#M2 = 0xFE345A23
#M1 = 0xCBE324A3
#M0 = 0x45423F45


S = (W3 << 96) + (W2 << 64) + (W1 << 32) + W0

RES = (E3 << 96) + (E2 << 64) + (E1 << 32) + E0

M = (M3 << 96) + (M2 << 64) + (M1 << 32) + M0

RES = np.bitwise_xor((M >> (VECTOR_LEN-1))*S, RES)

#print((S >> (VECTOR_LEN-1)))

for i in range(0, 119):#VECTOR_LEN-1):
  msb = (S >> (VECTOR_LEN-1))
  S = (S << 1) | msb
  S = np.mod(S, (1 << 128))
  print("BIT M:", ((M >> (VECTOR_LEN-2-i)) & 0x1))
  RES = np.bitwise_xor(((M >> (VECTOR_LEN-2-i)) & 0x1)*S, RES)
  
print("HEX Werte Polynom:")  
print(hex((S & (2**(32)-1))).split('x')[-1].zfill(8))
print(hex(((S >> 32) & (2**(32)-1))).split('x')[-1].zfill(8))
print(hex(((S >> 64) & (2**(32)-1))).split('x')[-1].zfill(8))
print(hex(((S >> 96) & (2**(32)-1))).split('x')[-1].zfill(8))


print("HEX Werte RESULT:")  
print(hex((RES & (2**(32)-1))).split('x')[-1].zfill(8))
print(hex(((RES >> 32) & (2**(32)-1))).split('x')[-1].zfill(8))
print(hex(((RES >> 64) & (2**(32)-1))).split('x')[-1].zfill(8))
print(hex(((RES >> 96) & (2**(32)-1))).split('x')[-1].zfill(8))
