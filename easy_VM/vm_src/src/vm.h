
#ifndef VM_H_
#define VM_H_

#ifdef __cplusplus
extern "C" {
#endif

#define DEFAULT_STACK_SIZE      1000
#define DEFAULT_CALL_STACK_SIZE 100
#define DEFAULT_NUM_LOCALS      10

typedef enum {
    NOOP    = 0,
    IADD    = 1,   // int add
    ISUB    = 2,
    IMUL    = 3,
    IDIV    = 4,
    IREM    = 5,
    IXOR    = 6,
    ILT     = 7,   // int less than
    IEQ     = 8,   // int equal
    BR      = 9,   // branch
    BRT     = 10,   // branch if true
    BRF     = 11,   // branch if true
    ICONST  = 12,   // push constant integer
    LOAD    = 13,  // load from local context
    GLOAD   = 14,  // load from global memory
    STORE   = 15,  // store in local context
    GSTORE  = 16,  // store in global memory
    PRINT   = 17,  // print stack top
    POP     = 18,  // throw away top of stack
    CALL    = 19,  // call function at address with nargs,nlocals
    RET     = 20,  // return value from function
    HALT    = 21
} VM_CODE;

typedef struct {
    int returnip;
    int locals[DEFAULT_NUM_LOCALS];
} Context;

typedef struct {
    int *code;
    int code_size;

    // global variable space
    int *globals;
    int nglobals;

    // Operand stack, grows upwards
    int stack[DEFAULT_STACK_SIZE];
    Context call_stack[DEFAULT_CALL_STACK_SIZE];
} VM;

VM *vm_create(int *code, int code_size, int nglobals);
void vm_free(VM *vm);
void vm_init(VM *vm, int *code, int code_size, int nglobals);
void vm_exec(VM *vm, int startip);
//void vm_print_data(int *globals, int count);

#ifdef __cplusplus
}
#endif

#endif
