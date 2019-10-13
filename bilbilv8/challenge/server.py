import string
import random
from pwn import *
import sys
N = 7
name = ''.join(random.choice(string.ascii_uppercase + string.digits) for _ in range(N))
name = "/tmp/"+name
f = open(name, "w")
print "code(base64) :"
sys.stdout.flush()

code =sys.stdin.readline().strip()
import base64
print code
code = base64.b64decode(code)
f.write(code)
f.close()

p = process(["/home/d8/d8", name])
p.interactive()
