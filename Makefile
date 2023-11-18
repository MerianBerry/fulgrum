bold_green := \x1B[92;1m
bold_cyan := \x1B[96;1m
reset := \x1B[0m

s0 := build/s0

s0_source_files := $(wildcard src/bootstrap/s0/*.c)
s0_object_files := $(patsubst src/bootstrap/s0/*.c, src/bootstrap/%.o, $(s0_source_files))
s1_source_files := $(wildcard src/bootstrap/s1/*.ful)

.PHONY: all clean s0 run s0

all: s0 s1

clean:
	@rm -rf build

s0: $(s0_source_files)
# compile s0 C files
	@mkdir -p $(shell dirname $(s0))
	@printf '[0/2]$(bold_green)Building boostrap compiler stage 0$(reset)\n'
	@gcc -std=c99 $(s0_source_files) -o $(s0)
	@printf '[1/2]$(bold_cyan)Done building s0 compiler$(reset)\n'

s1 : s0 $(s1_source_files)
	@printf '[1/2]$(bold_green)Building boostrap compiler stage 1$(reset)\n'
#	@sleep 1s
	@$(s0) src/bootstrap/s1/*.ful
	@printf '[2/2]$(bold_cyan)Done building s1 compiler$(reset)\n'
