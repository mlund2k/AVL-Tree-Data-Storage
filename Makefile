CC=g++
DEV=-Wall -g -std=c++14
OPT=-O3 -std=c++14

.PHONY: all
all: AVLCommands CreateData

CreateData: CreateData.cxx json.hpp
	$(CC) $(OPT) CreateData.cxx -o CreateData.exe

AVLCommands: AVLCommands.cxx AVL.o json.hpp
	$(CC) $(DEV) AVLCommands.cxx AVL.o -o AVLCommands.exe

AVL.o: AVL.cpp AVL.h
	$(CC) $(DEV) -c AVL.cpp

# Build
.PHONY: clean
clean:
	rm -f *.o
	rm -f *.exe
	rm -rf *dSYM

.PHONY: update
update:
	make clean
	make all
