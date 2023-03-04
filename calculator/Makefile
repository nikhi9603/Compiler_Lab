.phony: all

all:
	lex calculator.l
	yacc -d calculator.y
	gcc lex.yy.c y.tab.c y.tab.h -o calculator.out

tests:
	@read -p "Enter current directory testcase filename: " TESTFILE ;\
	./calculator.out < $$TESTFILE
clean:
	rm -rf lex.yy.c y.tab.c y.tab.h calculator.out 
