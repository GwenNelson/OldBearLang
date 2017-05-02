# BearLang

This is a lisp-like language built in C++.

# compiling

mkdir build

cd build

cmake ..

make

# running

Run bearlangi directly to open a REPL, pass it a filename to run that file.

# using the compiler

~/BearLang/build$ ./bearlangc ../examples/helloworld.bear 

~/BearLang/build$ g++ -std=c++11 -lreadline -lbearlang -L. ../examples/helloworld.bear.cpp -I../include -I../sexp-cpp/include

~/BearLang/build$ LD_LIBRARY_PATH=. ./a.out 
Hello, BearLang user

