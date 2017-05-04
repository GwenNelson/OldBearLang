#include <bearlang/bearlang.h>

bl_val_t* bl_eval_symbol(bl_val_t* env, bl_val_t* s) {
      bl_val_t* retval = bl_val_copy(bl_env_read(env, s)); // copy rather than ref so that bl_val_free(retval) doesn't alter env
      return retval;
}

bl_val_t* bl_eval_native_func(bl_val_t* env, bl_val_t* func, bl_val_t* params) {
      bl_val_t* func_closure = bl_mk_env(env);
      bl_val_t* retval       = func->fn_native_code(func_closure,params);
      bl_val_free(func_closure);
      return retval;
}

bl_val_t* bl_eval_native_oper(bl_val_t* env, bl_val_t* oper, bl_val_t* params) {
      bl_val_t* retval       = oper->fn_native_code(env,params);
      return retval;
}

bl_val_t* bl_eval_bl_func(bl_val_t* env, bl_val_t* func, bl_val_t* params) {
      bl_val_t* func_closure  = bl_mk_env(env);
      bl_val_t* func_arg_syms = func->fn_args;
      bl_val_t* func_arg_vals = params;

      bl_val_t* cur_sym = func_arg_syms;
      bl_val_t* cur_val = func_arg_vals;
      while(cur_val != NULL) {
         // TODO: handle invalid argument number
         bl_env_set(func_closure,bl_list_car(cur_sym),bl_list_car(cur_val));
         cur_sym = bl_list_cdr(cur_sym);
         cur_val = bl_list_cdr(cur_val);
      }

      bl_val_t* cur_line = func->fn_body;
      bl_val_t* retval = NULL;
      while(cur_line != NULL) {
         retval   = bl_eval_expr(func_closure,bl_list_car(cur_line));
         cur_line = bl_list_cdr(cur_line);
      }

      bl_val_free(func_closure);
      return retval;
}

bl_val_t* bl_eval_expr(bl_val_t* env, bl_val_t* expr) {
      if(expr == NULL) return NULL;
      switch(expr->type) {
          case VAL_TYPE_NIL:
               return NULL;
          break;
          case VAL_TYPE_STR:
               return expr;
          break;
          case VAL_TYPE_INT:
               return expr;
          break;
          case VAL_TYPE_SYMBOL:
               return bl_eval_symbol(env,expr);
          break;
          case VAL_TYPE_CONS:
               if(bl_list_car(expr) == NULL) return NULL;
               if(bl_list_car(expr)->type == VAL_TYPE_SYMBOL) {
                  bl_val_t* sym_val = bl_eval_symbol(env,bl_list_car(expr));
                  if(sym_val != NULL) {
                     if(sym_val->type == VAL_TYPE_FUNC_NATIVE) {
                        return bl_eval_native_func(env,sym_val,bl_eval_expr(env,bl_list_cdr(expr)));
                     }
                     if(sym_val->type == VAL_TYPE_OPER_NATIVE) {
                        return bl_eval_native_oper(env,sym_val,bl_list_cdr(expr));
                     }
                     if(sym_val->type == VAL_TYPE_FUNC_BL) {
                        return bl_eval_bl_func(env, sym_val, bl_list_cdr(expr));
                     }
                     return bl_mk_cons(sym_val,bl_eval_expr(env,expr->cdr));
                  }
               } 
               if(bl_list_car(expr)->type == VAL_TYPE_CONS) {
                 return bl_mk_cons(bl_eval_expr(env,expr->car),bl_eval_expr(env,expr->cdr));
               }
               return bl_mk_cons(expr->car,bl_eval_expr(env,expr->cdr));
          break;
          case VAL_TYPE_FUNC_BL:
               return expr;
          break;
          case VAL_TYPE_FUNC_NATIVE:
               return expr;
          break;
          case VAL_TYPE_ENV:
               return expr;
          break;
      }
}

