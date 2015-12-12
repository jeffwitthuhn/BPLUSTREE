all: test 
test: main.cpp Block.cpp Block.h BPlusTree.cpp BPlusTree.h ReplacementSelection.cpp ReplacementSelection.h
	g++ -c Block.cpp -std=c++11
	g++ -c ReplacementSelection.cpp -std=c++11
	g++ -c BPlusTree.cpp -std=c++11
	g++ main.cpp BPlusTree.o Block.o ReplacementSelection.o -std=c++11 -o run

