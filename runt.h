#ifndef RUNT_H
#define RUNT_H

#define RUNT_KILOBYTE 1000
#define RUNT_MEGABYTE (RUNT_KILOBYTE * 1000)
#define RUNT_GIGABYTE (RUNT_MEGABYTE * 1000)
#define RUNT_STACK_SIZE 32
#define RUNT_MODE_PROC 1

enum {
RUNT_NOT_OK = 0,
RUNT_OK,
RUNT_NIL = 0,
RUNT_FLOAT,
RUNT_STRING,
RUNT_CELL
};

typedef struct runt_vm runt_vm;

typedef int runt_int;
typedef float runt_float;
typedef int runt_type;
typedef unsigned int runt_uint;

typedef struct {
    runt_type type;
    void *ud;
} runt_ptr;

typedef runt_int (*runt_proc)(runt_vm *, runt_ptr);

typedef struct {
    runt_proc fun;
    runt_ptr p;
    runt_uint psize;
} runt_cell;

typedef struct {
    runt_type t;
    runt_ptr p;
    runt_float f;
} runt_stacklet;

typedef struct {
    runt_stacklet stack[RUNT_STACK_SIZE];
    runt_int pos;
    runt_int size;
} runt_stack;

typedef struct runt_entry {
    runt_cell *cell;
    runt_proc copy;
} runt_entry;

typedef struct runt_list {
    runt_int size;
    runt_entry root;
    runt_entry *last;
} runt_list;

typedef struct {
    runt_int size;
    runt_list list[128];
} runt_dict;

typedef struct {
    unsigned char *data;
    runt_uint size;
    runt_uint used;
} runt_memory_pool;

typedef struct {
    runt_cell *cells;
    runt_uint size;
    runt_uint used;
} runt_cell_pool;

struct runt_vm {
    runt_stack stack;
    runt_cell_pool cell_pool;
    runt_memory_pool memory_pool;
    runt_uint status;
    runt_cell *proc;

    runt_proc zproc;
    runt_ptr nil;
};

/* Main */
runt_int runt_init(runt_vm *vm);

/* Pools */

runt_int runt_cell_pool_set(runt_vm *vm, runt_cell *cells, runt_uint size);
runt_int runt_cell_pool_init(runt_vm *vm);

runt_int runt_memory_pool_set(runt_vm *vm, unsigned char *buf, runt_uint size);
runt_uint runt_malloc(runt_vm *vm, size_t size, void **ud);

/* Cell Operations */

runt_uint runt_new_cell(runt_vm *vm, runt_cell **cell);
runt_int runt_link_cell(runt_vm *vm, runt_cell *src, runt_cell *dest);
runt_int runt_bind(runt_vm *vm, runt_cell *cell, runt_proc proc);
runt_int runt_call(runt_vm *vm, runt_cell *cell);
runt_int runt_exec(runt_vm *vm, runt_cell *cell);

/* Stack Operations */

runt_stacklet * runt_pop(runt_vm *vm);
runt_stacklet * runt_push(runt_vm *vm);

/* Pointers */

runt_ptr runt_mk_ptr(runt_type type, void *ud);
runt_int runt_ref_to_cptr(runt_vm *vm, runt_uint ref, void **ud);
runt_cell * runt_to_cell(runt_ptr p);
runt_float * runt_to_float(runt_ptr p);
runt_ptr runt_mk_float(runt_vm *vm, runt_float ival);
const char * runt_to_string(runt_ptr p);
runt_ptr runt_mk_string(runt_vm *vm, const char *str, runt_uint size);

/* Dictionary */

runt_uint runt_entry_create(runt_vm *vm, 
        runt_cell *cell, 
        runt_proc copy, 
        runt_entry **entry);

runt_int runt_word(runt_vm *vm, 
        const char *name, 
        runt_int size,
        runt_entry *entry);

/* Lexing and Parsing */

runt_int runt_compile(runt_vm *vm, const char *str);

runt_type runt_lex(runt_vm *vm, 
        const char *str,
        runt_int size,
        runt_int *post);

/* Procedures */

runt_int runt_proc_begin(runt_vm *vm, runt_cell *proc);
runt_int runt_proc_end(runt_vm *vm);

#endif
