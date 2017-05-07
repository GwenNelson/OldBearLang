#include <bearlang/bearlang.h>

#include <gc.h>

bl_val_t* bl_val_alloc() {
       bl_val_t* retval   = GC_MALLOC(sizeof(bl_val_t));
       retval->alloc_type = VAL_ALLOC_DYNAMIC_SINGLE;
       return bl_val_ref(retval);
}

bl_val_t* bl_val_alloc_block(int n) {
       bl_val_t* retval  = GC_MALLOC(sizeof(bl_val_t)*n);
       retval->block_len = n;
       int i=0;
       for(i=0; i<n; i++) {
           retval[i].alloc_type = VAL_ALLOC_DYNAMIC_BLOCK;
           retval[i].head_block = retval;
       }
       return retval;
}

bl_val_t* bl_val_alloc_static(bl_val_t* v) {
       if(v==NULL) return NULL;
       v->alloc_type = VAL_ALLOC_STATIC;
       return v;
}

bl_val_t* bl_val_alloc_block_static(bl_val_t* v, int n) {
       if(v==NULL) return NULL;
       v->alloc_type = VAL_ALLOC_STATIC;
       v->block_len  = n;
       int i=0;
       for(i=0; i<n; i++) {
           v[i].alloc_type = VAL_ALLOC_STATIC;
           v[i].head_block = v;
       }
       return v;
}

bl_val_t* bl_val_free(bl_val_t* v) {
       if(v==NULL) return;
       if(v->alloc_type == VAL_ALLOC_STATIC) return;
       v->refs--;
       if(v->refs >0) return;

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
          case VAL_TYPE_BOOL:
          break;
          case VAL_TYPE_FUNC_BL:
               bl_val_free(v->fn_args);
               bl_val_free(v->fn_body);
          break;
          case VAL_TYPE_FUNC_NATIVE:
          break;
          case VAL_TYPE_OPER_NATIVE:
          break;
          case VAL_TYPE_HASHMAP:
               bl_val_free(v->hashmap_buckets);
               GC_FREE(v->hashmap_buckets);
          break;
          case VAL_TYPE_ENV:
               bl_val_free(v->env_parent);
               bl_val_free(v->env_contents);
          break;
       }
       switch(v->alloc_type) {
          case VAL_ALLOC_DYNAMIC_BLOCK:
               v->head_block->refs--;
               if(v->head_block->refs ==0) GC_free(v->head_block);
          break;
          case VAL_ALLOC_DYNAMIC_SINGLE:
               GC_free(v);
          break;
       }
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
          case VAL_TYPE_BOOL:
               return bl_mk_bool(v->bool_val);
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
       if(v->alloc_type == VAL_ALLOC_STATIC) return v;
       if(v->alloc_type == VAL_ALLOC_DYNAMIC_BLOCK) v->head_block->refs++;
       v->refs++;
       return v;
}


