import os
import subprocess

from base64 import b64decode, b64encode
from unicorn import *
from unicorn.x86_const import *

import codecs

from pwn import *

bad = False
ADDRESS = 0x1000000
sys_read = 3
sys_write = 4


def hook_intr(uc, intno, user_data):
    if intno != 0x80:
        uc.emu_stop()
        return
    eax = uc.reg_read(UC_X86_REG_EAX)

    if eax != sys_write and eax != sys_read:
        global bad 
        bad = True
        print("wtf")
        uc.emu_stop()
        return


def hook_code(uc, address, size, user_data):
    op = (uc.mem_read(address, 1));
    if op == "\xcb" or op == "\xca":
        bad = False
        uc.emu_stop()
        return

	
	
def sandbox(mode, code):
    try:
        mu = Uc(UC_ARCH_X86, mode)
        mu.mem_map(ADDRESS, 2 * 1024 * 1024)
        mu.mem_write(ADDRESS, code)
        mu.reg_write(UC_X86_REG_ESP, ADDRESS + 0x200000)
        mu.hook_add(UC_HOOK_INTR, hook_intr)
        mu.hook_add(UC_HOOK_CODE, hook_code)
        mu.emu_start(ADDRESS, ADDRESS + len(code))
    except UcError as e:
        print("ERROR: %s" % e)


def hit_me():
    payload = raw_input()
    payload = b64decode(payload)
    sandbox(UC_MODE_32, payload)
    print(bad)
    if bad:
        print("wwwwwwwwwwwwwwwwwwwww")
        return -1	
    p = process("./hit_me")
    p.send(payload)
    p.interactive()
    return 0

if __name__ == '__main__':
    hit_me()
