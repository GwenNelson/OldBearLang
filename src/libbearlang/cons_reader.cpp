// implements an s-expression splitter for console usage using GNU readline

#include <readline/readline.h>
#include <bearlang/sexp_splitter.h>
#include <bearlang/cons_reader.h>
#include <string>

namespace bearlang {

ConsReader::ConsReader() {
}


// TODO: add nicer editing, stuff like history etc, retain indentation level on newlines, tab auto-complete and such
std::string ConsReader::get_line() {
     char* l = readline("bearlang> ");
     if(l==NULL) return "";
     SExpSplitter::SExp e = s.add_line(std::string(l),"",0);
     free(l);
     if(e.is_null) {
       while(e.is_null) {
          l = readline("      ... ");
          if(l==NULL) return "";
          e = s.add_line(std::string(l),"",0);
          free(l);
       }
       return e.text;
     } else {
       return e.text;
     }
}

};
