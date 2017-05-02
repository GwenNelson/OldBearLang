// implements a hackish compiler
#include <bearlang/ppreader.h>
#include <bearlang/sexp_splitter.h>
#include <bearlang/parser.h>

#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sexp/value.hpp>
#include <sexp/util.hpp>
#include <sexp/error.hpp>

#include <boost/algorithm/string/replace.hpp>

using namespace std;
using namespace sexp; // i always read this as "sex p" for some reason
using namespace bearlang;

SExpParser p;
ofstream cppfile;

std::string val_to_cpp_str(sexp::Value v) {
     switch(v.get_type()) {
                case sexp::Value::Type::TYPE_NIL:
                     return "sexp::Value::nil()";
                break;
                case sexp::Value::Type::TYPE_BOOLEAN:
                     if(v.as_bool()) { 
                        return "sexp::Value::boolean(true)";
                     } else {
                        return "sexp::Value::boolean(false)";
                     }
                break;
                case sexp::Value::Type::TYPE_INTEGER:
                     return "sexp::Value::int("+v.str()+")";
                break;
                case sexp::Value::Type::TYPE_STRING:{
                     std::string s = v.as_string();
                     boost::replace_all(s,"\n","\\n");
                     return "sexp::Value::string(\"" +s+"\")";}
                break;
                case sexp::Value::Type::TYPE_SYMBOL:
                     return "sexp::Value::symbol(\"" + v.as_string()+"\")";
                break;
                case sexp::Value::Type::TYPE_CONS:
                     return "sexp::Value::cons(\n" + val_to_cpp_str(v.get_car()) + "," + val_to_cpp_str(v.get_cdr()) + ")\n";
                break;
     }
}

void handle_expr(std::string exp, std::string from_file, int line_no) {
     try {
         sexp::Value v = p.parse_string(exp);

         cppfile << "root_cave.eval_sexp(" << val_to_cpp_str(v) << ");" << endl;

     } catch(TypeError& e) {
         cerr << "Error when parsing " << from_file << ":" << line_no+e.get_line() << ": ";
         cerr << e.what() << endl;
         cerr << "Can not continue compilation, closing" << endl;
         abort();
     } 
}

void handle_file(std::string filename) {
     cppfile.open(filename + ".cpp");
     cppfile << "#include <bearlang/cave.h>" << endl;
     cppfile << "#include <bearlang/builtin_bear.h>" << endl;
     cppfile << "int main(int argc, char** argv) {bearlang::BearCave root_cave; bearlang::add_builtins(&root_cave);" << endl;
     PPReader r(filename);
     SExpSplitter s;
     for(;;) {
           PPReader::Line l = r.read();
           if(l.is_eof) break;
           SExpSplitter::SExp e = s.add_line(l.line, l.from_file,l.line_no);
           if(!e.is_null){ 
              handle_expr(e.text, e.from_file, e.line_no);
           }
     }
     cppfile << "return 0;}" << endl;

}

void show_usage() {
     cout << "Usage: bearlangc <sourcefile>" << endl;
}

int main(int argc, char **argv) {
    if(argc==2) { // read from file
       handle_file(string(argv[1]));
      } else {
       show_usage();
    }
}
