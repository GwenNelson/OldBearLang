// implements the s-exp values used by BearCave

#pragma once

#include <sexp/io.hpp>
#include <sexp/parser.hpp>
#include <sexp/util.hpp>
#include <sexp/value.hpp>

#include <map>
#include <string>


//TODO: at some point migrate from s-exp library possibly
//TODO: add garbage collection using a static instance of garbage collector and reference counting here
namespace bearlang {
    class BearCave;
    class BearVal {
       public:
          BearVal();
          BearVal(sexp::Value v);
          BearVal(sexp::Value v, bool _func);

          sexp::Value get_val(); // just dumps the value
          virtual sexp::Value eval(BearCave* cave, sexp::Value cdr); // for use by functions and native code builtins

          bool is_func;

       protected:
          sexp::Value val;

    };
};
