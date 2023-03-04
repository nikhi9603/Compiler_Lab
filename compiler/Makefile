intermediateFiles = compiler src/lex.yy.c src/y.tab.c include/y.tab.h 

all: clean compile 
	mkdir -p bin
	mkdir -p testcases
	mv $(intermediateFiles) -t bin

compile: mv_ifiles 
	gcc src/*.c -I include/ -o compiler

mv_ifiles: parse
	mv lex.yy.c y.tab.c -t src/
	mv y.tab.h -t include/
parse:
	lex src/compiler.l
	yacc -d src/compiler.y

tests:
	@read -p "Enter testcase filename present in 'testcases': " TESTFILE ;\
	bin/compiler < testcases/$$TESTFILE

clean:
	rm -rf bin/*

