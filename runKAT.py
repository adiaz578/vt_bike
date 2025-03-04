#!/usr/bin/env python3

import sys
from pexpect import spawn, EOF

# start cryptol
print("Hello from Python!")
cryptol = spawn("cryptol")

# set crypol ascii mode
cryptol.expect("Cryptol> ")
cryptol.sendline(":set ascii = on")
cryptol.expect("on")

# prep cryptol message
message = "hello from Cryptol!"
cryptol_message = "[ "
for i in range(len(message)):
    cryptol_message += '0x' + str(message[i].encode().hex())
    
    if (i != len(message)-1):
        cryptol_message += ", "
cryptol_message += " ]"

# send cryptol message
cryptol.sendline(cryptol_message)
cryptol.sendline(":q")
cryptol.expect(":q")

# get output from cryptol
print("Cryptol says:")
print(cryptol.before.decode())
