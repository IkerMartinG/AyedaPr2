#include <iostream>
#include <string>
#include "lattice.h"

// Función para imprimir el mensaje de uso
void usage(const std::string& programName) {
    std::cerr << "Uso: " << programName << " -size <M> <N> | -init <file> -border <b> [hot|cold]" << std::endl;
    exit(EXIT_FAILURE);
}

int main(int argc, char* argv[]) {
    // Valores predeterminados
    int rows = 11;  // Número de filas predeterminado
    int cols = 11;  // Número de columnas predeterminado
    std::string initFileName = "";  // Nombre de archivo de inicialización predeterminado
    std::string borderType = "reflective";  // Tipo de borde predeterminado
    bool isOpenHotBorder = false;  // Frontera abierta caliente predeterminada

    // Procesar argumentos de línea de comandos
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "-size" && i + 2 < argc) {
            rows = std::stoi(argv[i + 1]);
            cols = std::stoi(argv[i + 2]);
        } else if (arg == "-border" && i + 1 < argc) {
            borderType = argv[i + 1];
            if (borderType == "open" && i + 2 < argc) {
                // Verificar la opción adicional para frontera caliente o fría
                std::string hotColdOption = argv[i + 2];
                if (hotColdOption == "hot") {
                    isOpenHotBorder = true;
                } else if (hotColdOption == "cold") {
                    isOpenHotBorder = false;
                }
            }
            if (borderType != "reflective" && borderType != "noborder" && borderType != "open" && borderType != "periodic") {
                std::cerr << "Error: Tipo de frontera no válido." << std::endl;
                usage(argv[0]);
            }
        } else if (arg == "-init" && i + 1 < argc) {
            initFileName = argv[i + 1];
        }
    }

    // Crear e inicializar el tablero usando el primer constructor
    Lattice lattice(rows, cols, borderType);
    lattice.setHotBorder(isOpenHotBorder);

    return 0;
}

