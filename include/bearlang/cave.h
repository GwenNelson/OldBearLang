// implements the environment class, also known as the bear cave

#pragma once

#include <sexp/io.hpp>
#include <sexp/parser.hpp>
#include <sexp/util.hpp>

#include <sexp/value.hpp> //TODO: replace with our own value class

#include <map>
#include <string>

#include <bearlang/bear_val.h>

namespace bearlang {
    class BearCave {
       public:
          BearCave();
          BearCave(BearCave* _parent);
          sexp::Value eval_sexp(sexp::Value exp);
          sexp::Value eval_func(std::string func_name, sexp::Value cdr);
          sexp::Value read_sym(std::string sym_name);
          bool sym_exists(std::string sym_name);
          bool sym_is_func(std::string sym_name);
          void write_sym(std::string sym_name, sexp::Value sym_val);
          void write_sym(std::string sym_name, BearVal* b_val);
       private:
          BearCave* parent;
          std::map<std::string,BearVal*> symbols;

    };
};
