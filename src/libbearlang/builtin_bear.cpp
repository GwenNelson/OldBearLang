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
    return this->func(&func_closure,cdr);
}

// BUILTIN FUNCTIONS START HERE


sexp::Value builtin_add_oper(BearCave* cave, sexp::Value cdr) {
    int retval = 0;
    for(sexp::ListIterator it(cdr); it != sexp::ListIterator(); ++it) {
        retval += it->as_int();
    }
    return sexp::Value::integer(retval);
}

sexp::Value builtin_minus_oper(BearCave* cave, sexp::Value cdr) {
    int retval = cdr.get_car().as_int() - cdr.get_cdr().get_car().as_int();

    return sexp::Value::integer(retval);
}

sexp::Value builtin_multiply_oper(BearCave* cave, sexp::Value cdr) {
    int retval = cdr.get_car().as_int() * cdr.get_cdr().get_car().as_int();

    return sexp::Value::integer(retval);
}

sexp::Value builtin_divide_oper(BearCave* cave, sexp::Value cdr) {
    int retval = cdr.get_car().as_int() / cdr.get_cdr().get_car().as_int();

    return sexp::Value::integer(retval);
}

sexp::Value builtin_eq_oper(BearCave* cave, sexp::Value cdr) {
    bool retval = cdr.get_car() == cdr.get_cdr().get_car();

    return sexp::Value::boolean(retval);
}

sexp::Value builtin_lt_oper(BearCave* cave, sexp::Value cdr) {
    bool retval = cdr.get_car().as_int() < cdr.get_cdr().get_car().as_int();

    return sexp::Value::boolean(retval);
}

sexp::Value builtin_gt_oper(BearCave* cave, sexp::Value cdr) {
    bool retval = cdr.get_car().as_int() > cdr.get_cdr().get_car().as_int();

    return sexp::Value::boolean(retval);
}

// evaluates every expression in the cdr and then returns the last value
// best used with ' to pass in the cdr
sexp::Value builtin_do_oper(BearCave* cave, sexp::Value cdr) {
    sexp::Value retval = sexp::Value::nil();
    for(sexp::ListIterator it(cdr); it != sexp::ListIterator(); ++it) {
        retval = cave->eval_sexp(*it);
    }
    return retval;
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
     c->write_sym("-",    (BearVal*)new BuiltinBear(builtin_minus_oper));
     c->write_sym("*",    (BearVal*)new BuiltinBear(builtin_multiply_oper));
     c->write_sym("/",    (BearVal*)new BuiltinBear(builtin_divide_oper));
     c->write_sym("<",    (BearVal*)new BuiltinBear(builtin_lt_oper));
     c->write_sym(">",    (BearVal*)new BuiltinBear(builtin_gt_oper));
     c->write_sym("do",   (BearVal*)new BuiltinBear(builtin_do_oper));
     c->write_sym("eq",   (BearVal*)new BuiltinBear(builtin_eq_oper));
     c->write_sym("print",(BearVal*)new BuiltinBear(builtin_print_func));

}

};
