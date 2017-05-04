#include <bearlang/bearlang.h>
#include <string.h>

bl_val_t* bl_mk_cons(bl_val_t* car, bl_val_t* cdr) {
       bl_val_t* retval = bl_val_alloc();
       retval->type  = VAL_TYPE_CONS;
       retval->car   = bl_val_ref(car);
       retval->cdr   = bl_val_ref(cdr);
       return retval;
}

bl_val_t* bl_mk_int(int i) {
       bl_val_t* retval = bl_val_alloc();
       retval->type = VAL_TYPE_INT;
       retval->int_val = i;
       return retval;
}

bl_val_t* bl_mk_symbol(char* sym_name) {
       bl_val_t* retval = bl_val_alloc();
       retval->type     = VAL_TYPE_SYMBOL;
       retval->sym_name = strdup(sym_name);
       return retval;
}

bl_val_t* bl_mk_str(char* s) {
       bl_val_t* retval = bl_val_alloc();
       retval->type = VAL_TYPE_STR;
       retval->str_val = strdup(s);
       return retval;
}

bl_val_t* bl_mk_fn_native(bl_native_fn_t f) {
       bl_val_t* retval = bl_val_alloc();
       retval->type = VAL_TYPE_FUNC_NATIVE;
       retval->fn_native_code = f;
       return retval;
}

bl_val_t* bl_mk_oper_native(bl_native_fn_t f) {
       bl_val_t* retval = bl_val_alloc();
       retval->type = VAL_TYPE_OPER_NATIVE;
       retval->fn_native_code = f;
       return retval;
}

bl_val_t* bl_mk_fn_bl(bl_val_t* args, bl_val_t* body) {
       bl_val_t* retval = bl_val_alloc();
       retval->type     = VAL_TYPE_FUNC_BL;
       retval->fn_args  = bl_val_ref(args);
       retval->fn_body  = bl_val_ref(body);
       return retval;
}


