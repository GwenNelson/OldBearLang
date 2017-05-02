# BearLang

This is a lisp-like language built in C++.

# Basic syntax etc

BearLang is a lisp-like language so all programs are lists of s-expressions.

Use the following code to define a named function:

```
(fun func_name (arg_a arg_b arg_c) ... )
```

The function body goes in the ... above and is just a list of statements, see the examples.

Lists are evaluated immediately unless quoted by making the first symbol in the list a quote character.

Builtins are provided in native code in builtin_bear.cpp and in the eval function in cave.cpp - read those files for instructions

# compiling

mkdir build

cd build

cmake ..

make

# running

Run bearlangi directly to open a REPL, pass it a filename to run that file.

# using the compiler

See make_repl.sh for an example of how to compile to native code - this is still quite hacky at present

```
~/BearLang/build$ ./bearlangc ../examples/helloworld.bear 
~/BearLang/build$ g++ -std=c++11 -lreadline -lbearlang -L. ../examples/helloworld.bear.cpp -I../include -I../sexp-cpp/include
~/BearLang/build$ LD_LIBRARY_PATH=. ./a.out
Hello, BearLang user
```
