# Blossom Bootstrap Compiler

This multi-stage compiler has the purpose of compiling **basic** Blossom source code into a compiler of its own, which can compile the full Blossom compiler code.

## So there are 3 stages:

### Stage 0

Written in C, this compiler just serves to compile basic blossom code into machine code, and be an introduction into compiling code by scratch (I do not want to translate llvm).

This stage will compile the Stage 1 Blossom code into an executable.

### Stage 1

Written in basic Blossom, this compiler must handle more semantic information, and be able to compile the full Blossom source code of the Stage 2 compiler.

### Stage 2

This stage is considered a library/package, and will be able to compile generic Blossom source code (including itself).