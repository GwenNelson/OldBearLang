#include <bearlang/bearlang.h>


bl_val_t* bl_builtin_add(bl_val_t* env, bl_val_t* args) {
      // TODO - add support for strings etc
      int retval=0;
      bl_val_t* c = args;
      while(c != NULL) {
         retval += bl_list_car(c)->int_val; // TODO - add special functions for conversion to/from C types and make them handle errors
         c = bl_list_cdr(c);
      }
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
