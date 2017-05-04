#include <bearlang.h>

#include <malloc.h>
#include <string.h>
#include <stdio.h>

bl_val_t* bl_val_alloc() {
       bl_val_t* retval = calloc(sizeof(bl_val_t),1);
       return bl_val_ref(retval);
}

bl_val_t* bl_val_free(bl_val_t* v) {
       if(v==NULL) return;
       v->refs--;
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
               bl_val_free(v->fn_args);
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

bl_val_t* bl_mk_fn_bl(bl_val_t* args, bl_val_t* body) {
       bl_val_t* retval = bl_val_alloc();
       retval->type     = VAL_TYPE_FUNC_BL;
       retval->fn_args  = bl_val_ref(args);
       retval->fn_body  = bl_val_ref(body);
       return retval;
}

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

bl_val_t* bl_eval_symbol(bl_val_t* env, bl_val_t* s) {
      bl_val_t* retval = bl_env_read(env, s);
      return retval;
}

bl_val_t* bl_eval_native_func(bl_val_t* env, bl_val_t* func, bl_val_t* params) {
      bl_val_t* func_closure = bl_mk_env(env);
      bl_val_t* retval       = func->fn_native_code(func_closure,params);
      bl_val_free(func_closure);
      return retval;
}

bl_val_t* bl_eval_expr(bl_val_t* env, bl_val_t* expr) {
      if(expr == NULL) return NULL;
      switch(expr->type) {
          case VAL_TYPE_NIL:
               return NULL;
          break;
          case VAL_TYPE_STR:
               return expr;
          break;
          case VAL_TYPE_INT:
               return expr;
          break;
          case VAL_TYPE_SYMBOL:
               return bl_eval_symbol(env,expr);
          break;
          case VAL_TYPE_CONS:
               if(bl_list_car(expr) == NULL) return NULL;
               if(bl_list_car(expr)->type == VAL_TYPE_SYMBOL) {
                  bl_val_t* sym_val = bl_eval_symbol(env,bl_list_car(expr));
                  if(sym_val != NULL) {
                     if(sym_val->type == VAL_TYPE_FUNC_NATIVE) {
                        return bl_eval_native_func(env,sym_val,bl_eval_expr(env,bl_list_cdr(expr)));
                     }
                  }
               } else {
                 return bl_mk_cons(bl_eval_expr(env,expr->car),bl_eval_expr(env,expr->cdr));
               }
          break;
          case VAL_TYPE_FUNC_BL:
               return expr;
          break;
          case VAL_TYPE_FUNC_NATIVE:
               return expr;
          break;
          case VAL_TYPE_ENV:
               return expr;
          break;
      }
}

void bl_dump_expr(bl_val_t* expr) {
     if(expr==NULL) {
        printf("nil");
        return;
     }
     switch(expr->type) {
          case VAL_TYPE_NIL:
               printf("nil");
          break;
          case VAL_TYPE_STR:
               printf("\"%s\"",expr->str_val);
          break;
          case VAL_TYPE_INT:
               printf("%d",expr->int_val);
          break;
          case VAL_TYPE_SYMBOL:
               printf("%s",expr->sym_name);
          break;
          case VAL_TYPE_CONS:{
               printf("(");
               bl_val_t* c = expr;
               while(c != NULL) {
                  if(bl_list_car(c) != NULL) bl_dump_expr(c->car);
                  if(bl_list_cdr(c) != NULL) printf(" ");
                  c = bl_list_cdr(c);
               }
               printf(")");
          }
          break;
          case VAL_TYPE_FUNC_BL:
               printf("<function>"); //TODO - make this print out full definition
          break;
          case VAL_TYPE_FUNC_NATIVE:
               printf("<function>"); // as above, but make it contain a memory address or similar
          break;
          case VAL_TYPE_ENV:
               printf("<env>");
          break;
      }

}
