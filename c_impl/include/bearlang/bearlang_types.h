#pragma once

typedef enum bl_val_type_t {
    VAL_TYPE_NIL,
    VAL_TYPE_SYMBOL,
    VAL_TYPE_STR,
    VAL_TYPE_CONS,
    VAL_TYPE_INT,
    VAL_TYPE_FUNC_BL,
    VAL_TYPE_FUNC_NATIVE,
    VAL_TYPE_ENV,
} bl_val_type_t;

typedef struct bl_val_t bl_val_t;
typedef bl_val_t* (*bl_native_fn_t)(bl_val_t* env, bl_val_t* args); // pointer to a native function for use in fn values

typedef struct bl_val_t {
    bl_val_type_t type;

    // simple ref counting
    int refs;

    // for int
    int int_val;

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
    bl_val_t* env_contents; // list of (k,v) pairs

    // for cons
    bl_val_t* car;
    bl_val_t* cdr;
} bl_val_t;

