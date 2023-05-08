all:
	lex compiler.l
	yacc -d compiler.y
	g++ lex.yy.c y.tab.c syntax_tree.cpp sym_table.cpp y.tab.h syntax_tree.h sym_table.h -o compiler

tests:
	@read -p "Enter testcase filename present in 'testcases' directory: " TESTFILE ;\
	echo ;\
	./compiler testcases/$$TESTFILE

test1:
	@echo "testcase : bin_search.txt";
	@echo;
	@./compiler testcases/bin_search.txt

test2:
	@echo "testcase : bubble.txt";
	@echo;
	@./compiler testcases/bubble.txt

test3:
	@echo "testcase : factorial_recursive.txt";
	@echo;
	@./compiler testcases/factorial_recursive.txt

test4:
	@echo "testcase : Sum_Recursive,src.txt";
	@echo;
	@./compiler testcases/Sum_Recursive,src.txt

test5:
	@echo "testcase : test1.txt";
	@echo;
	@./compiler testcases/test1.txt



clean:
	rm -rf compiler y.tab.c y.tab.h lex.yy.c 
