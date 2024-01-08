<h1>
  <center>
  Fulgrum
  </center>
</h1>

## UHH

Welcome? This is the repository for a programming language project named Fulgrum. The overall purpose is to provide a language with qualities from Lua, C, and maybe a little bit of Rust. Syntax, system access from C. Functionality, modularity, flexibility, and ease-of-use of Lua. Type and memory safety of Rust. All this while being precompiled.

Be aware that as this project has really just begun, lots of it is still subject to change.

# Current vision

## Compiler Infrastructure

The base of the compiler infrastructure is an interpreter written in C99. Its purpose is to interpret and run the full precompiler code written in Fulgrum, which is used to compile the same Fulgrum precompiler.

`C Interpreter (runs)-> Fulgrum Compiler (compiles)-> Fulgrum Compiler -> Compiler Binary`

The C Interpreter is written with an API, so any C/C++ code can `#include` the [*API header*](lib/bootstrap/include/fulgrum.h), and link the interpreter, and run Fulgrum code from their C programs (this is how the C interpeter will work, using the [*driver C file*](src/bootstrap/driver.c)).

The Fulgrum precompiler will be able to compile Fulgrum source code into whatever target machine code in a standard linkable (ELF/Windows binary format) format. With this, you can create native, high performance applications or libraries.

Another precomplication function the compiler will have, is compilation into non-native byte code packages, that are cross platform and are only linkable by the Fulgrum linker. This allows source code to be packaged, and imported into user source code, and compiled to machine code along with the rest of the source code.

### Voltaic

Voltaic is the build system and package manager (so to speak), and is written in C99. Is purpose is to make sure user source code is compiled for the desired target and format, and to have the required packages. The configuration file will be in the `.toml` format.
