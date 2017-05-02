// implements an s-expression splitter - feed it lines and get back s-expressions in strings ready for parsing

#include <iostream>
#include <string>
#include <bearlang/sexp_splitter.h>

#include <boost/algorithm/string/predicate.hpp>

namespace bearlang {

SExpSplitter::SExpSplitter() {
   layer      = 0;
   buf        = "";
   sexp_buf   = "";
   start_line = 0;
}

SExpSplitter::SExp SExpSplitter::add_line(std::string text, std::string from_file, int _line_no) {
     if(text.size()==0) return SExp(true,text,from_file,_line_no);
     if(start_file.size()==0) {
        start_file=from_file;
        start_line=_line_no;
     }
     buf      += text;
     sexp_buf  = "";
     layer     = 0;
     if(!boost::starts_with(buf,"(")) {
         SExp retval = SExp(false,buf,start_file,start_line);
         buf        = "";
         start_line = 0;
         start_file = "";
         return retval;
     } else {
         buf += "\n";
     }
     for(char& c : buf) {
         //TODO implement comments here, or add them to the preprocessor
         if(c=='(') layer++;
         if(c==')') layer--;
         sexp_buf += c;
         if(layer==0) {
            SExp retval = SExp(false,sexp_buf,start_file,start_line);
            buf        = "";
            start_line = 0;
            start_file = "";
            return retval;
         }
     }
     return SExp(true,text,from_file,_line_no);
}

};
