// cell.cpp
#include "cell.h"
#include "lattice.h"  // Incluye la definición completa de Lattice
#include <iostream>
#include <string>


Cell::Cell() : position(), state(0) {}
Cell::Cell(const Position& pos, const State initialState) : position(pos), state(initialState) {}

State Cell::getState() const {
  return state;
}

State Cell::setState(State newState) {
  state = newState;
  return state;
}

std::pair<int, int> Cell::getPos() const {
    return position;
}

int Cell::nextState(const Lattice& lattice) {
    // Definir los offsets para los vecinos
    std::vector<std::pair<int, int>> neighbors = {
        {-1, -1}, {-1, 0}, {-1, 1},
        {0, -1},           {0, 1},
        {1, -1}, {1, 0}, {1, 1}
    };

    // Calcular la posición de los vecinos y sumar sus estados
    int liveNeighbors = 0;
    for (const auto& offset : neighbors) {
        int i = position.first + offset.first;
        int j = position.second + offset.second;

        // Verificar límites según el tipo de frontera
        if (i >= 0 && i < lattice.getRow() && j >= 0 && j < lattice.getCol()) {
            liveNeighbors += lattice.getCell({i, j}).getState();
        } else {
            // Manejar vecinos fuera de los límites según el tipo de frontera
            if (lattice.getBorderType() == "open") {
              // Manejar para frontera abierta (puede incluir lógica para hot o cold)
              if(lattice.GetisHotBorder()){
                liveNeighbors++;
              }
            } else if (lattice.getBorderType() == "periodic") {
              // Manejar para frontera periódica
            } else if (lattice.getBorderType() == "reflective") {
              // Manejar para frontera reflectante
              int reflectedI = std::min(std::max(0, i), lattice.getRow() - 1);
              int reflectedJ = std::min(std::max(0, j), lattice.getCol() - 1);

              // Sumar el estado de la celda reflejada.
              liveNeighbors += lattice.getCell({reflectedI, reflectedJ}).getState();
            
            } else {
              // Manejar para tipos de frontera no reconocidos
              std::cerr << "Error: Tipo de frontera no válido." << std::endl;
            }
        }
    }

    // Aplicar reglas del juego de la vida
    if (state == 1) {
        // Reglas para células vivas
        return (liveNeighbors == 2 || liveNeighbors == 3) ? 1 : 0;
    } else {
        // Reglas para células muertas
        return (liveNeighbors == 3) ? 1 : 0;
    }
}


void Cell::updateState() {
  state = nextState_;
}

std::ostream& operator<<(std::ostream& os, const Cell& cell) {
  os << (cell.state == 1 ? 'X' : '_');
  return os;
}
