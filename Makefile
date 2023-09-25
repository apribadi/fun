.PHONY: clean default

default: out/fun

CFLAGS = \
	-std=c2x \
	-O2 \
	-Wall \
	-Wconversion \
	-Wdouble-promotion \
	-Werror \
	-Weverything \
	-Wextra \
	-Wno-c++98-compat \
	-Wno-constant-logical-operand \
	-Wno-declaration-after-statement \
	-Wno-fixed-enum-extension \
	-Wno-gnu-statement-expression-from-macro-expansion \
	-Wno-missing-prototypes \
	-Wno-pre-c2x-compat \
	-Wno-used-but-marked-unused \
	-Wno-shadow \
	-Wno-shift-op-parentheses \
	-Wno-unsafe-buffer-usage \
	-ffp-contract=off \
	-fno-math-errno \
	-fno-omit-frame-pointer \
	-fno-slp-vectorize \
	-pedantic

clean:
	rm -f out/*

out/fun: src/fun.c src/prelude.h out/bvm.o out/rts.o
	clang -o $@ $< out/bvm.o out/rts.o $(CFLAGS) -DNDEBUG

out/bvm.o: src/bvm.c src/bvm.h src/prelude.h
	clang -c -o $@ $< $(CFLAGS) -DNDEBUG

out/rts.o: src/rts.c src/rts.h src/prelude.h
	clang -c -o $@ $< $(CFLAGS) -DNDEBUG
