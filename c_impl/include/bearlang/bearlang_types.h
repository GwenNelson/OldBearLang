#pragma once

#include <stdbool.h>

typedef enum bl_val_type_t {
    VAL_TYPE_NIL,
    VAL_TYPE_SYMBOL,
    VAL_TYPE_STR,
    VAL_TYPE_CONS,
    VAL_TYPE_INT,
    VAL_TYPE_BOOL,
    VAL_TYPE_FUNC_BL,
    VAL_TYPE_FUNC_NATIVE,
    VAL_TYPE_OPER_NATIVE,
    VAL_TYPE_ENV,
} bl_val_type_t;

typedef enum bl_val_alloc_type_t {
    VAL_ALLOC_DYNAMIC_SINGLE, // single value allocated using system malloc() and free()
    VAL_ALLOC_DYNAMIC_BLOCK,  // blocks of values allocated using system malloc() and free()
    VAL_ALLOC_STATIC,         // static reference - will never be freed
} bl_val_alloc_type_t;

typedef struct bl_val_t bl_val_t;
typedef bl_val_t* (*bl_native_fn_t)(bl_val_t* env, bl_val_t* args); // pointer to a native function for use in fn values

typedef struct bl_val_t {
    bl_val_type_t type;

    // memory management stuff
    bl_val_alloc_type_t alloc_type; // what type of memory allocation was used
    bl_val_t* head_block;           // if part of a block, this points to the block head
    int block_len;                  // how many elements in this block

    // simple ref counting
    int refs;

    // for int
    int int_val;

    // for boolean
    bool bool_val;

    // for string
    char* str_val;

    // for symbols
    char* sym_name;

    // for functions
    bl_val_t* fn_args; // list of symbols defining the argument symbols
    bl_val_t* fn_body;      // expression to evaluate when function is called and native_func is NULL

    // for native code functions
    bl_native_fn_t fn_native_code;

    // for environments
    bl_val_t* env_parent;
    bl_val_t* env_contents;

    // for cons
    bl_val_t* car;
    bl_val_t* cdr;
} bl_val_t;

