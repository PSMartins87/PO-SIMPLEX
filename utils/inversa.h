#include <iostream>
void preencheIdentidade(double **matriz, int tamanhoMatriz)
{
    for (int i = 0; i < tamanhoMatriz; i++)
    {
        for (int j = 0; j < tamanhoMatriz; j++)
        {
            if (i == j)
            {
                matriz[i][j] = 1;
            }
        }
    }
}

void preencherMatrix() {}
void mostrarMatriz(double **matriz, int tamanhoMatriz)
{
    for (int i = 0; i < tamanhoMatriz; i++)
    {
        for (int j = 0; j < tamanhoMatriz; j++)
        {
            std::cout << matriz[i][j] << "    " << std::endl;
        }
        std::cout << std::endl;
    }
}

void trocarLinhas(double **matriz, double **identidade, int linha1, int linha2, int tamanhoMatriz)
{
    for (int k = 0; k < tamanhoMatriz; k++)
    {
        double temp = matriz[linha1][k];
        matriz[linha1][k] = matriz[linha2][k];
        matriz[linha2][k] = temp;

        temp = identidade[linha1][k];
        identidade[linha1][k] = identidade[linha2][k];
        identidade[linha2][k] = temp;
    }
}

void eliminarElemento(double **matriz, double **identidade, int linha1, int linha2, int coluna, int tamanhoMatriz)
{
    double fator = matriz[linha2][coluna] / matriz[linha1][coluna];
    for (int k = 0; k < tamanhoMatriz; k++)
    {
        matriz[linha2][k] -= fator * matriz[linha1][k];
        identidade[linha2][k] -= fator * identidade[linha1][k];
    }
}

void zerarElementosAcimaDiagonal(double **matriz, double **identidade, int tamanhoMatriz)
{
    for (int i = 0; i < tamanhoMatriz; i++)
    {
        for (int j = i + 1; j < tamanhoMatriz; j++)
        {
            eliminarElemento(matriz, identidade, i, j, i, tamanhoMatriz);
        }
    }
}

void zerarElementosAbaixoDiagonal(double **matriz, double **identidade, int tamanhoMatriz)
{
    for (int i = tamanhoMatriz - 1; i >= 0; i--)
    {
        for (int j = i - 1; j >= 0; j--)
        {
            eliminarElemento(matriz, identidade, i, j, i, tamanhoMatriz);
        }
    }
}

void calcularInversa(double **matriz, double **identidade, double **inversa, int tamanhoMatriz)
{
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

double calculaDeterminante(double **matriz, int tamanhoMatriz)
{
    if (tamanhoMatriz == 1)
    {
        return matriz[0][0];
    }
    else
    {
        double determinante = 0.0;
        int j, k, jAux, iAux;

        for (int i = 0; i < tamanhoMatriz; i++)
        {
            if (matriz[0][i] != 0)
            {
                double **aux;
                aux = new double *[tamanhoMatriz];
                for (int i = 0; i < tamanhoMatriz; i++)
                {
                    aux[i] = new double[tamanhoMatriz];
                }
                int iAux = 0;
                int jAux = 0;
                for (j = 1; j < tamanhoMatriz; j++)
                {
                    for (k = 0; k < tamanhoMatriz; k++)
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
