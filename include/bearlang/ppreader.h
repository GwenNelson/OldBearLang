// Implements a simple preprocessor that spits out lines, filenames and line numbers

#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <map>

//TODO: add support for specifying include paths
namespace bearlang {
    class PPReader {
       public:
          class Line { // represents a single line of input or an EOF and adds context and such
             public:
                Line(bool _is_eof, std::string _line, std::string _from_file, int _line_no) {is_eof = _is_eof; line=_line; from_file = _from_file; line_no = _line_no;};
                bool is_eof;
                std::string line;
                std::string from_file;
                int line_no;
          };
          PPReader(std::string filename);
          ~PPReader();
          void set_parent(PPReader* p);
          PPReader* get_root_reader(); // returns the root of the parent/child hiearchy
          std::string get_cur_filename();
          int get_cur_line_no();
          Line read(); // returns a single instance of Line class
       private:
          PPReader *parent;
          PPReader *child;
          std::ifstream *input_fd;
          std::string cur_filename;
          int cur_line_no;
       protected:
          std::map<std::string,std::string> defines;
    };
};
