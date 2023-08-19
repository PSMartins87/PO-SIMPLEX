#include <iostream>
#include <vector>

/**
 * @brief Preenche uma matriz com a matriz identidade.
 *
 * @param matriz A matriz a ser preenchida.
 * @param tamanhoMatriz O tamanho da matriz (número de linhas e colunas).
 */
void preencheIdentidade(std::vector<std::vector<double>> &matriz, int tamanhoMatriz)
{
    matriz.assign(tamanhoMatriz, std::vector<double>(tamanhoMatriz, 0.0));
    for (int i = 0; i < tamanhoMatriz; i++)
    {
        matriz[i][i] = 1;
    }
}

/**
 * @brief Multiplica duas matrizes.
 *
 * @param matriz1 A primeira matriz.
 * @param matriz2 A segunda matriz.
 * @return O resultado da multiplicação das matrizes.
 */
std::vector<std::vector<double>> multiplicarMatrizes(const std::vector<std::vector<double>> &matriz1, const std::vector<std::vector<double>> &matriz2)
{
    int linhasMatriz1 = matriz1.size();
    int colunasMatriz1 = matriz1[0].size();
    int linhasMatriz2 = matriz2.size();
    int colunasMatriz2 = matriz2[0].size();

    if (colunasMatriz1 != linhasMatriz2)
    {
        std::cout << "O número de colunas da matriz1 deve ser igual ao número de linhas da matriz2." << std::endl;
        return std::vector<std::vector<double>>();
    }
    std::vector<std::vector<double>> resultado(linhasMatriz1, std::vector<double>(colunasMatriz2, 0.0));

    for (int i = 0; i < linhasMatriz1; i++)
    {
        for (int j = 0; j < colunasMatriz2; j++)
        {
            for (int k = 0; k < colunasMatriz1; k++)
            {
                resultado[i][j] += matriz1[i][k] * matriz2[k][j];
            }
        }
    }

    return resultado;
}

/**
 * @brief Mostra os elementos de uma matriz.
 *
 * @param matriz A matriz a ser exibida.
 */
void mostrarMatriz(const std::vector<std::vector<double>> &matriz)
{
    for (const auto &linha : matriz)
    {
        for (double elemento : linha)
        {
            std::cout << elemento << "    ";
        }
        std::cout << std::endl;
    }
}

/**
 * @brief Troca duas linhas em uma matriz.
 *
 * @param matriz A matriz na qual as linhas serão trocadas.
 * @param identidade A matriz identidade correspondente.
 * @param linha1 O índice da primeira linha.
 * @param linha2 O índice da segunda linha.
 * @param tamanhoMatriz O tamanho da matriz (número de linhas e colunas).
 */
void trocarLinhas(std::vector<std::vector<double>> &matriz, std::vector<std::vector<double>> &identidade, int linha1, int linha2, int tamanhoMatriz)
{
    for (int k = 0; k < tamanhoMatriz; k++)
    {
        std::swap(matriz[linha1][k], matriz[linha2][k]);
        std::swap(identidade[linha1][k], identidade[linha2][k]);
    }
}

/**
 * @brief Elimina um elemento de uma matriz para fazer a diagonal se tornar 1 e abaixo dela zeros.
 *
 * @param matriz A matriz na qual a eliminação será realizada.
 * @param identidade A matriz identidade correspondente.
 * @param linha1 A linha alvo (para se tornar 1).
 * @param linha2 A linha na qual a eliminação será feita (para se tornar zero).
 * @param coluna A coluna alvo.
 * @param tamanhoMatriz O tamanho da matriz (número de linhas e colunas).
 */
void eliminarElemento(std::vector<std::vector<double>> &matriz, std::vector<std::vector<double>> &identidade, int linha1, int linha2, int coluna, int tamanhoMatriz)
{
    double fator = matriz[linha2][coluna] / matriz[linha1][coluna];
    for (int k = 0; k < tamanhoMatriz; k++)
    {
        matriz[linha2][k] -= fator * matriz[linha1][k];
        identidade[linha2][k] -= fator * identidade[linha1][k];
    }
}

