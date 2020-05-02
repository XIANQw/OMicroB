include ../etc/Makefile.conf

TARGETS := test.elf test.hex

all: $(TARGETS)

$(TARGETS): test.ml
	$(BIN)/omicrob -v -device=microbit $^ -o $@

clean:
	rm -f *.cmi *.cmo *.byte test.c *.arm_o *.elf *.arm_elf *.hex
