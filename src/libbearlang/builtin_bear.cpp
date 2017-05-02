// implements builtin function base class - a builtin bear - and several builtins together with util for adding them to a root bear cave

#include <bearlang/builtin_bear.h>
#include <bearlang/cave.h>

#include <iostream>
#include <sstream>

#include <string>

namespace bearlang {

BuiltinBear::BuiltinBear(bear_native_func_t _func) {
    std::ostringstream funcaddr;
    funcaddr << (void const*)_func;
    
    this->val     = sexp::Value::symbol("'native-function@"+funcaddr.str());
    this->func    = _func;
    this->is_func = true;
}

sexp::Value BuiltinBear::eval(BearCave* cave, sexp::Value cdr) {
    BearCave func_closure(cave); // create a new cave to use as function closure
    return this->func(&func_closure,cave->eval_sexp(cdr));
}

// BUILTIN FUNCTIONS START HERE

sexp::Value builtin_add_oper(BearCave* cave, sexp::Value cdr) {
    int retval = 0;
    for(sexp::ListIterator it(cdr); it != sexp::ListIterator(); ++it) {
        retval += it->as_int();
    }
    return sexp::Value::integer(retval);
}

sexp::Value builtin_print_func(BearCave* cave, sexp::Value cdr) {
    for(sexp::ListIterator it(cdr); it != sexp::ListIterator(); ++it) {
        if(it->is_string()) {
           std::cout << it->as_string();
        } else {
           std::cout << *it;
        }
    }
    return sexp::Value::nil();
}

void add_builtins(BearCave* c) {
     c->write_sym("+",    (BearVal*)new BuiltinBear(builtin_add_oper));
     c->write_sym("print",(BearVal*)new BuiltinBear(builtin_print_func));
}

};
