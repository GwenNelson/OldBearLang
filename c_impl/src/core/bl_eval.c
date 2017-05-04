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

bl_val_t* bl_eval_expr(bl_val_t* env, bl_val_t* expr) {
      if(expr == NULL) return NULL;
      switch(expr->type) {
          case VAL_TYPE_NIL:
               return NULL;
          break;
          case VAL_TYPE_STR:
               return bl_val_ref(expr);
          break;
          case VAL_TYPE_INT:
               return bl_val_ref(expr);
          break;
          case VAL_TYPE_SYMBOL:
               return bl_val_ref(bl_eval_symbol(env,expr));
          break;
          case VAL_TYPE_CONS:
               if(bl_list_car(expr) == NULL) return NULL;
               if(bl_list_car(expr)->type == VAL_TYPE_SYMBOL) {
                  bl_val_t* sym_val = bl_eval_symbol(env,bl_list_car(expr));
                  if(sym_val != NULL) {
                     if(sym_val->type == VAL_TYPE_FUNC_NATIVE) {
                        return bl_val_ref(bl_eval_native_func(env,sym_val,bl_eval_expr(env,bl_list_cdr(expr))));
                     }
                  }
               } else {
                 return bl_val_ref(bl_mk_cons(bl_eval_expr(env,expr->car),bl_eval_expr(env,expr->cdr)));
               }
          break;
          case VAL_TYPE_FUNC_BL:
               return bl_val_ref(expr);
          break;
          case VAL_TYPE_FUNC_NATIVE:
               return bl_val_ref(expr);
          break;
          case VAL_TYPE_ENV:
               return bl_val_ref(expr);
          break;
      }
}

