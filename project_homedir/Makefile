.PHONY: doxygen

SRC_DIR = src
SLP_DIR = src/SLP_interpreter
UTILS = src/utils.cpp

GCC = gcc -std=c17 -g
GPP = g++ -std=c++20 -g

SLP_interpreter.out:
	$(GPP) $(SLP_DIR)/main.cpp $(SLP_DIR)/grammar_interpreter.cpp $(UTILS) -o out/SLP_interpreter.out

SLP_interpreter_C.out:
	$(GCC) $(SRC_DIR)/SLP_interpreter_C.c -o out/SLP_interpreter_C.out

doxygen:
	doxygen Doxyfile
	cp doc/latex/annotatedstructs.tex doc/latex/annotated.tex
	cd doc/latex && make && cd ../..
	make latex_clean

latex_clean:
	mv doc/latex/refman.pdf doc/refman.pdf
	rm -r doc/latex/*
	mv doc/refman.pdf doc/latex/refman.pdf