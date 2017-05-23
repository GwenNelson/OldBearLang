#include <bearlang/bearlang.h>

bl_val_t* bl_mk_env(bl_val_t* parent) {
       bl_val_t* retval     = bl_val_alloc();
       retval->type         = VAL_TYPE_ENV;
       retval->env_parent   = bl_val_ref(parent);
       retval->env_contents = bl_mk_hashmap(8); // this should be enough for most purposes
       return retval;
}

bl_val_t* bl_env_read(bl_val_t* env, bl_val_t* sym) {
      bl_val_t* retval = bl_hashmap_get(env->env_contents,sym);
      if(retval != NULL) return retval;
      if(env->env_parent != NULL) return bl_env_read(env->env_parent, sym);
      return NULL;
}

bl_val_t* bl_env_set(bl_val_t* env, bl_val_t* k, bl_val_t* v) {
       bl_hashmap_set(env->env_contents,k,v);
       return v;      
}

