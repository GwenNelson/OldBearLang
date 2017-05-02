// implements s-expression parser

#pragma once

// TODO: write own value class with proper COW semantics etc as in design
#include <sexp/io.hpp>
#include <sexp/value.hpp>
#include <sexp/parser.hpp>
#include <sexp/util.hpp>

#include <string>

namespace bearlang {
    class SExpParser {
       public:
          SExpParser();
          sexp::Value parse_string(std::string s);
    };
};
