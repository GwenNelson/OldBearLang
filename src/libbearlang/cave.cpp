// implements the environment class, also known as the bear cave

#include <map>
#include <string>
#include <stdexcept> //TODO: rethink exception handling stuff

#include <iostream>

#include <boost/algorithm/string/predicate.hpp>

#include <bearlang/cave.h>
#include <bearlang/bear_val.h>

namespace bearlang {
BearCave::BearCave() {
    this->parent = NULL;
}

BearCave::BearCave(BearCave* _parent) {
    this->parent = _parent;
}

sexp::Value BearCave::read_sym(std::string sym_name) {
    if(symbols.count(sym_name)==0) {
       if(this->parent != NULL) {
          return this->parent->read_sym(sym_name);
       } else {
          throw std::runtime_error("Could not find symbol " + sym_name + " in any bear cave!");
       }
    } else {
       return this->symbols[sym_name]->get_val();
    }

}

bool BearCave::sym_exists(std::string sym_name) {
     if(symbols.count(sym_name)==1) return true;
     if(this->parent != NULL) return this->parent->sym_exists(sym_name);
     return false;
}

void BearCave::write_sym(std::string sym_name, sexp::Value sym_val) {
     this->symbols[sym_name] = new BearVal(sym_val);
}

void BearCave::write_sym(std::string sym_name, BearVal *b_val) {
     this->symbols[sym_name] = b_val;
}

bool BearCave::sym_is_func(std::string sym_name) {
     if(boost::starts_with(sym_name,"'")) return false;
     if(!sym_exists(sym_name)) return false;
     if(symbols.count(sym_name)==1) {
        return this->symbols[sym_name]->is_func;
     } else {
        return this->parent->sym_is_func(sym_name);
     }
     return false;
}

sexp::Value BearCave::eval_func(std::string func_name, sexp::Value cdr) {
     if(!sym_is_func(func_name)) {
        throw std::runtime_error("eval_func() called on non-function!");
     }
     if(symbols.count(func_name)==1) {
        return this->symbols[func_name]->eval(this,this->eval_sexp(cdr));
     } else {
        return this->parent->eval_func(func_name,this->eval_sexp(cdr));
     }
}

sexp::Value BearCave::eval_sexp(sexp::Value exp) {
     std::string sym_name;
     switch(exp.get_type()) {
         case sexp::Value::Type::TYPE_NIL:
              return exp;
         break;
         case sexp::Value::Type::TYPE_BOOLEAN:
              return exp;
         break;
         case sexp::Value::Type::TYPE_INTEGER:
              return exp;
         break;
         case sexp::Value::Type::TYPE_STRING:
              return exp;
         break;
         case sexp::Value::Type::TYPE_SYMBOL:
              sym_name = exp.as_string();
              if(boost::starts_with(sym_name,"'")) {
                 return sexp::Value::symbol(sym_name.substr(1));
              } else {
                 if(!sym_exists(sym_name)) throw std::runtime_error("Symbol " + sym_name + " not found in bear cave!");
                 return read_sym(sym_name);
              }
         break;
         case sexp::Value::Type::TYPE_CONS:{
              sexp::Value exp_car = exp.get_car();

              if(exp_car.is_symbol()) {
                  std::string car_sym_name = exp_car.as_string();
                  if(car_sym_name.compare("'")==0) return exp.get_cdr();
                  if(car_sym_name.compare("fun")==0) {
                     sexp::Value func        = exp.get_cdr();
                     std::string func_name   = func.get_car().as_string();
                     sexp::Value func_body   = func.get_cdr();
                     this->write_sym(func_name,new BearVal(func_body, true));
                     return sexp::Value::nil();
                  }
                  if(!sym_exists(car_sym_name)) throw std::runtime_error("Symbol " + car_sym_name + " not found in bear cave!");

                  if(sym_is_func(car_sym_name)) {
                     return eval_func(car_sym_name, eval_sexp(exp.get_cdr()));
                  } else {
                     return read_sym(car_sym_name);
                  }
              } else {
                  sexp::Value retval = sexp::Value::array();
                  for(sexp::ListIterator it(exp); it != sexp::ListIterator(); ++it) {
                      retval.append(eval_sexp(*it));
                  }
                  return sexp::Parser::from_string(retval.str().substr(1));
              }
         }
         break;
         case sexp::Value::Type::TYPE_ARRAY: // TODO: get rid of the array type, everything should just be lists
         break;
     }

}

};
