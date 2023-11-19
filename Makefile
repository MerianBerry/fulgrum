#bold_green := \x1B[92;1m
#bold_cyan := \x1B[96;1m
#reset := \x1B[0m

s0 := build/s0
ccolor := build/ccolor

s0_source_files := $(wildcard src/bootstrap/s0/*.c)
s0_object_files := $(patsubst src/bootstrap/s0/*.c, src/bootstrap/%.o, $(s0_source_files))
s1_source_files := $(wildcard src/bootstrap/s1/*.ful)

ifeq ($(OS), Windows_NT)
	HOST := Windows
	@mkdir := mkdir $(subst /,\,${1}) > nul 2>&1 || (exit 0)
else
	HOST := $(shell uname -s)
define mkdir
$(shell mkdir -p ${1})
endef
endif



.PHONY: all clean ccolor s0 run s1
	

all: s0 s1

clean:
	@rm -rf build

# Compile Cross Compiled Color
ccolor: src/ccolor.c src/bootstrap/s0/string.c
	$(call mkdir, build)
	@echo [0/3]Building color util 
	@gcc -std=gnu99 -Wno-overflow src/ccolor.c src/bootstrap/s0/string.c -o $(ccolor)
	@echo [1/3]Done building color util

# Compile s0 C files
s0: ccolor $(s0_source_files)
	@$(ccolor) "[1/3]%c(magenta)Build bootstrap compiler stage 0\n%c(reset)"
	@gcc -std=gnu99 -Wno-overflow -lc $(s0_source_files) -o $(s0)
	@$(ccolor) "[2/3]%c(bright_green)Done building s0 compiler\n%c(reset)"

# Compile s1 Fulgra files
s1 : s0 $(s1_source_files)
	@$(ccolor) "[2/3]%c(magenta)Building boostrap compiler stage 1\n%c(reset)"
	@cd build; \
		./s0 ../src/bootstrap/s1/*.ful
	@$(ccolor) "[3/3]%c(bright_green)Done building s1 compiler\n%c(reset)"
