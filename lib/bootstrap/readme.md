# Fulgrum Bootstrap Interpreter

An interpreter written in C, that runs the primary fulgrum code to compile itself.

Its a pretty weird process.

The C interpreter is a library, driven by [the bootstrap driver](../../src/bootstrap/main.c), that interprets the primary fulgrum library code, which is used to compile the primary fulgrum library code. It's like codeception.
