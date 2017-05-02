// implements builtin function base class - a builtin bear - and several builtins together with util for adding them to a root bear cave

#pragma once

#include <bearlang/bear_val.h>
#include <bearlang/cave.h>

typedef sexp::Value (*bear_native_func_t)(bearlang::BearCave*,sexp::Value); // params are the cave created for evaluating the function in and the cdr passed to the function

namespace bearlang {
    class BuiltinBear : public BearVal {
       public:
          BuiltinBear(bear_native_func_t _func);
          sexp::Value eval(BearCave* cave, sexp::Value cdr);
       private:
          bear_native_func_t func;
    };

    void add_builtins(BearCave* c);
};
