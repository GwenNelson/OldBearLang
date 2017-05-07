#include <bearlang/bearlang.h>

static int bl_hashmap_hash_val(bl_val_t* v, int buckets) {
     if(v == NULL) return 0;
     switch(v->type) {
          case VAL_TYPE_NIL:
               return 0;
          break;
          case VAL_TYPE_SYMBOL:
               return (int)(v->sym_name[0]) % buckets;
          break;
          case VAL_TYPE_STR:
               return (int)(v->str_val[0]) % buckets;
          break;
          case VAL_TYPE_CONS:
               return (bl_hashmap_hash_val(v->car,buckets)*bl_hashmap_hash_val(v->cdr,buckets)) % buckets;
          break;
          case VAL_TYPE_INT:
               return v->int_val % buckets;
          break;
          case VAL_TYPE_BOOL:
               return (int)v->bool_val % buckets;
          break;
          case VAL_TYPE_FUNC_BL:
               return (bl_hashmap_hash_val(v->fn_args,buckets)*bl_hashmap_hash_val(v->fn_body,buckets)) % buckets;
          break;
          case VAL_TYPE_FUNC_NATIVE:
               return ((int)v->fn_native_code) % buckets;
          break;
          case VAL_TYPE_OPER_NATIVE:
               return ((int)v->fn_native_code) % buckets;
          break;
          case VAL_TYPE_ENV:{
               return bl_hashmap_hash_val(v->env_contents,buckets);
          }

     }

}

void bl_hashmap_set(bl_val_t* map, bl_val_t* k, bl_val_t* v) {
     int bucket_index = bl_hashmap_hash_val(k,map->hashmap_bucket_count) % (map->hashmap_bucket_count);
     bl_val_t* c      = &(map->hashmap_buckets[bucket_index]);
     while(c != NULL) {
        if(c != NULL) {
            // TODO - make this support key types other than symbols properly
            if(bl_symbol_eq(k,bl_list_car(bl_list_car(c)))) {
               bl_val_t* old_car = bl_list_car(bl_list_cdr(bl_list_car(c)));
               bl_val_free(old_car);
               bl_list_cdr(bl_list_car(c))->car = v;
               return;
            }

        }
        c = bl_list_cdr(c);
     }
     bl_list_append(&(map->hashmap_buckets[bucket_index]),bl_mk_cons(bl_val_ref(k),bl_val_ref(bl_mk_cons(bl_val_ref(v),NULL))));

}

bl_val_t* bl_hashmap_get(bl_val_t* map, bl_val_t* k) {
     int bucket_index = bl_hashmap_hash_val(k,map->hashmap_bucket_count) % (map->hashmap_bucket_count);
     bl_val_t* c      = &(map->hashmap_buckets[bucket_index]);
     while(c != NULL) {
        if(c != NULL) {
            // TODO - make this support key types other than symbols properly
            if(bl_symbol_eq(k,bl_list_car(bl_list_car(c)))) {
               return bl_list_car(bl_list_cdr(bl_list_car(c)));
            }

        }
        c = bl_list_cdr(c);
     }
     return NULL;
}
