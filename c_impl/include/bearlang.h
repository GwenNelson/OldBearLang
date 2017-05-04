#pragma once

// ==== DATA TYPES ====

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

#define true 1
#define false 0
typedef int bool;

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

// ==== MEMORY MANAGEMENT ====
bl_val_t* bl_val_alloc();           // allocates memory for a value and returns pointer to it, with a single reference
bl_val_t* bl_val_free(bl_val_t* v);    // decrements reference counter for v and frees the memory if that was the last reference
bl_val_t* bl_val_copy(bl_val_t* v);    // creates a new value with a copy of the old value with a single reference
bl_val_t* bl_val_ref(bl_val_t* v);     // increments reference counter for v and returns a pointer to it

// ==== ENVIRONMENTS ====
// environments at present are just lists of (k,v) pairs where k is a VAL_TYPE_SYMBOL and v is an arbitrary type
bl_val_t* bl_mk_env(bl_val_t* parent);                         // create a new environment, pass NULL if this is the topmost environment
bl_val_t* bl_env_read(bl_val_t* env, bl_val_t* sym);           // obtain the value of a symbol from the environment, if not found in this environment or any parent, returns NULL
bl_val_t* bl_env_set(bl_val_t* env, bl_val_t* k, bl_val_t* v); // set the value of a symbol in an environment, returns the same value

// ==== VALUE CONSTRUCTION ====
bl_val_t* bl_mk_cons(bl_val_t* car, bl_val_t* cdr);             // creates a new cons value and returns a pointer to it with a single reference, car and cdr are reference incremented
bl_val_t* bl_mk_int(int i);                               // creates an integer value and returns a pointer to it with a single reference
bl_val_t* bl_mk_symbol(char* sym_name);                   // creates a symbol and returns a pointer to it with a single reference
bl_val_t* bl_mk_str(char* s);                             // creates a string and returns a pointer to it with a single reference
bl_val_t* bl_mk_fn_native(bl_native_fn_t f);              // creates a function using native code and returns a pointer to it with a single reference
bl_val_t* bl_mk_fn_bl(bl_val_t* args, bl_val_t* body);    // creates a function using BearLang code, args and body are reference incremented, returns pointer with single reference

// ==== SYMBOL OPERATIONS ====
bool bl_symbol_eq(bl_val_t* a, bl_val_t* b); // returns true if the two symbols are the same - NOT their values

// ==== LIST OPERATIONS ====
bl_val_t* bl_list_car(bl_val_t* l);              // returns the car of the list without incrementing the reference counter
bl_val_t* bl_list_cdr(bl_val_t* l);              // returns the cdr of the list without incrementing the reference counter
bl_val_t* bl_list_append(bl_val_t* l, bl_val_t* v); // appends v to the end of l, increments ref counter for v and returns l
bl_val_t* bl_list_last(bl_val_t* l);             // returns the last value in the list without incrementing the reference counter

// ==== BUILTIN FUNCTIONS ====
bl_val_t* bl_builtin_add(bl_val_t* env, bl_val_t* args); // sums the args and returns the result as a new int val

// ==== EVALUATION ====
bl_val_t* bl_eval_expr(bl_val_t* env, bl_val_t* expr); // evalutes expr and returns the result of evaluation as a new value with a single reference

// ==== TOOLS AND MISC ====
void bl_dump_expr(bl_val_t* expr); // dumps an expression to stdout
