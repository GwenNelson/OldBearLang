#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include <unistd.h>

#include <stdbool.h>

#include <readline/readline.h>

#include <mpc.h>

#include <bearlang/bearlang.h>

bl_val_t* repl_env;

bl_val_t* parse_ast(mpc_ast_t *t) {
       int i=0;
       if(strstr(t->tag,"number")) return bl_mk_int(atoi(t->contents));
       if(strstr(t->tag,"symbol")) { 
          return bl_mk_symbol(t->contents);
       }
       if(strstr(t->tag,"string"))  {
          char* str_content = strdup(t->contents + 1);
          str_content[strlen(str_content)-1] = 0;
          str_content = mpcf_unescape(str_content);
          bl_val_t* sval = bl_mk_str(str_content);
          free(str_content);
          return sval;
       }
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
mpc_parser_t* mpc_Bool;
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

bl_val_t* handle_expr(bl_val_t* e,bool echoret) {
     bl_val_t* ret_val    = bl_eval_expr(repl_env,e);
     if(echoret) {
        bl_dump_expr(ret_val);printf("\n");
     }
     return ret_val;
}

int sexp_ready;
char* last_line = NULL;
void handle_line(char* l) {
     printf("\n");
     if(!l) exit(0);
     if(sexp_ready != 1) { rl_insert_text(l); free(l); return; }
     add_history(l);
     char* copy = strdup(l);
     free(l);


    mpc_result_t r;
    if (mpc_parse("<stdin>", copy, mpc_Lispy, &r)) {
      /* On success print and delete the AST */
//      mpc_ast_print(r.output);
      
      mpc_ast_t* t = r.output;
      bl_val_t* parsed_val = parse_ast(t);
      bl_val_t* ret_val    = handle_expr(parsed_val->car,true);
      bl_val_free(ret_val);
      bl_val_free(parsed_val);
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
        sexp_ready = 1;
        rl_set_prompt("BearLang> ");
     } else {
        mpc_err_delete(r.error);
        rl_set_prompt("        > ");

        last_line = strdup(rl_line_buffer);
        rl_insert_text(" \n");
        rl_done = 1;
        sexp_ready = 0;
//        rl_insert_text("           ");
        retval = 0;
     }
    free(copy);
    return retval;
}

int pre_input_hack() { 
    if(!sexp_ready) {
       if(last_line != NULL) {
          rl_insert_text(last_line);
          rl_insert_text(" \n          ");
          free(last_line);
          last_line = NULL;
       }
    }
}

void do_tty_reads() {
     char c;
     rl_variable_bind("blink-matching-paren","on");
     rl_callback_handler_install("BearLang> ", &handle_line);
     rl_pre_input_hook = pre_input_hack;
     rl_bind_key ('\r', &handle_nl);


     while (!feof(stdin)) {
        read(STDIN_FILENO, &c, 1);
        if(! rl_stuff_char(c)) exit(1);
        rl_callback_read_char();
     }
}

void run_tests() {
     bl_val_t* test_hashmap = bl_mk_hashmap(4);
     bl_hashmap_set(test_hashmap,bl_mk_symbol("foo"),bl_mk_str("foobar"));
     bl_hashmap_set(test_hashmap,bl_mk_symbol("bla"),bl_mk_str("meow"));
     bl_dump_expr(bl_hashmap_get(test_hashmap,bl_mk_symbol("foo")));printf("\n");
     bl_dump_expr(bl_hashmap_get(test_hashmap,bl_mk_symbol("bla")));printf("\n");
     bl_hashmap_set(test_hashmap,bl_mk_symbol("foo"),bl_mk_str("new foo value"));
     bl_dump_expr(bl_hashmap_get(test_hashmap,bl_mk_symbol("foo")));printf("\n");

}

int main(int argc, char** argv) {
    repl_env  = bl_init_env();
    init_mpc();
    run_tests();
    if(argc==2) {
       mpc_result_t r;
       if(mpc_parse_contents(argv[1],mpc_Lispy,&r)) {
         bl_val_t* parsed_val = parse_ast(r.output);
         bl_val_t* c = parsed_val;
         while(c != NULL) {
           if(bl_list_car(c) != NULL) bl_val_free(handle_expr(bl_list_car(c),false));
           c = bl_list_cdr(c);
         }

         bl_val_free(parsed_val);
         mpc_ast_delete(r.output);
       } else {
         mpc_err_print(r.error);
         mpc_err_delete(r.error);
       }
    }
    if(isatty(0)) {
       do_tty_reads();
    }
}
