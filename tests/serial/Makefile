include ../../etc/Makefile.conf

SOURCES := serial.ml
TARGETS := serial.byte serial.c serial.elf serial.avr serial.hex
ATTY := /dev/tty.usbmodem*

all: $(TARGETS)

$(TARGETS): $(SOURCES)
	$(BIN)/omicrob -device=arduboy -v $^ -gc MAC -arch 16 -no-flash-heap -stack-size 800 -heap-size 800
	# $(BIN)/omicrob -v $^ -gc MAC -arch 32 -trace 3 -no-flash-heap -no-flash-globals -stack-size 300 -heap-size 300 -cxxopt -DDEBUG -avrcxxopt -DDEBUG
	# $(BIN)/omicrob -v $^ -gc SC -arch 16 -no-flash-heap -no-flash-globals -stack-size 300 -heap-size 800

flash: serial.hex
	sudo ./reset.py $(ATTY)
	$(BIN)/omicrob -v -flash -sudo $<

clean:
	@rm -f *.cmo *.cmi
	@rm -rf *.elf.dSYM
	@rm -f $(TARGETS)

.PHONY: all flash clean
