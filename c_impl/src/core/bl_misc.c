#include <bearlang/bearlang.h>

#include <malloc.h>
#include <string.h>
#include <stdio.h>


void bl_dump_expr(bl_val_t* expr) {
     if(expr==NULL) {
        printf("nil");
        return;
     }
     switch(expr->type) {
          case VAL_TYPE_NIL:
               printf("nil");
          break;
          case VAL_TYPE_STR:
               printf("\"%s\"",expr->str_val);
          break;
          case VAL_TYPE_INT:
               printf("%d",expr->int_val);
          break;
          case VAL_TYPE_SYMBOL:
               printf("%s",expr->sym_name);
          break;
          case VAL_TYPE_CONS:{
               printf("(");
               bl_val_t* c = expr;
               while(c != NULL) {
                  if(bl_list_car(c) != NULL) bl_dump_expr(c->car);
                  if(bl_list_cdr(c) != NULL) printf(" ");
                  c = bl_list_cdr(c);
               }
               printf(")");
          }
          break;
          case VAL_TYPE_FUNC_BL:
               printf("<function>"); //TODO - make this print out full definition
          break;
          case VAL_TYPE_OPER_NATIVE:
               printf("<oper-native>");
          break;
          case VAL_TYPE_FUNC_NATIVE:
               printf("<function-native>"); // as above, but make it contain a memory address or similar
          break;
          case VAL_TYPE_ENV:
               printf("<env>");
          break;
      }

}

static bl_val_t default_env_add_symbol = {
    .alloc_type   = VAL_ALLOC_STATIC,
    .type         = VAL_TYPE_SYMBOL,
    .sym_name     = "+",
};

static bl_val_t default_env_set_symbol = {
    .alloc_type   = VAL_ALLOC_STATIC,
    .type         = VAL_TYPE_SYMBOL,
    .sym_name     = "=",
};

static bl_val_t default_env_contents_add_fn = {
    .alloc_type     = VAL_ALLOC_STATIC,
    .type           = VAL_TYPE_FUNC_NATIVE,
    .fn_native_code = bl_builtin_add,
};

static bl_val_t default_env_contents_set_oper = {
    .alloc_type     = VAL_ALLOC_STATIC,
    .type           = VAL_TYPE_OPER_NATIVE,
    .fn_native_code = bl_builtin_set,
};

static bl_val_t default_env_contents_add_fn_cons = {
    .alloc_type     = VAL_ALLOC_STATIC,
    .type           = VAL_TYPE_CONS,
    .car            = &default_env_contents_add_fn,
    .cdr            = NULL,
};

static bl_val_t default_env_contents_set_oper_cons = {
    .alloc_type    = VAL_ALLOC_STATIC,
    .type          = VAL_TYPE_CONS,
    .car           = &default_env_contents_set_oper,
    .cdr           = NULL,
};

static bl_val_t default_env_contents_add_cons = {
    .alloc_type   = VAL_ALLOC_STATIC,
    .type         = VAL_TYPE_CONS,
    .car          = &default_env_add_symbol,
    .cdr          = &default_env_contents_add_fn_cons,
};

static bl_val_t default_env_contents_set_cons = {
    .alloc_type   = VAL_ALLOC_STATIC,
    .type         = VAL_TYPE_CONS,
    .car          = &default_env_set_symbol,
    .cdr          = &default_env_contents_set_oper_cons,
};

static bl_val_t default_env_contents_list[];
static bl_val_t default_env_contents_list[] = {
    { .alloc_type   = VAL_ALLOC_STATIC,
      .type         = VAL_TYPE_CONS,
      .car          = &default_env_contents_add_cons,
      .cdr          = &default_env_contents_list[1],},
    { .alloc_type   = VAL_ALLOC_STATIC,
      .type         = VAL_TYPE_CONS,
      .car          = &default_env_contents_set_cons,
      .cdr          = NULL,},
};

static bl_val_t default_env = {
    .alloc_type   = VAL_ALLOC_STATIC,
    .type         = VAL_TYPE_ENV,
    .env_parent   = NULL,
    .env_contents = default_env_contents_list,
};

bl_val_t* bl_init_env() {
//    bl_val_t* retval = bl_mk_env(&default_env);
//    retval->contents = bl_val_alloc_block_static(bl_val_alloc_block(3));
//    retval->contents = bl_mk_static_list(retval->contents,default_env_contents);
    bl_dump_expr(default_env.env_contents);
    return &default_env;
//    bl_env_set(retval,bl_mk_symbol("+"),bl_mk_fn_native(bl_builtin_add));

//    bl_env_set(retval,bl_mk_symbol("="),bl_mk_oper_native(bl_builtin_set));
//    bl_env_set(retval,bl_mk_symbol("fn"),bl_mk_oper_native(bl_builtin_fn));

//    return retval;
}
