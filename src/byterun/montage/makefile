LEX_C = flex
YACC_C = yacc
GCC = gcc

get_env: parser ast.h ast.c get_env.c
	$(GCC) -c ast.c
	$(GCC) -c get_env.c
	$(GCC) -o get_env ast.o get_env.o lex.yy.o y.tab.o -lm -ll
	@cp circuit.txt /tmp/
	@mv get_env /tmp/

parser: lexer.lex parser.y
	$(YACC_C) -d parser.y
	$(LEX_C) lexer.lex
	$(GCC) -c lex.yy.c
	$(GCC) -c y.tab.c

clean:
	rm lex.yy.*
	rm y.tab.*
	rm *.o
	rm /tmp/get_env
