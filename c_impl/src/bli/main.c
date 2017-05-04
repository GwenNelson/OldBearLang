#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <readline/readline.h>

#include <mpc.h>

#include <bearlang/bearlang.h>

bl_val_t* repl_env;

bl_val_t* parse_ast(mpc_ast_t *t) {
       int i=0;
       if(strstr(t->tag,"number")) return bl_mk_int(atoi(t->contents));
       if(strstr(t->tag,"symbol")) return bl_mk_symbol(t->contents);
       if(strcmp(t->tag,">")==0 || strstr(t->tag,"sexpr")) {

          bl_val_t* retval = bl_mk_cons(parse_ast(t->children[1]),NULL);
          for(i=2; i < t->children_num-1; i++) {
              bl_list_append(retval, parse_ast(t->children[i]));
          }
          return retval;
       }
       return NULL;
}

void handle_line(char* l) {
     char* copy = strdup(l);

     mpc_parser_t* Number = mpc_new("number");
     mpc_parser_t* Symbol = mpc_new("symbol");
     mpc_parser_t* Sexpr  = mpc_new("sexpr");
     mpc_parser_t* String = mpc_new("string");
     mpc_parser_t* Expr   = mpc_new("expr");
     mpc_parser_t* Lispy  = mpc_new("lispy");

     mpca_lang(MPCA_LANG_DEFAULT,
      "                                          \
        number : /-?[0-9]+/ ;                    \
        symbol : /[a-zA-Z0-9_+\\-*\\/\\\\=<>!&]+/ ;         \
        sexpr  : '(' <expr>* ')' ;               \
        string : /\"(\\\\.|[^\"])*\"/ ;         \
        expr   : <number> | <string> | <symbol> | <sexpr> ; \
        lispy  : /^/ <expr>* /$/ ;               \
      ", Number, Symbol, Sexpr, String, Expr, Lispy);


    mpc_result_t r;
    if (mpc_parse("<stdin>", copy, Lispy, &r)) {
      /* On success print and delete the AST */
//      mpc_ast_print(r.output);
      
      mpc_ast_t* t = r.output;
      bl_val_t* parsed_val = parse_ast(t);
      bl_val_t* ret_val    = bl_eval_expr(repl_env,parsed_val->car);
      bl_dump_expr(ret_val);printf("\n");
      bl_val_free(parsed_val);
      bl_val_free(ret_val);
      mpc_ast_delete(r.output);
    } else {
      mpc_err_print(r.error);
      mpc_err_delete(r.error);
    
    }

     free(copy);
}

int main(int argc, char** argv) {
    repl_env = bl_mk_env(NULL);

    bl_env_set(repl_env,bl_mk_symbol("+"),bl_mk_fn_native(bl_builtin_add));

    for(;;) {
       char* input = readline("> ");
       handle_line(input);
       free(input);
    }
}
