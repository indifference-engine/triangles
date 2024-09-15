ifeq ($(OS),Windows_NT)
	CC = x86_64-w64-mingw32-gcc
else
	CC = clang
endif

CFLAGS = -Wall -Wextra -Werror -std=c99 -nostdlib -ffreestanding -O3 -pedantic

# Unfortunately, I have found that make quite often selects the wrong shell
# (e.g. PowerShell), so commands like "find" won't work unless we explicitly
# specify bash.  You also can't use a variable for this (e.g. $(SHELL)) as make
# inexplicably tries to read something from the PATH and fails.  So hardcoding a
# reference to bash seems to be the only way to get a working build.
C_FILES = $(shell bash -c "find src -type f -iname ""*.c""") $(shell bash -c "find tests/helpers -type f -iname ""*.c""")
H_FILES = $(shell bash -c "find src -type f -iname ""*.h""") $(shell bash -c "find tests/helpers -type f -iname ""*.h""")
O_FILES = $(patsubst %.c,obj/%.o,$(C_FILES))
TOTAL_REBUILD_FILES = makefile $(H_FILES)

TESTS = $(patsubst tests/cases/%, %, $(shell bash -c "find tests/cases -mindepth 1 -maxdepth 1 -type d"))

test: $(addprefix tests/pass_markers/, $(TESTS))

dist/%: tests/cases/%/main.c $(O_FILES)
	mkdir -p $(dir $@)
	$(CC) $(CLAGS) -flto $^ -o $@

obj/%.o: %.c $(TOTAL_REBUILD_FILES)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

tests/cases/%/outputs/actual/viewport/opacities.data: \
	dist/% \
	tests/cases/%/inputs/viewport/opacities.data \
	tests/cases/%/inputs/viewport/reds.data \
	tests/cases/%/inputs/viewport/greens.data \
	tests/cases/%/inputs/viewport/blues.data \
	tests/cases/%/inputs/viewport/depths.data \
	tests/cases/%/inputs/texture/opacities.data \
	tests/cases/%/inputs/texture/reds.data \
	tests/cases/%/inputs/texture/greens.data \
	tests/cases/%/inputs/texture/blues.data
	mkdir -p $(dir $@)
	mkdir -p $(dir $(patsubst %/, %, $(dir $@)))texture/
	cd tests/cases/$* && ../../../dist/$*

tests/cases/%/outputs/actual/viewport/reds.data: tests/cases/%/outputs/actual/viewport/opacities.data
	echo

tests/cases/%/outputs/actual/viewport/greens.data: tests/cases/%/outputs/actual/viewport/opacities.data
	echo

tests/cases/%/outputs/actual/viewport/blues.data: tests/cases/%/outputs/actual/viewport/opacities.data
	echo

tests/cases/%/outputs/actual/viewport/depths.data: tests/cases/%/outputs/actual/viewport/opacities.data
	echo

tests/cases/%/outputs/actual/texture/reds.data: tests/cases/%/outputs/actual/viewport/opacities.data
	echo

tests/cases/%/outputs/actual/texture/greens.data: tests/cases/%/outputs/actual/viewport/opacities.data
	echo

tests/cases/%/outputs/actual/texture/blues.data: tests/cases/%/outputs/actual/viewport/opacities.data
	echo

tests/cases/%/outputs/actual/texture/opacities.data: tests/cases/%/outputs/actual/viewport/opacities.data
	echo

tests/cases/%.hex: tests/cases/%.data
	xxd $< > $@

tests/pass_markers/%: \
		tests/cases/%/outputs/expected/viewport/opacities.hex \
		tests/cases/%/outputs/expected/viewport/reds.hex \
		tests/cases/%/outputs/expected/viewport/greens.hex \
		tests/cases/%/outputs/expected/viewport/blues.hex \
		tests/cases/%/outputs/expected/viewport/depths.hex \
		tests/cases/%/outputs/expected/texture/opacities.hex \
		tests/cases/%/outputs/expected/texture/reds.hex \
		tests/cases/%/outputs/expected/texture/greens.hex \
		tests/cases/%/outputs/expected/texture/blues.hex \
		tests/cases/%/outputs/actual/viewport/opacities.hex \
		tests/cases/%/outputs/actual/viewport/reds.hex \
		tests/cases/%/outputs/actual/viewport/greens.hex \
		tests/cases/%/outputs/actual/viewport/blues.hex \
		tests/cases/%/outputs/actual/viewport/depths.hex \
		tests/cases/%/outputs/actual/texture/opacities.hex \
		tests/cases/%/outputs/actual/texture/reds.hex \
		tests/cases/%/outputs/actual/texture/greens.hex \
		tests/cases/%/outputs/actual/texture/blues.hex
	diff tests/cases/$*/outputs/expected/viewport/opacities.hex tests/cases/$*/outputs/actual/viewport/opacities.hex
	diff tests/cases/$*/outputs/expected/viewport/reds.hex tests/cases/$*/outputs/actual/viewport/reds.hex
	diff tests/cases/$*/outputs/expected/viewport/greens.hex tests/cases/$*/outputs/actual/viewport/greens.hex
	diff tests/cases/$*/outputs/expected/viewport/blues.hex tests/cases/$*/outputs/actual/viewport/blues.hex
	diff tests/cases/$*/outputs/expected/viewport/depths.hex tests/cases/$*/outputs/actual/viewport/depths.hex
	diff tests/cases/$*/outputs/expected/texture/opacities.hex tests/cases/$*/outputs/actual/texture/opacities.hex
	diff tests/cases/$*/outputs/expected/texture/reds.hex tests/cases/$*/outputs/actual/texture/reds.hex
	diff tests/cases/$*/outputs/expected/texture/greens.hex tests/cases/$*/outputs/actual/texture/greens.hex
	diff tests/cases/$*/outputs/expected/texture/blues.hex tests/cases/$*/outputs/actual/texture/blues.hex
	mkdir -p $(dir $@)
	touch $@

clean:
	rm -rf obj dist $(patsubst %, tests/cases/%/outputs/actual, $(TESTS)) tests/pass_markers
