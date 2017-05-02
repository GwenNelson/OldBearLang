// implements an s-expression splitter for console usage using GNU readline

#pragma once

#include <bearlang/sexp_splitter.h>

#include <string>

namespace bearlang {
    class ConsReader {
       public:
          ConsReader();
          std::string get_line();
       private:
          SExpSplitter s;
    };
};
