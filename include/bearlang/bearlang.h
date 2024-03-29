#pragma once

#include <bearlang/bearlang_types.h>
#include <stdbool.h>
#include <stdlib.h>

// ==== MEMORY MANAGEMENT ====
// implemented in bl_val_mm.c
// all values used within BearLang should be handled using these functions and not via directly calling malloc() and free()
bl_val_t* bl_val_alloc();                   // allocates memory for a value and returns pointer to it, with a single reference
bl_val_t* bl_val_alloc_block(int n);        // allocates memory for n values and returns a pointer to the head of the block - this does NOT init reference counts etc, only allocates the block

bl_val_t* bl_val_alloc_block_static(bl_val_t* v, int n); // allocates a static block

bl_val_t* bl_val_alloc_static(bl_val_t* v); // creates a static reference pointer to v and returns it - this means that v will NEVER be freed, use with caution
                                            // note that this does not do any init or setup either, it is intended for use in compiled code

bl_val_t* bl_val_free(bl_val_t* v);  // decrements reference counter for v and frees the memory if that was the last reference
bl_val_t* bl_val_copy(bl_val_t* v);  // creates a new value with a copy of the old value with a single reference
bl_val_t* bl_val_ref(bl_val_t* v);   // increments reference counter for v and returns a pointer to it

// ==== ENVIRONMENTS ====
// implemented in bl_env.c
// environments at present are just lists of (k,v) pairs where k is a VAL_TYPE_SYMBOL and v is an arbitrary type
bl_val_t* bl_mk_env(bl_val_t* parent);                         // create a new environment, pass NULL if this is the topmost environment
bl_val_t* bl_env_read(bl_val_t* env, bl_val_t* sym);           // obtain the value of a symbol from the environment, if not found in this environment or any parent, returns NULL
bl_val_t* bl_env_set(bl_val_t* env, bl_val_t* k, bl_val_t* v); // set the value of a symbol in an environment, returns the same value

// ==== VALUE CONSTRUCTION ====
// implemented in bl_mk.c
// these functions should be used for creating new values
bl_val_t* bl_mk_cons(bl_val_t* car, bl_val_t* cdr);       // creates a new cons value and returns a pointer to it with a single reference, car and cdr are reference incremented
bl_val_t* bl_mk_static_list(bl_val_t* b, bl_val_t* l);    // creates a static ref list starting in the block specified by b and populated with items from the array in l, use only on static values
bl_val_t* bl_mk_int(int i);                               // creates an integer value and returns a pointer to it with a single reference
bl_val_t* bl_mk_symbol(char* sym_name);                   // creates a symbol and returns a pointer to it with a single reference
bl_val_t* bl_mk_str(char* s);                             // creates a string and returns a pointer to it with a single reference
bl_val_t* bl_mk_fn_native(bl_native_fn_t f);              // creates a function using native code and returns a pointer to it with a single reference
bl_val_t* bl_mk_oper_native(bl_native_fn_t f);            // creates an operator using native code and returns a pointer to it with a single reference
bl_val_t* bl_mk_fn_bl(bl_val_t* args, bl_val_t* body);    // creates a function using BearLang code, args and body are reference incremented, returns pointer with single reference
bl_val_t* bl_mk_bool(bool b);                             // creates a boolean value
bl_val_t* bl_mk_hashmap(int bucket_count);                // creates a hashmap

// ==== HASHMAP OPERATIONS ====
// implemented in bl_hashmap_ops.c
void      bl_hashmap_set(bl_val_t* map, bl_val_t* k, bl_val_t* v);  // sets a hashmap value, this will overwrite any previous value
bl_val_t* bl_hashmap_get(bl_val_t* map, bl_val_t* k);               // gets a hashmap value, returns NULL if not found

// ==== SYMBOL OPERATIONS ====
// implemented in bl_symbol_ops.c
// use these functions when working with symbols (duh)
bool bl_symbol_eq(bl_val_t* a, bl_val_t* b); // returns true if the two symbols are the same - NOT their values

