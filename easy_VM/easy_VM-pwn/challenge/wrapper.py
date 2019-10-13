#!/usr/bin/python3
import os
import tempfile
import subprocess

banner = """
 _____                  _   ____  ___
|  ___|                | | | |  \/  |
| |__  __ _ ___ _   _  | | | | .  . |
|  __|/ _` / __| | | | | | | | |\/| |
| |__| (_| \__ \ |_| | \ \_/ / |  | |
\____/\__,_|___/\__, |  \___/\_|  |_/
                 __/ |               
                |___/                
"""

print(banner)
print("[ Payload ]")
print(">> ", end='')

data = input()
fp = tempfile.NamedTemporaryFile(delete=False)
fp.file.write(bytes(data, 'utf-8'))
fp.close()

try:
    ret = subprocess.check_output(["./vm", "-f", fp.name], timeout=10)
    print("===========Result===========")
    print(ret.decode('utf-8'))
except subprocess.CalledProcessError as error:
    print(error.output.decode('utf-8'))
finally:
    os.unlink(fp.name)
    exit()
