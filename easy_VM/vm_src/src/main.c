#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "vm.h"

#define MAX_CODE_SIZE 8192

/*
POKA 2019 Easy_VM Rev & Pwn

based on https://github.com/parrt/simple-virtual-machine-C

# Rev 
find input value

# Pwn 
1. leak libc address and calculate other function address using served libc
	- system@libc
	- &__free_hook

2. in vm struct
```
    int *globals;
    int nglobals;

    // Operand stack, grows upwards
    int stack[DEFAULT_STACK_SIZE];
```
get globals pointer address using stack pop

3. overwrite globals pointer to free_hook
4. overwrite free_hook to system@libc
5. revoke globals address to any heap address
6. write command at globals buffer
7. halt and free(vm->globals);
*/

void help(){
	printf("[Usage]\n");
	printf("-f [File name]\n");
	printf("-i [User input]\n");
	exit(0);
}


int split (char *str, char c, char ***arr){
    int count = 1;
    int token_len = 1;
    int i = 0;
    char *p;
    char *t;

    p = str;
    while (*p != '\0'){
        if (*p == c)
            count++;
        p++;
    }

    *arr = (char**) malloc(sizeof(char*) * count);
    if (*arr == NULL)
        exit(1);

    p = str;
    while (*p != '\0'){
        if (*p == c){
            (*arr)[i] = (char*) malloc( sizeof(char) * token_len );
            if ((*arr)[i] == NULL)
                exit(1);

            token_len = 0;
            i++;
        }
        p++;
        token_len++;
    }

    (*arr)[i] = (char*) malloc( sizeof(char) * token_len );
    if ((*arr)[i] == NULL)
        exit(1);

    i = 0;
    p = str;
    t = ((*arr)[i]);
    while (*p != '\0'){
        if (*p != c && *p != '\0'){
            *t = *p;
            t++;
        }else{
            *t = '\0';
            i++;
            t = ((*arr)[i]);
        }
        p++;
    }
    return count;
}

int main(int argc, char *argv[]) {
	int is_f = 0;
	int is_in = 0;
	char *file_name = NULL;
	char *user_input = NULL;
	int param_opt;

	while( -1 !=( param_opt = getopt( argc, argv, "f:i:h"))){
		switch( param_opt){
			case  'f':  
				is_f  = 1;
				file_name = optarg;
				break;

			case  'i':  
				is_in  = 1;
				user_input = optarg;
				break;

			case 'h':
				help();
			case  '?':  
				help();
		}
	}

	if( (!is_f) || (file_name == NULL) ) {
		help();
	}

	long lSize;
	size_t result;
	char* buffer;
	FILE* fp = fopen(file_name, "r");
	if(fp == NULL) {
        fprintf(stderr, "Can't open file: %s\n", file_name);
        exit(1);
	}
	fseek(fp, 0, SEEK_END);
	lSize = ftell(fp);
	rewind(fp);

	buffer = (char*)malloc(sizeof(char) * lSize);
	if (buffer == NULL) {
		fprintf(stderr, "Memeory Error");
		exit(1);
	}

	result = fread(buffer, 1, lSize, fp);
	if (result != lSize) {
		fprintf(stderr, "Reading Error\n");
		exit(1);
	}
	fclose(fp);

	char **arr = NULL;
	int ins_cnt = split(buffer, ',' , &arr);

    int code[ins_cnt];

    for (int i = 0; i < ins_cnt; i++){
		code[i] = atoi(arr[i]);
		free(arr[i]);
    }
    free(arr);
	free(buffer);

    VM *vm;
    if(is_in){
    	int input_len = strlen(user_input);
    	int *globals_var = calloc(input_len, sizeof(int));
    	for(int i = 0; i < input_len; i ++ ){
    		// printf("char : %c\n", user_input[i]); // debug
    		globals_var[i] = (int)user_input[i];
    	}
			vm = vm_create(code, sizeof(code), input_len);
			vm->globals = globals_var;
    }else{

			vm = vm_create(code, sizeof(code), 0);
    }
	vm_exec(vm, 0);
	//if(is_in) { vm_print_data(vm->globals, vm->nglobals);} 
	vm_free(vm);

	return 0;
}

