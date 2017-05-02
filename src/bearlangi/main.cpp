// implements a simple interpreter
#include <bearlang/ppreader.h>
#include <bearlang/sexp_splitter.h>
#include <bearlang/cons_reader.h>
#include <bearlang/parser.h>
#include <bearlang/cave.h>
#include <bearlang/builtin_bear.h>

#include <iostream>
#include <string>
#include <stdio.h>
#include <unistd.h>
#include <exception>

#include <sexp/error.hpp>

using namespace std;
using namespace sexp; // i always read this as "sex p" for some reason
using namespace bearlang;

SExpParser p;
BearCave   root_cave;

void handle_expr(std::string exp, std::string from_file, int line_no, bool showret) {
     try {
         if(showret) {
            cout << root_cave.eval_sexp(p.parse_string(exp)) << endl;
         } else {
            root_cave.eval_sexp(p.parse_string(exp));
         }
     } catch(TypeError& e) {
         cerr << "Error when parsing " << from_file << ":" << line_no+e.get_line() << ": ";
         cerr << e.what() << endl;
     } catch(exception& e) {
         cerr << "General exception in expression starting from " << from_file << ":" << line_no << ": ";
         cerr << e.what() << endl;
     }
}

// TODO: switch PPReader to support passing stream or FD in rather than just filename
void handle_file(std::string filename) {
     PPReader r(filename);
     SExpSplitter s;
     for(;;) {
           PPReader::Line l = r.read();
           if(l.is_eof) break;
           SExpSplitter::SExp e = s.add_line(l.line, l.from_file,l.line_no);
           if(!e.is_null) {
              handle_expr(e.text, e.from_file, e.line_no,false);
           }
     }
}

void handle_cons() {
     ConsReader cons;
     while(!feof(stdin)) {
         string l = cons.get_line();
         if(l.size()==0) {
            cout << endl;
            return;
         }
         handle_expr(l, "<CONSOLE>", 0,true);
     }
}

int main(int argc, char **argv) {
    // add builtin bears to the root cave
    add_builtins(&root_cave);
    root_cave.write_sym("test",sexp::Value::string("meow"));

    if(argc==2) { // read from file
       handle_file(string(argv[1]));
      } else { // read from stdin
       if(isatty(0)) {
         handle_cons();
       } else {
         handle_file("/dev/stdin"); // fuck non-unix platforms
       }
    }
}
