#include <stdio.h>
#include <string.h>
#include <unistd.h> 
char shellcode[0x100] = {0 ,};
int main(void)
{
	read(0, shellcode, 0x100);
	(*(void(*)()) shellcode)();
	return 0;
}
