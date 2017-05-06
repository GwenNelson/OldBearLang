#include <bearlang/bearlang.h>

#include <string.h>
#include <stdio.h>

bl_val_t* bl_builtin_add(bl_val_t* env, bl_val_t* args) {
      // TODO - add support for (+ "bla bla" 2) and such (add strings and integers together, automatically convert integers to strings)
      if(bl_list_car(args)->type == VAL_TYPE_STR) {
         bl_val_t* retval = bl_mk_str("");
         bl_val_t* c = args;
         while(c != NULL) {
            if(c != NULL) {
               retval->str_val = realloc(retval->str_val,strlen(retval->str_val)+strlen(bl_list_car(c)->str_val));
               strcat(retval->str_val,bl_list_car(c)->str_val);
            }
            c = bl_list_cdr(c);
         }
         return retval;
      }
      int retval=0;
      bl_val_t* c = args;
      while(c != NULL) {
         retval += bl_list_car(c)->int_val; // TODO - add special functions for conversion to/from C types and make them handle errors
         c = bl_list_cdr(c);
      }
      return bl_mk_int(retval);
}

bl_val_t* bl_builtin_sub(bl_val_t* env, bl_val_t* args) {
      bl_val_t* a = bl_list_car(args);
      bl_val_t* b = bl_list_car(bl_list_cdr(args));
      int retval = a->int_val - b->int_val;
      return bl_mk_int(retval);
}

bl_val_t* bl_builtin_mult(bl_val_t* env, bl_val_t* args) {
      bl_val_t* a = bl_list_car(args);
      bl_val_t* b = bl_list_car(bl_list_cdr(args));
      int retval  = a->int_val * b->int_val;
      return bl_mk_int(retval);
}

bl_val_t* bl_builtin_div(bl_val_t* env, bl_val_t* args) {
      bl_val_t* a = bl_list_car(args);
      bl_val_t* b = bl_list_car(bl_list_cdr(args));
      int retval  = a->int_val / b->int_val;
      return bl_mk_int(retval);
}

bl_val_t* bl_builtin_set(bl_val_t* env, bl_val_t* args) {
      bl_val_t* k = bl_val_copy(bl_list_car(args));
      bl_val_t* v = bl_eval_expr(env,(bl_list_car(bl_list_cdr(args))));
      bl_env_set(env,k,v);
      return v;
}

bl_val_t* bl_builtin_fn(bl_val_t* env, bl_val_t* args) {
      bl_val_t* fn_params = bl_val_ref(bl_list_car(args));
      bl_val_t* fn_body   = bl_val_ref(bl_list_cdr(args));
      return bl_mk_fn_bl(fn_params,fn_body);
}

bl_val_t* bl_builtin_fun(bl_val_t* env, bl_val_t* args) {
      bl_val_t* fun_name  = bl_val_copy(bl_list_car(args));
      bl_val_t* fn_params = bl_list_cdr(args);
      bl_val_t* fn_retval = bl_builtin_fn(env,fn_params);
      bl_env_set(env,fun_name,fn_retval);
      return(fn_retval);
}

bl_val_t* bl_builtin_print(bl_val_t* env, bl_val_t* args) {
      bl_val_t* c = args;
      while(c != NULL) {
         if(bl_list_car(c)==NULL) continue;
         switch(bl_list_car(c)->type) {
          case VAL_TYPE_NIL:
               printf("");
          break;
          case VAL_TYPE_STR:
               printf("%s",bl_list_car(c)->str_val);
          break;
          case VAL_TYPE_INT:
               printf("%d",bl_list_car(c)->int_val);
          break;
          case VAL_TYPE_SYMBOL:
               printf("%s",c->sym_name);
          break;
          case VAL_TYPE_CONS:{
               printf("(");
               bl_val_t* inner_c = bl_list_car(c);
               while(inner_c != NULL) {
                  if(bl_list_car(inner_c) != NULL) bl_builtin_print(env,c->car);
                  if(bl_list_cdr(inner_c) != NULL) printf(" ");
                  inner_c = bl_list_cdr(inner_c);
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
      c = bl_list_cdr(c);
    }
    return NULL;
}
