//gcc -g runner.c -o runner -lseccomp -static
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <seccomp.h>
#include <linux/seccomp.h>
#include <fcntl.h>

int perr(char *msg){
	fprintf(stderr, msg, strlen(msg));
}

int main(int argc, char *argv[]){

	if (argv[1] == NULL){
		printf("no argv\n");
		return -1;
	}

    scmp_filter_ctx ctx;
	ctx = seccomp_init(SCMP_ACT_ALLOW);
	seccomp_rule_add(ctx, SCMP_ACT_KILL, SCMP_SYS(execve), 1, SCMP_A0(SCMP_CMP_NE, (scmp_datum_t)(argv[1])));
	seccomp_rule_add(ctx, SCMP_ACT_KILL, SCMP_SYS(open), 1, SCMP_CMP(1, SCMP_CMP_MASKED_EQ, O_WRONLY, O_WRONLY));
	seccomp_rule_add(ctx, SCMP_ACT_KILL, SCMP_SYS(open), 1, SCMP_CMP(1, SCMP_CMP_MASKED_EQ, O_RDWR, O_RDWR));
	seccomp_load(ctx);

	alarm(2);
	perr("executing...\n");
    char * noargv[] = {NULL};
    execvp(argv[1], noargv);
	perr("\nend of execution\n");
    return 0;
}
