#include <bearlang/bearlang.h>
#include <string.h>

bool bl_symbol_eq(bl_val_t* a, bl_val_t* b) {
       if(a==b) return true;
       if(a != NULL) {
          if(b != NULL) {
             if(strcmp(a->sym_name,b->sym_name)==0) return true;
             return false;
          }
          return false;
       }
       return false;
}
