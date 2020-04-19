include ../../etc/Makefile.conf

SOURCES := test.ml
TARGETS := test.byte test.c test.elf

all: $(TARGETS)

$(TARGETS): $(SOURCES)
	$(BIN)/omicrob -device=arduboy -v $^ -mlopt -verbose -stack-size 200 -heap-size 200 -trace 2

clean:
	@rm -f *.cmo *.cmi
	@rm -f *.byte *.c *.elf

.PHONY: all flash clean
