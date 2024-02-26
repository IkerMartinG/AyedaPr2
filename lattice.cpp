#include "lattice.h"
#include "cell.h"
#include <iostream>
#include <fstream>

Lattice::Lattice(int row, int col, const std::string& borderType) : row(row), col(col), borderType(borderType) {
    // Reservar memoria para el puntero a puntero
    grid.resize(row, std::vector<Cell>(col));
    
    RequestLiveCells();
}

void Lattice::RequestLiveCells() {
    int liveCellCount;
    std::cout << "Ingrese el número de células vivas para la configuración inicial: ";
    std::cin >> liveCellCount;

    std::cout << "Ingrese las posiciones de las células vivas (Formato: fila columna):" << std::endl;

    // Inicializar todas las células a 0
    for (int j = 0; j < col; ++j) {
        for (int i = 0; i < row; ++i) {
            grid[i][j] = Cell({i, j}, 0);
        }
    }

    // Solicitar las posiciones de las células vivas y establecer el estado a 1 directamente
    for (int i = 0; i < liveCellCount; ++i) {
        int rowIndex, colIndex;
        std::cout << "Célula " << i + 1 << ": ";
        std::cin >> rowIndex >> colIndex;

        // Validar que las posiciones estén dentro del rango del tablero
        if (rowIndex >= 0 && rowIndex < row && colIndex >= 0 && colIndex < col) {
            // Establecer el estado de la célula como "viva"
            grid[rowIndex][colIndex].setState(1);
        } else {
            std::cerr << "Error: Posición fuera del rango del tablero. Vuelva a ingresar las coordenadas." << std::endl;
            --i; // Retroceder el índice para repetir la entrada de la célula
        }
    }
}

Lattice::Lattice(const char* fileName) {
    std::ifstream file(fileName);

    if (file.is_open()) {
        // Leer el número de filas y columnas
        file >> row >> col;

        // Reservar memoria para el puntero a puntero
        grid.resize(row, std::vector<Cell>(col));

        // Leer el contenido del archivo y asignarlo a las células
        for (int j = 0; j < col; ++j) {
            for (int i = 0; i < row; ++i) {
                char cellState;
                file >> cellState;

                // Asignar el estado a la célula
                grid[i][j].setState((cellState == 'X') ? 1 : 0);
            }
        }

        file.close();
    } else {
        std::cerr << "Error: No se pudo abrir el archivo '" << fileName << "'." << std::endl;
    }
}

// Dentro de la clase Lattice, en el método Population
std::size_t Lattice::Population() const {
    std::size_t population = 0;
    for (int j = 0; j < col; ++j) {
        for (int i = 0; i < row; ++i) {
            population += (grid[i][j].getState() == 1) ? 1 : 0;
        }
    }
    return population;
}

int Lattice::getCol() const {
    return col;
}

int Lattice::getRow() const {
    return row;
}

std::string Lattice::getBorderType() const{
    return borderType;
}

// Dentro de la clase Lattice
const Cell& Lattice::getCell(Position pos) const {
    int i = pos.first;
    int j = pos.second;

    if (i >= 0 && i < row && j >= 0 && j < col) {
        return grid[i][j];
    } else {
        std::cerr << "Error: Posición fuera de rango." << std::endl;
        exit(EXIT_FAILURE);
    }
}

void Lattice::setHotBorder(bool hotBorderValue)
{
    hotBorder = hotBorderValue;
}

bool Lattice::GetisHotBorder() const {
  return hotBorder;
}

// En lattice.cpp
void Lattice::expandGrid() {
    // Duplica el tamaño de la matriz en ambas dimensiones
    int newRow = row * 2;
    int newCol = col * 2;

    // Crea una nueva matriz más grande
    std::vector<std::vector<Cell>> newGrid(newRow, std::vector<Cell>(newCol));

    // Inicializa todas las nuevas células a un estado específico (por ejemplo, muertas)
    for (int i = 0; i < newRow; ++i) {
        for (int j = 0; j < newCol; ++j) {
            newGrid[i][j] = Cell({i, j}, 0);  // Aquí, el segundo parámetro 0 representa el estado muerto
        }
    }

    // Copia las células existentes a la nueva matriz
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            newGrid[i][j] = grid[i][j];
        }
    }

    // Asigna la nueva matriz como el retículo actual
    grid = std::move(newGrid);

    // Actualiza las dimensiones del retículo
    row = newRow;
    col = newCol;
}


// Implementación de nextGeneration
void Lattice::nextGeneration() {
    
    if (borderType == "no-border") {
        // Verifica si hay células vivas en el borde antes de cada expansión
        bool expandNeeded = false;

        // Verifica el borde superior e inferior
        for (int j = 0; j < col; ++j) {
            if (grid[0][j].getState() == 1 || grid[row - 1][j].getState() == 1) {
                expandNeeded = true;
                break;
            }
        }

        // Verifica los bordes izquierdo y derecho
        for (int i = 0; i < row; ++i) {
            if (grid[i][0].getState() == 1 || grid[i][col - 1].getState() == 1) {
                expandNeeded = true;
                break;
            }
        }

        // Expande el retículo si es necesario
        if (expandNeeded) {
            expandGrid();
        }
    }

    // Actualizar el estado de cada célula en la matriz
    for (int j = 0; j < col; ++j) {
        for (int i = 0; i < row; ++i) {
            grid[i][j].nextState(*this);
        }
    }

    // Actualizar el estado de cada célula en la matriz
    for (int j = 0; j < col; ++j) {
        for (int i = 0; i < row; ++i) {
            grid[i][j].updateState();
        }
    }
}


// En lattice.cpp
const Cell& Lattice::operator[](const Position& pos) const {
    int i = pos.first;
    int j = pos.second;

    if (i >= 0 && i < row && j >= 0 && j < col) {
        return grid[i][j];
    } else {
        std::cerr << "Error: Posición fuera de rango." << std::endl;
        exit(EXIT_FAILURE);
    }
}

// Implementación del operador de inserción
std::ostream& operator<<(std::ostream& os, const Lattice& lattice) {
    for (int j = 0; j < lattice.col; ++j) {
        for (int i = 0; i < lattice.row; ++i) {
            os << lattice.grid[i][j];
        }
        os << std::endl;
    }
    return os;
}