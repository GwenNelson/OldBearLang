#!/bin/sh

mkdir -p build
cd build
cmake ..
make

./bearlangc ../examples/repl.bear
g++ -std=c++11 -L. -lreadline -lbearlang ../examples/repl.bear.cpp -I../include -I../sexp-cpp/include -o bearrepl
