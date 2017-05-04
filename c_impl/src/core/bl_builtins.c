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