// ==== LIST OPERATIONS ====
// implemented in bl_list_ops.c
// use these functions when working with lists and cons cells (they're essentially the same)
bl_val_t* bl_list_car(bl_val_t* l);                 // returns the car of the list without incrementing the reference counter
bl_val_t* bl_list_cdr(bl_val_t* l);                 // returns the cdr of the list without incrementing the reference counter
bl_val_t* bl_list_append(bl_val_t* l, bl_val_t* v); // appends v to the end of l, increments ref counter for v and returns l
bl_val_t* bl_list_last(bl_val_t* l);                // returns the last value in the list without incrementing the reference counter

// ==== BUILTIN FUNCTIONS ====
// implemented in bl_builtins.c
// these should be added to your root environment to get a usable language
bl_val_t* bl_builtin_add(bl_val_t*   env, bl_val_t* args); // sums the args and returns the result as a new int val OR concatenates strings
bl_val_t* bl_builtin_sub(bl_val_t*   env, bl_val_t* args); // takes 2 args (a b) and returns a-b
bl_val_t* bl_builtin_mult(bl_val_t*  env, bl_val_t* args); // takes 2 args (a b) and returns a*b
bl_val_t* bl_builtin_div(bl_val_t*   env, bl_val_t* args); // takes 2 args (a b) and returns a/b
bl_val_t* bl_builtin_print(bl_val_t* env, bl_val_t* args); // prints the arguments to stdout
bl_val_t* bl_builtin_lt(bl_val_t*    env, bl_val_t* args); // takes 2 int args and returns boolean true if a<b
bl_val_t* bl_builtin_gt(bl_val_t*    env, bl_val_t* args); // takes 2 int args and returns boolean true if a>b

// ==== BUILTIN OPERATORS ====
// implemented in bl_builtins.c
bl_val_t* bl_builtin_set(bl_val_t* env, bl_val_t* args); // sets a variable in the environment and returns the new value
bl_val_t* bl_builtin_fn(bl_val_t*  env, bl_val_t* args); // BearLang's lambda - returns a new function
bl_val_t* bl_builtin_fun(bl_val_t* env, bl_val_t* args); // BearLang's defun - sets variable in environment to new function and returns that function
bl_val_t* bl_builtin_do(bl_val_t*  env, bl_val_t* args); // evaluates all the arguments and returns result of last one
bl_val_t* bl_builtin_if(bl_val_t*  env, bl_val_t* args); // implements (if cond then-action else-action) ;)
bl_val_t* bl_builtin_eq(bl_val_t*  env, bl_val_t* args); // implements eq operator - evaluates arguments and returns a boolean if they're equal
bl_val_t* bl_builtin_car(bl_val_t* env, bl_val_t* args); // implements car operator

// ==== EVALUATION ====
// implemented in bl_eval.c
// where the magic happens ;)
bl_val_t* bl_eval_symbol(bl_val_t* env, bl_val_t* s);                                // returns either the value of the symbol s or NULL if not found
bl_val_t* bl_eval_native_func(bl_val_t* env, bl_val_t* func, bl_val_t* params);      // evaluates the native code function specified by func against eval(params) and returns the result
bl_val_t* bl_eval_bl_func(bl_val_t* env, bl_val_t* func, bl_val_t* params);          // evaluates the specified BearLang function against eval(params) and returns the result
bl_val_t* bl_eval_native_oper(bl_val_t* env, bl_val_t* oper_sym, bl_val_t* params);  // evaluates the specified builtin operator against the params and returns the result
bl_val_t* bl_eval_expr(bl_val_t* env, bl_val_t* expr);                               // evalutes expr and returns the result of evaluation as a new value with a single reference

// ==== TOOLS AND MISC ====
// implemented in bl_misc.c
// misc stuff that doesn't fit elsewhere
bl_val_t* bl_init_env();           // returns a default sane environment intended for use as the root env - this is implemented as a static ref
void bl_dump_expr(bl_val_t* expr); // dumps an expression to stdout
