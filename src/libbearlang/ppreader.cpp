// Implements a simple preprocessor that spits out lines, filenames and line numbers

#include <string.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <bearlang/ppreader.h>

#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/replace.hpp>

namespace bearlang {
PPReader::PPReader(std::string filename) {
    this->cur_filename = filename;
    this->cur_line_no  = 0;
    this->parent       = NULL;
    this->child        = NULL;
    this->input_fd     = new std::ifstream(filename);
}

PPReader::~PPReader() {
    if(this->child != NULL)    delete this->child;
    if(this->input_fd != NULL) delete this->input_fd;
}

void PPReader::set_parent(PPReader* p) {
     this->parent = p;
}

PPReader* PPReader::get_root_reader() {
     if(this->parent==NULL) return this;
     return this->parent->get_root_reader(); // this should recurse up the tree until hitting one that's NULL
}

std::string PPReader::get_cur_filename() {
    if(this->child != NULL) return this->child->get_cur_filename();
    return this->cur_filename;
}

int PPReader::get_cur_line_no() {
    if(this->child != NULL) return this->child->get_cur_line_no();
    return this->cur_line_no;
}

PPReader::Line PPReader::read() {
    // check first if we have a child include, and if so read from it or kill it for EOF
    if(this->child != NULL) {
       Line retval = this->child->read();
       if(retval.is_eof) { // if the child is at EOF we should close it and return to reading our own file, but cleaning up first
         delete this->child;
         this->child = NULL;
       } else {
         return retval;    // if the child is not at EOF, we should return the next line from child
       }
    }
    std::string buf;
    if(!std::getline(*input_fd, buf)) { // we are at EOF, so let's return the EOF Line instance
       return Line(true, // is EOF
                   std::string(""), // empty string
                   cur_filename,
                   cur_line_no);
    }
    for (auto const& d : get_root_reader()->defines) { // replace any #define bits
          boost::replace_all(buf, d.first, d.second);
    }
    if(boost::starts_with(buf, ";")) { // comments should be totally ignored except for incrementing line number
        cur_line_no++;
        return this->read();
    }
    if(boost::starts_with(buf,"#!")) {
        cur_line_no++;
        return this->read();
    }
    if(buf.find(";") != std::string::npos) {
       buf = buf.substr(0,buf.find(";"));
    }

    if(boost::starts_with(buf, "#define ")) {
       // extract key and value
       char* defline = strdup(buf.c_str());
       std::string k = std::string(strtok(defline+8," "));
       std::string v = std::string(strtok(NULL," "));
       free(defline);
       boost::replace_all(v, "\n", "");
       get_root_reader()->defines[k] = v;
       cur_line_no++;
       return this->read(); // like with comments, #define lines are skipped
    }
    if(boost::starts_with(buf,"#include ")) {
       cur_line_no++;
       this->child = new PPReader(buf.substr(9));
       return this->child->read();
    }
    cur_line_no++;
    Line retval = Line(false,buf,cur_filename,cur_line_no);
    return retval;
}

};
