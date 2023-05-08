all:
	lex compiler.l
	yacc -d compiler.y
	g++ lex.yy.c y.tab.c syntax_tree.cpp sym_table.cpp y.tab.h syntax_tree.h sym_table.h -o compiler

tests:
	@read -p "Enter testcase filename present in 'testcases' directory: " TESTFILE ;\
	echo ;\
	test -f testcases/$$TESTFILE || echo "Testcase file not found" ; \
	test -f outputs/output.c && rm -rf outputs/output.c ;\
	touch outputs/output.c ; \
	test -f testcases/$$TESTFILE && echo "./compiler testcases/$$TESTFILE outputs/output.c" ; \
	test -f testcases/$$TESTFILE && ./compiler testcases/$$TESTFILE outputs/output.c \
	echo ; \
	test -f outputs/output.c && test -f testcases/$$TESTFILE && echo "gcc outputs/output.c"; \
	test -f outputs/output.c && test -f testcases/$$TESTFILE && gcc outputs/output.c ;\
	test -f outputs/output.c && test -f testcases/$$TESTFILE && ./a.out;\
	echo;\
	test -f outputs/output.c ||  echo "Output file with C code is not generated due to errors in the testcase"; 

clean:
	rm -rf compiler y.tab.c y.tab.h lex.yy.c 
