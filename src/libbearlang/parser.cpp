// implements s-expression parser

#include <bearlang/parser.h>
#include <string>

namespace bearlang {

SExpParser::SExpParser() {
}

sexp::Value SExpParser::parse_string(std::string s) {
    return sexp::Parser::from_string(s,true);
}

};
