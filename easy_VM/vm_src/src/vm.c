
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "vm.h"

typedef struct {
    char name[8];
    int nargs;
} VM_INSTRUCTION;

static VM_INSTRUCTION vm_instructions[] = {
    { "noop",   0 },    // 0
    { "iadd",   0 },    // 1
    { "isub",   0 },    // 2
    { "imul",   0 },    // 3
    { "idiv",   0 },    // 4
    { "irem",   0 },    // 5
    { "ixor",   0 },    // 6
    { "ilt",    0 },    // 7
    { "ieq",    0 },    // 8
    { "br",     1 },    // 9
    { "brt",    1 },    // 10
    { "brf",    1 },    // 11
    { "iconst", 1 },    // 12
    { "load",   1 },
    { "gload",  1 },
    { "store",  1 },
    { "gstore", 1 },
    { "print",  0 },
    { "pop",    0 },
    { "call",   3 },
    { "ret",    0 },
    { "halt",   0 }
};

static void vm_context_init(Context *ctx, int ip, int nlocals);

void vm_init(VM *vm, int *code, int code_size, int nglobals)
{
    vm->code = code;
    vm->code_size = code_size;
    vm->globals = calloc(nglobals, sizeof(int));
    //printf("vm->globals : %p\n", &vm->globals);
    vm->nglobals = nglobals;
}

void vm_free(VM *vm)
{
    free(vm->globals);
    free(vm);
}

VM *vm_create(int *code, int code_size, int nglobals)
{
    VM *vm = calloc(1, sizeof(VM));
    vm_init(vm, code, code_size, nglobals);
    return vm;
}

void vm_exec(VM *vm, int startip)
{
    // registers
    int ip;         // instruction pointer register
    int sp;         // stack pointer register
    int callsp;     // call stack pointer register

    int a = 0;
    int b = 0;
    int addr = 0;
    int offset = 0;

    ip = startip;
    sp = -1;
    callsp = -1;
    int opcode = vm->code[ip];

    while (opcode != HALT && ip < vm->code_size) {
        ip++; //jump to next instruction or to operand
        switch (opcode) {
            case NOOP:
                break;
            case IADD:
                b = vm->stack[sp--];           // 2nd opnd at top of stack
                a = vm->stack[sp--];           // 1st opnd 1 below top
                vm->stack[++sp] = a + b;       // push result
                break;
            case ISUB:
                b = vm->stack[sp--];
                a = vm->stack[sp--];
                vm->stack[++sp] = a - b;
                break;
            case IMUL:
                b = vm->stack[sp--];
                a = vm->stack[sp--];
                vm->stack[++sp] = a * b;
                break;
            case IDIV:
                b = vm->stack[sp--];
                a = vm->stack[sp--];
                vm->stack[++sp] = a / b;
                break;
            case IREM:
                b = vm->stack[sp--];
                a = vm->stack[sp--];
                vm->stack[++sp] = a % b;
                break;
            case IXOR:
                b = vm->stack[sp--];
                a = vm->stack[sp--];
                vm->stack[++sp] = a ^ b;
                break;
            case ILT:
                b = vm->stack[sp--];
                a = vm->stack[sp--];
                vm->stack[++sp] = (a < b) ? true : false;
                break;
            case IEQ:
                b = vm->stack[sp--];
                a = vm->stack[sp--];
                vm->stack[++sp] = (a == b) ? true : false;
                break;
            case BR:
                ip = vm->code[ip];
                break;
            case BRT:
                addr = vm->code[ip++];
                if (vm->stack[sp--] == true) ip = addr;
                break;
            case BRF:
                addr = vm->code[ip++];
                if (vm->stack[sp--] == false) ip = addr;
                break;
            case ICONST:
                vm->stack[++sp] = vm->code[ip++];  // push operand
                break;
            case LOAD: // load local or arg
                offset = vm->code[ip++];
                vm->stack[++sp] = vm->call_stack[callsp].locals[offset];
                break;
            case GLOAD: // load from global memory
                addr = vm->code[ip++];
                vm->stack[++sp] = vm->globals[addr];
                break;
            case STORE:
                offset = vm->code[ip++];
                vm->call_stack[callsp].locals[offset] = vm->stack[sp--];
                break;
            case GSTORE:
                addr = vm->code[ip++];
                vm->globals[addr] = vm->stack[sp--];
                break;
            case PRINT:
                printf("%d\n", vm->stack[sp--]);
                break;
            case POP:
                --sp;
                break;
            case CALL:
                // expects all args on stack
                addr = vm->code[ip++];			// index of target function
                int nargs = vm->code[ip++]; 	// how many args got pushed
                int nlocals = vm->code[ip++]; 	// how many locals to allocate
                ++callsp; // bump stack pointer to reveal space for this call
                vm_context_init(&vm->call_stack[callsp], ip, nargs+nlocals);
                // copy args into new context
                for (int i=0; i<nargs; i++) {
                    vm->call_stack[callsp].locals[i] = vm->stack[sp-i];
                }
                sp -= nargs;
                ip = addr;		// jump to function
                break;
            case RET:
                ip = vm->call_stack[callsp].returnip;
                callsp--; // pop context
                break;
            default:
                printf("[%d] invalid opcode: %d\n", (ip - 1), opcode);
                exit(1);
        }
        opcode = vm->code[ip];
    }
}

static void vm_context_init(Context *ctx, int ip, int nlocals) {
    if ( nlocals>DEFAULT_NUM_LOCALS ) {
        fprintf(stderr, "Call Error: %d\n", nlocals);
    }
    ctx->returnip = ip;
}

/*
void vm_print_data(int *globals, int count)
{
    printf("[Data]\n");
    for (int i = 0; i < count; i++) {
        printf("%04d: %d\n", i, globals[i]);
    }
}
*/