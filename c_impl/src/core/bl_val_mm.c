#include <bearlang/bearlang.h>

#include <malloc.h>

bl_val_t* bl_val_alloc() {
       bl_val_t* retval = calloc(sizeof(bl_val_t),1);
       return bl_val_ref(retval);
}

bl_val_t* bl_val_free(bl_val_t* v) {
       if(v==NULL) return;
       v->refs--;
       if(v->refs > 0) return;
       switch(v->type) {
          case VAL_TYPE_NIL:
          break;
          case VAL_TYPE_SYMBOL:
               free(v->sym_name);
          break;
          case VAL_TYPE_STR:
               free(v->str_val);
          break;
          case VAL_TYPE_CONS:
               bl_val_free(v->car);
               bl_val_free(v->cdr);
          break;
          case VAL_TYPE_INT:
          break;
          case VAL_TYPE_FUNC_BL:
               bl_val_free(v->fn_args);
               bl_val_free(v->fn_body);
          break;
          case VAL_TYPE_FUNC_NATIVE:
          break;
          case VAL_TYPE_OPER_NATIVE:
          break;
          case VAL_TYPE_ENV:
               bl_val_free(v->env_parent);
               bl_val_free(v->env_contents);
          break;
       }
       free(v);
}

bl_val_t* bl_val_copy(bl_val_t* v) {
       if(v==NULL) return NULL;
       switch(v->type) {
          case VAL_TYPE_NIL:
          break;
          case VAL_TYPE_SYMBOL:
               return bl_mk_symbol(v->sym_name);
          break;
          case VAL_TYPE_STR:
               return bl_mk_str(v->str_val);
          break;
          case VAL_TYPE_CONS:
               return bl_mk_cons(v->car,v->cdr);
          break;
          case VAL_TYPE_INT:
               return bl_mk_int(v->int_val);
          break;
          case VAL_TYPE_FUNC_BL:
               return bl_mk_fn_bl(v->fn_args, v->fn_body);
          break;
          case VAL_TYPE_FUNC_NATIVE:
               return bl_mk_fn_native(v->fn_native_code);
          break;
          case VAL_TYPE_OPER_NATIVE:
               return bl_mk_oper_native(v->fn_native_code);
          break;
          case VAL_TYPE_ENV:{
               bl_val_t* retval = bl_mk_env(v->env_parent);
               bl_list_append(retval->env_contents,bl_val_copy(v->env_contents));
               return retval;
          }
          break;
       }
}

bl_val_t* bl_val_ref(bl_val_t* v) {
       if(v==NULL) return NULL;
       v->refs++;
       return v;
}


