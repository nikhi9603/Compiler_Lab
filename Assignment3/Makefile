all:
	lex compiler.l
	yacc -d compiler.y
	g++ lex.yy.c y.tab.c syntax_tree.cpp y.tab.h syntax_tree.h
	./a.out