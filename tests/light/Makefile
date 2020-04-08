include ../../etc/Makefile.conf

SOURCES := light.ml light_noalloc.ml light_ocalustre.ml
TARGETS := light.byte light.c light.elf light.avr light.hex

all: $(TARGETS)

$(TARGETS): $(SOURCES)
	$(BIN)/omicrob -device=arduboy -v $^ -gc MAC -arch 16 -no-flash-heap -stack-size 300 -heap-size 2000 -trace 0 -o light.byte
	# $(BIN)/omicrob -v $^ -gc SC -arch 16 -no-flash-heap -no-flash-globals -stack-size 300 -heap-size 800

flash: light.hex
	sudo ./reset.py $(ATTY)
	$(BIN)/omicrob -v -flash -sudo $<


simul: light.elf
	./$< '../../bin/omicrob_pin_simulator circuit.txt'

clean:
	@rm -f *.cmo *.cmi
	@rm -rf *.elf.dSYM
	@rm -f $(TARGETS)

.PHONY: all flash clean
