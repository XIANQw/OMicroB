include ../../etc/Makefile.conf

SOURCES := test.ml
TARGETS := test.byte test.c test.elf test.avr test.hex

all: $(TARGETS)

$(TARGETS): $(SOURCES)
	$(BIN)/omicrob -device=arduboy -v $^ -mlopt -verbose -stack-size 2048 -heap-size 2048 -trace 0

flash: test.hex
	$(BIN)/omicrob -flash -sudo $<

simul: test.elf
	./$<

clean:
	@rm -f *.cmo *.cmi
	@rm -rf *.elf.dSYM
	@rm -f $(TARGETS)

.PHONY: all flash clean
