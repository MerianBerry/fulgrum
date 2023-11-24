#bold_green := \x1B[92;1m
#bold_cyan := \x1B[96;1m
#reset := \x1B[0m
s0 := build/s0
ccolor := build/ccolor

s0_source_files := $(wildcard src/bootstrap/s0/*.c)
s0_object_files := $(patsubst src/bootstrap/s0/*.c, src/bootstrap/%.o, $(s0_source_files))
s1_source_files := $(wildcard src/bootstrap/s1/*.ful)

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



.PHONY: all gcccheck clean ccolor s0 run s1
.SILENT: gcccheck
	

all: gcccheck ccolor s0 s1

clean:
	@rm -rf build

# Check for gcc 
gcccheck:
	@gcc --help > $(null) || \
		(echo Failed to find gcc compiler; exit 1)
	@echo GCC compiler is present

# Compile Cross Compiled Color
ccolor: gcccheck src/ccolor.c src/bootstrap/s0/string.c
	$(call mkdir, build)
	@echo [0/3]Building color util 
	@gcc -std=c99 -Wno-overflow src/ccolor.c src/bootstrap/s0/string.c -o $(ccolor)$(executableExt)
	@echo [1/3]Done building color util

# Compile s0 C files
s0: gcccheck ccolor $(s0_source_files)
	@$(ccolor) "[1/3]%c(magenta)Build bootstrap compiler stage 0\n%c(reset)"
	@gcc -std=c99 -Wno-overflow $(s0_source_files) -o $(s0)$(executableExt)
	@$(ccolor) "[2/3]%c(bright_green)Done building s0 compiler\n%c(reset)"

# Compile s1 Fulgra files
s1 : s0 $(s1_source_files)
	@$(ccolor) "[2/3]%c(magenta)Building boostrap compiler stage 1\n%c(reset)"
	@$(s0)$(executableExt) src/bootstrap/s1/main.ful
	@$(ccolor) "[3/3]%c(bright_green)Done building s1 compiler\n%c(reset)"
