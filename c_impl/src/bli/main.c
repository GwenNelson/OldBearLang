#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include <readline/readline.h>

#include <mpc.h>

#include <bearlang/bearlang.h>

bl_val_t* repl_env;

bl_val_t* parse_ast(mpc_ast_t *t) {
       int i=0;
       if(strstr(t->tag,"number")) return bl_mk_int(atoi(t->contents));
       if(strstr(t->tag,"symbol")) return bl_mk_symbol(t->contents);
       if(strcmp(t->tag,">")==0 || strstr(t->tag,"sexpr")) {

          bl_val_t* retval = bl_val_alloc_block(t->children_num);

          for(i=1; i < t->children_num-1; i++) {
              retval[i-1].type = VAL_TYPE_CONS;
              retval[i-1].car  = parse_ast(t->children[i]);
              retval[i-1].cdr  = bl_val_ref(&(retval[i]));
          }
          return bl_val_ref(retval);
       }
       return NULL;
}

mpc_parser_t* mpc_Number;
mpc_parser_t* mpc_Symbol;
mpc_parser_t* mpc_String;
mpc_parser_t* mpc_Sexpr;
mpc_parser_t* mpc_Expr;
mpc_parser_t* mpc_Lispy;


void init_mpc() {
     mpc_Number = mpc_new("number");
     mpc_Symbol = mpc_new("symbol");
     mpc_Sexpr  = mpc_new("sexpr");
     mpc_String = mpc_new("string");
     mpc_Expr   = mpc_new("expr");
     mpc_Lispy  = mpc_new("lispy");

    
     mpca_lang(MPCA_LANG_DEFAULT,
      "                                          \
        number : /-?[0-9]+/ ;                    \
        symbol : /[a-zA-Z0-9_+\\-*\\/\\\\=<>!&]+/ ;         \
        sexpr  : '(' <expr>* ')' ;               \
        string : /\"(\\\\.|[^\"])*\"/ ;         \
        expr   : <number> | <string> | <symbol> | <sexpr> ; \
        lispy  : /^/ <expr>* /$/ ;               \
      ", mpc_Number, mpc_Symbol, mpc_Sexpr, mpc_String, mpc_Expr, mpc_Lispy);

}

void handle_line(char* l) {
     printf("\n");
     if(!l) exit(0);
     add_history(l);
     char* copy = strdup(l);


    mpc_result_t r;
    if (mpc_parse("<stdin>", copy, mpc_Lispy, &r)) {
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


int handle_nl() {
    char* copy = strdup(rl_line_buffer);
    int retval = 0;
     mpc_result_t r;
     if(mpc_parse("<stdin>",copy,mpc_Lispy,&r)) {

        mpc_ast_delete(r.output);
        rl_done = 1;
     } else {
        rl_insert_text(" \n");
        rl_insert_text("           ");
        retval = 0;
     }
    free(copy);
    return retval;
}


void do_reads() {
     char c;
     rl_variable_bind("blink-matching-paren","on");
     rl_callback_handler_install("BearLang> ", &handle_line);
     rl_bind_key ('\r', &handle_nl);


     while (!feof(stdin)) {
        read(STDIN_FILENO, &c, 1);
        if(! rl_stuff_char(c)) exit(1);
        rl_callback_read_char();
     }
}


int main(int argc, char** argv) {
    repl_env  = bl_init_env();
    init_mpc();
    do_reads();
}