/**
 * @brief Zera os elementos acima da diagonal principal de uma matriz.
 *
 * @param matriz A matriz a ser processada.
 * @param identidade A matriz identidade correspondente.
 * @param tamanhoMatriz O tamanho da matriz (número de linhas e colunas).
 */
void zerarElementosAcimaDiagonal(std::vector<std::vector<double>> &matriz, std::vector<std::vector<double>> &identidade, int tamanhoMatriz)
{
    for (int i = 0; i < tamanhoMatriz; i++)
    {
        for (int j = i + 1; j < tamanhoMatriz; j++)
        {
            eliminarElemento(matriz, identidade, i, j, i, tamanhoMatriz);
        }
    }
}

/**
 * @brief Zera os elementos abaixo da diagonal principal de uma matriz.
 *
 * @param matriz A matriz a ser processada.
 * @param identidade A matriz identidade correspondente.
 * @param tamanhoMatriz O tamanho da matriz (número de linhas e colunas).
 */
void zerarElementosAbaixoDiagonal(std::vector<std::vector<double>> &matriz, std::vector<std::vector<double>> &identidade, int tamanhoMatriz)
{
    for (int i = tamanhoMatriz - 1; i >= 0; i--)
    {
        for (int j = i - 1; j >= 0; j--)
        {
            eliminarElemento(matriz, identidade, i, j, i, tamanhoMatriz);
        }
    }
}

/**
 * @brief Calcula a matriz inversa usando eliminação de Gauss-Jordan.
 *
 * @param matriz A matriz para a qual a inversa será calculada.
 * @param identidade A matriz identidade correspondente.
 * @param tamanhoMatriz O tamanho da matriz (número de linhas e colunas).
 */
void calcularInversa(
    std::vector<std::vector<double>> &matriz, 
    std::vector<std::vector<double>> &identidade, 
    int tamanhoMatriz
)
{
    std::vector<std::vector<double>> inversa;
    zerarElementosAcimaDiagonal(matriz, identidade, tamanhoMatriz);
    zerarElementosAbaixoDiagonal(matriz, identidade, tamanhoMatriz);
    for (int i = 0; i < tamanhoMatriz; i++)
    {
        double pivo = matriz[i][i];
        for (int j = 0; j < tamanhoMatriz; j++)
        {
            inversa[i][j] = identidade[i][j] / pivo;
        }
    }
}

/**
 * @brief Calcula o determinante de uma matriz usando o método de Laplace.
 *
 * @param matriz A matriz para a qual o determinante será calculado.
 * @param tamanhoMatriz O tamanho da matriz (número de linhas e colunas).
 * @return O determinante da matriz.
 */
double calculaDeterminante(const std::vector<std::vector<double>> &matriz, int tamanhoMatriz)
{
    if (tamanhoMatriz == 1)
    {
        return matriz[0][0];
    }
    else
    {
        double determinante = 0.0;
        for (int i = 0; i < tamanhoMatriz; i++)
        {
            if (matriz[0][i] != 0)
            {
                std::vector<std::vector<double>> aux(tamanhoMatriz - 1, std::vector<double>(tamanhoMatriz - 1, 0.0));
                int iAux = 0;
                int jAux = 0;
                for (int j = 1; j < tamanhoMatriz; j++)
                {
                    for (int k = 0; k < tamanhoMatriz; k++)
                    {
                        if (k != i)
                        {
                            aux[iAux][jAux] = matriz[j][k];
                            jAux++;
                        }
                    }
                    iAux++;
                    jAux = 0;
                }
                double fator = (i % 2 == 0) ? matriz[0][i] : -matriz[0][i];
                determinante = determinante + fator * calculaDeterminante(aux, tamanhoMatriz - 1);
            }
        }
        return determinante;
    }
}
