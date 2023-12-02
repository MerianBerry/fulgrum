#bold_green := \x1B[92;1m
#bold_cyan := \x1B[96;1m
#reset := \x1B[0m

OBJOUT := build/obj

c_src_dir := lib/interpreter
c_source_files := $(wildcard $(c_src_dir)/*.c)
c_object_files := $(patsubst $(c_src_dir)/%.c, $(OBJOUT)/%.o, $(c_source_files))
c_driver_files := $(wildcard src/interpreter/*.c)
c_driver_objfiles := $(patsubst src/interpreter/%.c, $(OBJOUT)/%.o, $(c_driver_files))
clicol_source_file := src/cli_color.c 
clicol_object_file := $(OBJOUT)/cli_color.o
lib_source_files := $(wildcard lib/*.ful)
c_standard := c89


.PHONY: all cc-check cli_color fulgrumi run fulgrum
.SILENT: cc-check
.ONESHELL:

ifeq ($(OS), Windows_NT)
executableExt := .exe
libExt := .lib
HOST := Windows
@mkdir := mkdir $(subst /,\,${1}) > nul 2>&1 || (exit 0)
null := > nul 2>&1
which := where
else
libPref := lib
libExt := .a
HOST := $(shell uname -s)
define mkdir
$(shell mkdir -p ${1})
endef
null := /dev/null
which := which
endif

interp := build/$(libPref)fulclib$(libExt)
fulgrumi := build/fulgrumi$(executableExt)
export cli_color := build/cli_color$(executableExt)
	

all: cc-check cli_color $(interp) fulgrumi

# Check for cc
cc-check:
	$(eval CC=null)
	$(eval CFMT=null)
	
	$(if $(shell $(which) gcc), $(eval CC=gcc), $(if $(shell $(which) clang), $(eval CC=clang)))
	$(info Consolodate compiler: $(CC))
	$(if $(value $(CC)), $(error Could not find viable compiler))
	$(if $(shell $(which) clang-format), $(eval CFMT=clang-format), $(info clang-format isnt present, will not format code))

# Compile Cross Compiled Color
cli_color: cc-check src/cli_color.c $(c_src_dir)/string.c
	@echo "[1/4]Building $(cli_color)"
	$(call mkdir, build)
	$(call mkdir, $(OBJOUT))
	$(CC) -std=$(c_standard) -I. src/cli_color.c $(c_src_dir)/string.c -o $(cli_color)
#	@echo [1/3]Done building color util

$(interp): cc-check cli_color ${c_object_files}
	@echo none > $(null)
	$(cli_color) "[2/4]&c(magenta)Building $(interp)&c(reset)"
	$(cli_color) "[2/4]&c(bright_green)Linking $(interp)&c(reset)"
	ar r $(interp) ${c_object_files} > $(null)

$(OBJOUT)/%.o: $(c_src_dir)/%.c
	@$(cli_color) "&c(cyan)Compiling $@ from $<&c(reset)"
	$(if $(value $(CFMT)),,$(shell $(CFMT) -i -style=file $<))
	$(CC) -std=$(c_standard) -g -c -I. $< -o $@

# Compile fulgrumi C files
fulgrumi: cc-check cli_color $(interp) $(c_driver_objfiles)
	@$(cli_color) "[3/4]&c(bright_green)Linking $(fulgrumi)&c(reset)"
	$(CC) -std=$(c_standard) -Lbuild ${c_driver_objfiles} $(interp) -o $(fulgrumi)

${c_driver_objfiles}: ${c_driver_files}
	@$(cli_color) "&c(cyan)Compiling $@ from $<&c(reset)"
	$(if $(value $(CFMT)),,$(shell $(CFMT) -i -style=file $<))
	$(CC) -std=$(c_standard) -g -c -I. $< -o $@

# Compile fulgrum files
fulgrum : fulgrumi lib/main.ful
	@$(cli_color) "[4/4]&c(magenta)Building main compiler\n&c(reset)"
	@$(fulgrumi) lib/main.ful
	@$(cli_color) "[4/4]&c(bright_green)Done\n&c(reset)"
