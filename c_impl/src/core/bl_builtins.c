#include <bearlang/bearlang.h>

#include <string.h>

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
