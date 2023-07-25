#include "utils/inversa.h"
int main()
{
    int tamanhoMatriz = 0;
    std::cout << "Tamanho da matriz" << std::endl;
    std::cin >> tamanhoMatriz;

    double **matrizIdentidade = {0};
    matrizIdentidade = new double *[tamanhoMatriz];
    for (int i = 0; i < tamanhoMatriz; i++)
    {
        matrizIdentidade[i] = new double[tamanhoMatriz];
    }

    double **matriz = {0};
    matriz = new double *[tamanhoMatriz];
    for (int i = 0; i < tamanhoMatriz; i++)
    {
        matriz[i] = new double[tamanhoMatriz];
    }

    double **inversa = {0};
    inversa = new double *[tamanhoMatriz];
    for (int i = 0; i < tamanhoMatriz; i++)
    {
        inversa[i] = new double[tamanhoMatriz];
    }
    
    preencheIdentidade(matrizIdentidade, tamanhoMatriz);
    system("clear");

    if (calculaDeterminante(matriz, tamanhoMatriz) != 0)
    {
        calcularInversa(matriz, matrizIdentidade, inversa, tamanhoMatriz);
        mostrarMatriz(inversa, tamanhoMatriz);
    }
    else
    {
        std::cout << "matriz não invertivel" << std::endl;
    }
}