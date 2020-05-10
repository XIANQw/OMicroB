include ../../etc/Makefile.conf

mbserial.hex: mbserial.ml
	$(BIN)/omicrob -device=microbit -arch 32 -v mbserial.ml -mlopt -verbose -stack-size 2048 -trace 0

clean:
	rm -f *.cmi *.cmo *.byte *.c *.arm_o *.elf *.arm_elf *.hex
