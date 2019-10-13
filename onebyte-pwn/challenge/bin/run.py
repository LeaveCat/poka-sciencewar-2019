#!/usr/bin/python -u
from os import system
import os

def read_until(deli):
    out = ''
    while True:
        _ = raw_input().strip('\n')
        if _ == deli:
            return out
        else:
            out += _ + '\n'

original  = open('/home/ctf/main.c', 'rb').read().strip()
malformed = read_until('$').strip()

check_len = max(len(original), len(malformed))

count = 0
for i in xrange(check_len):
    try:
        if original[i] != malformed[i]: count += 1
    except IndexError:
        print('wtf')
        exit(-1)

    if count > 1:
        exit(-2)

print(malformed) # your source code

import uuid
random_fn = uuid.uuid4().hex

with open('/tmp/sc/%s.c' % random_fn, 'wb') as f:
    f.write(malformed)

system('gcc -o /tmp/sc/%s /tmp/sc/%s.c' % (random_fn, random_fn))

os.setgroups([])
os.setgid(1000)
os.setuid(1000)

system('/tmp/sc/%s' % random_fn)

