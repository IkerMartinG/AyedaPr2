CC = g++
CFLAGS = -std=c++11

all: automata

automata: cell.cpp lattice.cpp main.cpp
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f automata
