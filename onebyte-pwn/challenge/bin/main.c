#include <stdio.h>

int main(int argc, char **argv)
{
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stdin, NULL, _IONBF, 0);
	char buf[256];

	read(0, buf, 256);
	puts(buf);

	read(0, buf, 256);
	puts(buf);

	read(0, buf, 256);
	return 0;
}
