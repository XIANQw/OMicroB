LEX_C = flex
YACC_C = yacc
GCC = gcc

montage: parser ast.h ast.c montage.c
	$(GCC) -c ast.c
	$(GCC) -c montage.c
	$(GCC) -o montage ast.o montage.o lex.yy.o y.tab.o -lm -ll
	@cp circuit.txt /tmp/
	# @mv montage /tmp/

parser: lexer.lex parser.y
	$(YACC_C) -d parser.y
	$(LEX_C) lexer.lex
	$(GCC) -c lex.yy.c
	$(GCC) -c y.tab.c

clean:
	rm lex.yy.*
	rm y.tab.*
	rm *.o
	rm /tmp/montage
