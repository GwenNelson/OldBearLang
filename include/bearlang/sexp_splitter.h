// implements an s-expression splitter - feed it lines and get back s-expressions in strings ready for parsing

#pragma once

#include <string>
#include <sstream>
namespace bearlang {
    class SExpSplitter {
       public:
          class SExp { // represents a single s-expression ready to be parsed
             public:
                SExp(bool _is_null, std::string _text, std::string _from_file, int _line_no) {is_null = _is_null; text = _text; from_file =_from_file; line_no = _line_no;};
                bool is_null; // returned if the last added line didn't complete an S-Expression
                std::string text;
                std::string from_file;
                int line_no; // this represents the line the expression begins on, it may cover multiple lines
          };
          SExpSplitter();
          SExp add_line(std::string text, std::string from_file, int _line_no);

       private:
          int start_line;
          int layer;
          std::string start_file;
          std::string sexp_buf;
          std::string buf;
    };
};
