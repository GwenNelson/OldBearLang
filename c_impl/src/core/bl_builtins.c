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

bl_val_t* bl_builtin_do(bl_val_t* env, bl_val_t* args) {
      bl_val_t* c      = args;
      bl_val_t* retval = NULL;
      while(c != NULL) {
         bl_val_t* car_retval          = bl_eval_expr(env,bl_list_car(c));
         if(car_retval != NULL) retval = car_retval;
         c = bl_list_cdr(c);
      }
      return retval;
}

bl_val_t* bl_builtin_if(bl_val_t* env, bl_val_t* args) {
      bl_val_t* cond         = bl_eval_expr(env,bl_list_car(args));
      bl_val_t* then_clause  = bl_list_car(bl_list_cdr(args));
      bl_val_t* else_clause  = bl_list_car(bl_list_cdr(bl_list_cdr(args))); // TODO: add first, rest, second, third list ops to make this simpler
      if(cond->bool_val) {
         return bl_eval_expr(env,then_clause);
      } else {
         return bl_eval_expr(env,else_clause);
      }
      return NULL;

}

// TODO - seperate out a function for use within pure C code
bl_val_t* bl_builtin_eq(bl_val_t* env, bl_val_t* args) {
      bl_val_t* a = bl_eval_expr(env,bl_list_car(args));
      bl_val_t* b = bl_eval_expr(env,bl_list_car(bl_list_cdr(args)));
      if(a==b) return bl_mk_bool(true);

      if(a != NULL) {
         if(a->type != b->type) return bl_mk_bool(false);
         switch(a->type) {
            case VAL_TYPE_NIL:
                 if(b->type == VAL_TYPE_NIL) return bl_mk_bool(true);
            break;
            case VAL_TYPE_STR:
                 if(strcmp(a->str_val,b->str_val)==0) return bl_mk_bool(true);
            break;
            case VAL_TYPE_INT:
                 if(a->int_val == b->int_val) return bl_mk_bool(true);
            break;
            case VAL_TYPE_CONS:
                 // TODO - implement this fucker - basically iterate through the fucker and fuckity fuckfuck fuck
            break;
            case VAL_TYPE_FUNC_BL:
                 // TODO - implement this fucker too
            break;
            case VAL_TYPE_OPER_NATIVE:
                 if(a->fn_native_code == b->fn_native_code) return bl_mk_bool(true);
            break;
            case VAL_TYPE_FUNC_NATIVE:
                 if(a->fn_native_code == b->fn_native_code) return bl_mk_bool(true);
            break;
            case VAL_TYPE_ENV:
                 // TODO - implement like the others
            break;
            case VAL_TYPE_BOOL:
                 if(a->bool_val == b->bool_val) return bl_mk_bool(true);
            break;
         }
      }
      return bl_mk_bool(false);
}

bl_val_t* bl_builtin_lt(bl_val_t* env, bl_val_t* args) {
      bl_val_t* a = bl_eval_expr(env,bl_list_car(args));
      bl_val_t* b = bl_eval_expr(env,bl_list_car(bl_list_cdr(args)));
      if(a->int_val < b->int_val) return bl_mk_bool(true);
      return bl_mk_bool(false);
}

bl_val_t* bl_builtin_gt(bl_val_t* env, bl_val_t* args) {
      bl_val_t* a = bl_eval_expr(env,bl_list_car(args));
      bl_val_t* b = bl_eval_expr(env,bl_list_car(bl_list_cdr(args)));
      if(a->int_val > b->int_val) return bl_mk_bool(true);
      return bl_mk_bool(false);
}


void bl_print_expr(bl_val_t* expr) {
     if(expr==NULL) {
        return;
     }
     switch(expr->type) {
          case VAL_TYPE_NIL:
          break;
          case VAL_TYPE_STR:
               printf("%s",expr->str_val);
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

bl_val_t* bl_builtin_print(bl_val_t* env, bl_val_t* args) {
    if(args == NULL) return NULL;
    bl_val_t* c = args;
    while(c != NULL) {
        bl_print_expr(bl_list_car(c));
        c = bl_list_cdr(c);
    }
    return NULL;
}

bl_val_t* bl_builtin_car(bl_val_t* env, bl_val_t* args) {
    if(bl_list_car(args) != NULL) {
       if(bl_list_car(args)->type == VAL_TYPE_CONS) return bl_list_car(bl_list_car(args));
    }
    return bl_list_car(args);
}
