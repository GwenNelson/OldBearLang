#include <bearlang/bearlang.h>

bl_val_t* bl_mk_env(bl_val_t* parent) {
       bl_val_t* retval     = bl_val_alloc();
       retval->type         = VAL_TYPE_ENV;
       retval->env_parent   = bl_val_ref(parent);
       retval->env_contents = bl_mk_cons(NULL,NULL);
       return retval;
}

bl_val_t* bl_env_read(bl_val_t* env, bl_val_t* sym) {
      bl_val_t* c = env->env_contents;

      while(c != NULL) {
         if(c != NULL) {
            if(bl_symbol_eq(sym,bl_list_car(bl_list_car(c)))) {
               return bl_list_car(bl_list_cdr(bl_list_car(c)));
            }
         }
         c = bl_list_cdr(c);
      }
      if(env->env_parent != NULL) return bl_env_read(env->env_parent, sym);
      return NULL;
}

bl_val_t* bl_env_set(bl_val_t* env, bl_val_t* k, bl_val_t* v) {
       // TODO - look for already existing value here and throw an error
       bl_list_append(env->env_contents,bl_mk_cons(k,bl_mk_cons(v,NULL)));
       return v;      
}

