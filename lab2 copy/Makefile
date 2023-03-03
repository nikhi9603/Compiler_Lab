.phony: all clean
all:
	lex calculator.l
	yacc -d calculator.y
	g++ lex.yy.c syntax_tree.cpp y.tab.c sym_table.cpp syntax_tree.h y.tab.h sym_table.h
	#cat test1.txt
	@echo
	@echo
	./a.out < test1.txt

clean:
	rm -rf lex.yy.c y.tab.c y.tab.h y.tab.h.gch syntax_tree.h.gch