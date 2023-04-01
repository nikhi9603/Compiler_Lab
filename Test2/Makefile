all: clean
	lex compiler.l
	yacc -d compiler.y
	g++ lex.yy.c y.tab.c syntax_tree.cpp sym_table.cpp y.tab.h syntax_tree.h sym_table.h -o compiler

clean:
	rm -rf compiler y.tab.c y.tab.h lex.yy.c 
