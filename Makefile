.phony: all
all:
	lex mycalc.l
	yacc -d mycalc.y
	g++ lex.yy.c y.tab.c syntax_tree.cpp syntax_tree.h y.tab.h
	./a.out < test1.txt

clean:
	rm -rf lex.yy.c y.tab.c y.tab.h y.tab.h.gch syntax_tree.h.gch a.out