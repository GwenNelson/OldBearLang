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

#include <bearlang/bearlang_static_macros.h>


static bl_val_t default_env_contents_add_fn    = BL_STATIC_NATIVEFUNC(bl_builtin_add);
static bl_val_t default_env_contents_sub_fn    = BL_STATIC_NATIVEFUNC(bl_builtin_sub);
static bl_val_t default_env_contents_mult_fn   = BL_STATIC_NATIVEFUNC(bl_builtin_mult);
static bl_val_t default_env_contents_div_fn    = BL_STATIC_NATIVEFUNC(bl_builtin_div);
static bl_val_t default_env_contents_set_oper  = BL_STATIC_NATIVEOPER(bl_builtin_set);
static bl_val_t default_env_contents_fn_oper   = BL_STATIC_NATIVEOPER(bl_builtin_fn);
static bl_val_t default_env_contents_print_fn  = BL_STATIC_NATIVEFUNC(bl_builtin_print);
static bl_val_t default_env_contents_fun_oper  = BL_STATIC_NATIVEOPER(bl_builtin_fun);

#define DEFAULT_ENV_CONTENTS \
        X("+",    default_env_contents_add_fn,   default_env_contents, 0) \
        X("-",    default_env_contents_sub_fn,   default_env_contents, 1) \
        X("*",    default_env_contents_mult_fn,  default_env_contents, 2) \
        X("/",    default_env_contents_div_fn,   default_env_contents, 3) \
        X("=",    default_env_contents_set_oper, default_env_contents, 4) \
        X("fn",   default_env_contents_fn_oper,  default_env_contents, 5) \
        X("print",default_env_contents_print_fn, default_env_contents, 6) \
        X("fun",  default_env_contents_fun_oper, default_env_contents, 7)

BL_ASSOC_VAL_START(default_env_contents)
#define X BL_ASSOC_VAL_ENTRY
DEFAULT_ENV_CONTENTS
BL_ASSOC_VAL_END
#undef X

BL_ASSOC_KEY_START(default_env_contents)
#define X BL_ASSOC_KEY_ENTRY
DEFAULT_ENV_CONTENTS
BL_ASSOC_KEY_END
#undef X


BL_ASSOC_ITEMS_START(default_env_contents)
#define X BL_ASSOC_ITEMS_ENTRY
DEFAULT_ENV_CONTENTS
BL_ASSOC_ITEMS_END
#undef X

static bl_val_t default_env_contents_list[];
static bl_val_t default_env_contents_list[] = {
    BL_STATIC_LIST_CONS(default_env_contents,0),
    BL_STATIC_LIST_CONS(default_env_contents,1),
    BL_STATIC_LIST_CONS(default_env_contents,2),
    BL_STATIC_LIST_CONS(default_env_contents,3),
    BL_STATIC_LIST_CONS(default_env_contents,4),
    BL_STATIC_LIST_CONS(default_env_contents,5),
    BL_STATIC_LIST_CONS(default_env_contents,6),
    BL_STATIC_LIST_CONS_LAST(default_env_contents,7)
};

static bl_val_t default_env = {
    .alloc_type   = VAL_ALLOC_STATIC,
    .type         = VAL_TYPE_ENV,
    .env_parent   = NULL,
    .env_contents = default_env_contents_list,
};

bl_val_t* bl_init_env() {
    return &default_env;
}
