.phony: all clean
all:
	lex calculator.l
	yacc -d calculator.y
	g++ lex.yy.c  y.tab.c syntax_tree.cpp sym_table.cpp  syntax_tree.h sym_table.h y.tab.h 
	./a.out < test.txt

clean:
	rm -rf lex.yy.c y.tab.c y.tab.h y.tab.h.gch syntax_tree.h.gch sym_table.h.gch a.out