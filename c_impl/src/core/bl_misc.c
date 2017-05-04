#include <bearlang/bearlang.h>

#include <malloc.h>
#include <string.h>
#include <stdio.h>


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
          case VAL_TYPE_OPER_NATIVE:
               printf("<oper-native>");
          break;
          case VAL_TYPE_FUNC_NATIVE:
               printf("<function-native>"); // as above, but make it contain a memory address or similar
          break;
          case VAL_TYPE_ENV:
               printf("<env>");
          break;
      }

}
