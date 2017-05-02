// implements the s-exp values used by BearCave

#include <sexp/io.hpp>
#include <sexp/parser.hpp>
#include <sexp/util.hpp>
#include <sexp/value.hpp>

#include <vector>

#include <bearlang/cave.h>
#include <bearlang/bear_val.h>

#include <iostream>

namespace bearlang {
BearVal::BearVal() {
    this->val     = sexp::Value::nil();
    this->is_func = false;
}

BearVal::BearVal(sexp::Value v) {
    this->val     = v;
    this->is_func = false;
}

BearVal::BearVal(sexp::Value v, bool _func) {
    this->val     = v;
    this->is_func = _func;
}

sexp::Value BearVal::get_val() {
    return this->val;
}

sexp::Value BearVal::eval(BearCave *cave, sexp::Value cdr) {
    // default implementation just returns the value and ignores cdr for non functions
    if(this->is_func) {
      BearCave func_closure(cave);
      
      sexp::Value param_names = sexp::Value::array();
      for(sexp::ListIterator it(this->val.get_car()); it != sexp::ListIterator(); ++it) {
          param_names.append(sexp::Value(*it));
      }

      sexp::Value param_values = sexp::Value::array();
      for(sexp::ListIterator it(cdr); it != sexp::ListIterator(); ++it) {
          param_values.append(sexp::Value(*it));
      }

      std::vector<sexp::Value> param_names_v  = param_names.as_array();
      std::vector<sexp::Value> param_values_v = param_values.as_array();
      for(int i=0; i < list_length(cdr); i++) {
          func_closure.write_sym(param_names_v[i].as_string(),param_values_v[i]);
      }

      sexp::Value retval = sexp::Value::nil();
      sexp::Value func_body = this->val.get_cdr();
      for(sexp::ListIterator it(func_body); it != sexp::ListIterator(); ++it) {
          sexp::Value line(*it);
          retval = func_closure.eval_sexp(line);
      }
      return retval;

    } else {
      return this->val;
    }
}

};
