#include <bearlang/bearlang.h>



bl_val_t* bl_list_car(bl_val_t* l) {
       if(l==NULL) return NULL;
       if(l->type == VAL_TYPE_NIL) return NULL;
       // TODO: handle error if type is not a cons/list
       return l->car;
}

bl_val_t* bl_list_cdr(bl_val_t* l) {
       if(l==NULL) return NULL;
       if(l->type == VAL_TYPE_NIL) return NULL;
       // TODO: handle error if type is not a cons/list
       return l->cdr;
}

bl_val_t* bl_list_last(bl_val_t* l) {
      bl_val_t* c = l;
      while(bl_list_cdr(c) != NULL) {
         c = bl_list_cdr(c);
      }
      return c;
}

bl_val_t* bl_list_append(bl_val_t* l, bl_val_t* v) {
      if(bl_list_car(l) == NULL) { 
        l->car = bl_val_ref(v);
        return l;
      }
      bl_list_last(l)->cdr = bl_mk_cons(v, NULL);
      return l;
}


