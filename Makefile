#bold_green := \x1B[92;1m
#bold_cyan := \x1B[96;1m
#reset := \x1B[0m
bootstrap := build/bootstrap
ccolor := build/ccolor

c_source_files := $(wildcard lib/bootstrap/*.c)
c_driver_files := $(wildcard src/bootstrap/*.c)
c_object_files := $(patsubst src/bootstrap/*.c, src/bootstrap/%.o, $(c_source_files))
lib_source_files := $(wildcard lib/*.ful)

ifeq ($(OS), Windows_NT)
	executableExt := .exe
	HOST := Windows
	@mkdir := mkdir $(subst /,\,${1}) > nul 2>&1 || (exit 0)
null := nul 2>&1
else
	HOST := $(shell uname -s)
define mkdir
$(shell mkdir -p ${1})
endef
null := /dev/null
endif



.PHONY: all gcccheck ccolor bootstrap run fulgrum
.SILENT: gcccheck
	

all: gcccheck ccolor bootstrap fulgrum

# Check for gcc 
gcccheck:
	@gcc --help > $(null) || \
		(echo Failed to find gcc compiler; exit 1)
	@echo GCC compiler is present

# Compile Cross Compiled Color
ccolor: gcccheck src/ccolor.c lib/bootstrap/string.c
	$(call mkdir, build)
	@echo [0/3]Building color util 
	@gcc -std=c99 -Wno-overflow -I. src/ccolor.c lib/bootstrap/string.c -o $(ccolor)$(executableExt)
	@$(ccolor) "[1/3]%c(bright_green)Done building cli color util%c(reset)\n"
#	@echo [1/3]Done building color util

# Compile bootstrap C files
bootstrap: gcccheck ccolor $(c_source_files) $(c_driver_files)
	@clang-format -i -style=file $(c_source_files)
	@$(ccolor) "[1/3]%c(magenta)Building bootstrap compiler\n%c(reset)"
	@gcc -std=c99 -Wno-overflow -I. $(c_source_files) $(c_driver_files) -o $(bootstrap)$(executableExt)
	@$(ccolor) "[2/3]%c(bright_green)Done building bootstrap compiler\n%c(reset)"

# Compile fulgrum files
fulgrum : bootstrap lib/main.ful
	@$(ccolor) "[2/3]%c(magenta)Building main compiler\n%c(reset)"
	@$(bootstrap)$(executableExt) lib/main.ful
	@$(ccolor) "[3/3]%c(bright_green)Done building main compiler\n%c(reset)"
