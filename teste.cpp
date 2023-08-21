#include "utils/InstanceReader.h"
#include "utils/Simplex.h"
//#include "utils/matriz.h"

void swapRows(std::vector<std::vector<double>>& matrix, int row1, int row2) {
    std::vector<double> temp = matrix[row1];
    matrix[row1] = matrix[row2];
    matrix[row2] = temp;
}

// Função para dividir uma linha pelo pivô
void divideRow(std::vector<std::vector<double>>& matrix, int row, double divisor) {
    int cols = matrix[row].size();
    for (int j = 0; j < cols; ++j) {
        matrix[row][j] /= divisor;
    }
}

// Função para subtrair uma linha multiplicada por um fator de outra linha
void subtractRow(std::vector<std::vector<double>>& matrix, int row1, int row2, double factor) {
    int cols = matrix[row1].size();
    for (int j = 0; j < cols; ++j) {
        matrix[row1][j] -= factor * matrix[row2][j];
    }
}

// Função para calcular a inversa da matriz usando Gauss-Jordan
std::vector<std::vector<double>> inverseUsingGaussJordan(std::vector<std::vector<double>> matrix) {
    int n = matrix.size();
    std::vector<std::vector<double>> augmentedMatrix(n, std::vector<double>(2 * n, 0.0));

    // Criando a matriz aumentada [A | I]
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            augmentedMatrix[i][j] = matrix[i][j];
        }
        augmentedMatrix[i][i + n] = 1.0;
    }

    // Aplicando o método de Gauss-Jordan
    for (int i = 0; i < n; ++i) {
        // Pivoteamento: colocando o maior elemento na coluna como pivô
        int maxRow = i;
        for (int j = i + 1; j < n; ++j) {
            if (abs(augmentedMatrix[j][i]) > abs(augmentedMatrix[maxRow][i])) {
                maxRow = j;
            }
        }
        swapRows(augmentedMatrix, i, maxRow);

        double pivot = augmentedMatrix[i][i];
        divideRow(augmentedMatrix, i, pivot);

        for (int j = 0; j < n; ++j) {
            if (j != i) {
                double factor = augmentedMatrix[j][i];
                subtractRow(augmentedMatrix, j, i, factor);
            }
        }
    }

    // Extrair a matriz inversa da parte à direita da matriz aumentada
    std::vector<std::vector<double>> inverse(n, std::vector<double>(n, 0.0));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            inverse[i][j] = augmentedMatrix[i][j + n];
        }
    }

    return inverse;
}

int main()
{
    LPInstance instance = loadFile();
    simplex(instance);

    return 0;
}