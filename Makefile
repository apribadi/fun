.PHONY: clean default

default: out/fun

CFLAGS = \
	-std=c2x \
	-O2 \
	-Wall \
	-Wconversion \
	-Wdouble-promotion \
	-Weverything \
	-Wextra \
	-Wno-c++98-compat \
	-Wno-constant-logical-operand \
	-Wno-declaration-after-statement \
	-Wno-fixed-enum-extension \
	-Wno-gnu-statement-expression-from-macro-expansion \
	-Wno-missing-prototypes \
	-Wno-pre-c2x-compat \
	-Wno-shadow \
	-Wno-shift-op-parentheses \
	-Wno-unsafe-buffer-usage \
	-Wno-used-but-marked-unused \
	-ffp-contract=off \
	-fno-math-errno \
	-fno-omit-frame-pointer \
	-fno-slp-vectorize \
	-pedantic

clean:
	rm -f out/*

out/fun: src/fun.c src/prelude.h src/isa_vm.h out/vm.o out/rt.o
	clang -o $@ $< out/vm.o out/rt.o $(CFLAGS) -DNDEBUG

out/vm.o: src/vm.c src/vm.h src/isa_vm.h src/prelude.h
	clang -c -o $@ $< $(CFLAGS) -DNDEBUG

out/rt.o: src/rt.c src/rt.h src/prelude.h
	clang -c -o $@ $< $(CFLAGS) -DNDEBUG
