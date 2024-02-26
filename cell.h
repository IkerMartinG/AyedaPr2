// cell.h
#ifndef CELL_H
#define CELL_H

#include <iostream>
#include <utility>


typedef std::pair<int, int> Position;
typedef int State;

class Lattice;  // Declaración anticipada

class Cell {
public:
    Cell();
    Cell(const Position& pos, State state);

    int getState() const;
    int setState(State newState);
    std::pair<int, int> getPos() const;


    int nextState(const Lattice& lattice);
    void updateState();

    friend std::ostream& operator<<(std::ostream& os, const Cell& cell);

private:
    Position position;
    State nextState_;
    State state;
};

#endif // CELL_H
