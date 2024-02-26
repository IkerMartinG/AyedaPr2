// lattice.h
#ifndef LATTICE_H
#define LATTICE_H

#include "cell.h"
#include <string>
#include <vector>
#include <fstream>  // Agrega esta línea para ifstream

class Lattice {
public:
    Lattice(int row, int col, const std::string& borderType);
    Lattice(const char* fileName);

    void RequestLiveCells();
    std::size_t Population() const;
    void nextGeneration();
    int getRow() const;
    int getCol() const;
    std::string getBorderType() const;
    const Cell& getCell(Position pos) const;
    void setHotBorder(bool hotBorderValue);
    bool GetisHotBorder() const;
    void expandGrid();

    // Sobrecarga del operador []
    const Cell& operator[](const Position& pos) const;
    friend std::ostream& operator<<(std::ostream& os, const Lattice& lattice);

private:
    std::vector<std::vector<Cell>> grid;
    int row;  // Número de filas
    int col;  // Número de columnas
    std::string borderType; // Quité la referencia aquí
    bool hotBorder;
};

#endif // LATTICE_H
