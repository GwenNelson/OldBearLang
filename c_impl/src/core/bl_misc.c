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
          case VAL_TYPE_BOOL:
               if(expr->bool_val) {
                  printf("True");
               } else {
                  printf("False");
               }
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


bl_val_t* bl_init_env() {
    bl_val_t* retval = bl_mk_env(NULL);

    // maths functions
    bl_env_set(retval,bl_mk_symbol("+"),bl_mk_fn_native(bl_builtin_add));
    bl_env_set(retval,bl_mk_symbol("-"),bl_mk_fn_native(bl_builtin_sub));
    bl_env_set(retval,bl_mk_symbol("*"),bl_mk_fn_native(bl_builtin_mult));
    bl_env_set(retval,bl_mk_symbol("/"),bl_mk_fn_native(bl_builtin_div));
    bl_env_set(retval,bl_mk_symbol(">"),bl_mk_fn_native(bl_builtin_gt));
    bl_env_set(retval,bl_mk_symbol("<"),bl_mk_fn_native(bl_builtin_lt));

    // conditionals, boolean logic, branching and iteration
    bl_env_set(retval,bl_mk_symbol("if"),bl_mk_oper_native(bl_builtin_if));
    bl_env_set(retval,bl_mk_symbol("eq"),bl_mk_oper_native(bl_builtin_eq));
    bl_env_set(retval,bl_mk_symbol("True"),bl_mk_bool(true));
    bl_env_set(retval,bl_mk_symbol("False"),bl_mk_bool(false));

    // lambda and assignment operators
    bl_env_set(retval,bl_mk_symbol("="), bl_mk_oper_native(bl_builtin_set));
    bl_env_set(retval,bl_mk_symbol("fn"),bl_mk_oper_native(bl_builtin_fn));

    // convenience operators and synactic sugar
    bl_env_set(retval,bl_mk_symbol("fun"),bl_mk_oper_native(bl_builtin_fun));
    bl_env_set(retval,bl_mk_symbol("do"),bl_mk_oper_native(bl_builtin_do));

    // basic console I/O
    bl_env_set(retval,bl_mk_symbol("print"),bl_mk_fn_native(bl_builtin_print));
    return retval;
}
