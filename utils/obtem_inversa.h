#ifndef OBTEM_INVERSA_H
#define OBTEM_INVERSA_H

#include <iostream>
#include <vector>

// Calcula a inversa da matriz passada
std::vector<std::vector<double>> calcula_inversa(
    std::vector<std::vector<double>> matriz, std::vector<std::vector<double>> identidade
);

// Funções responsáveis por encontrar a inversa de uma matriz
//-----------------------------------------------------------
void eliminarElemento(
    std::vector<std::vector<double>>& matriz, std::vector<std::vector<double>>& identidade,
    int linha1, int linha2, int coluna, int tamanhoMatriz
){
    double fator = matriz[linha2][coluna] / matriz[linha1][coluna];
    for (int k = 0; k < tamanhoMatriz; k++)
    {
        matriz[linha2][k] -= fator * matriz[linha1][k];
        identidade[linha2][k] -= fator * identidade[linha1][k];
    }
}

void zerarElementosAbaixoDiagonal(
    std::vector<std::vector<double>>& matriz, std::vector<std::vector<double>>& identidade, int tamanhoMatriz
){
    for (int i = tamanhoMatriz - 1; i >= 0; i--)
    {
        for (int j = i - 1; j >= 0; j--)
        {
            eliminarElemento(matriz, identidade, i, j, i, tamanhoMatriz);
        }
    }
}

void zerarElementosAcimaDiagonal(
    std::vector<std::vector<double>>& matriz, std::vector<std::vector<double>>& identidade, int tamanhoMatriz
){
    for (size_t i = 0; i < tamanhoMatriz; i++)
    {
        for (size_t j = i + 1; j < tamanhoMatriz; j++)
        {
            eliminarElemento(matriz, identidade, i, j, i, tamanhoMatriz);
        }
    }
}

std::vector<std::vector<double>> calcula_inversa(
    std::vector<std::vector<double>> matriz, std::vector<std::vector<double>> identidade
){
    std::vector<std::vector<double>> inversa(matriz.size(), std::vector<double>(matriz.size(), 0.0));
    int tamanhoMatriz = matriz.size();
    zerarElementosAcimaDiagonal(matriz, identidade, tamanhoMatriz);
    zerarElementosAbaixoDiagonal(matriz, identidade, tamanhoMatriz);

    for (size_t i = 0; i < matriz.size(); i++){
        for (size_t j = 0; j < matriz[i].size(); j++){
            std::cout << matriz[i][j] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "Identidade" << std::endl;
    for (size_t i = 0; i < identidade.size(); i++){
        for (size_t j = 0; j < identidade[i].size(); j++){
            std::cout << identidade[i][j] << " ";
        }
        std::cout << "\n";
    }    
    if (matriz == identidade){
        return matriz;
    }else{
        for (size_t i = 0; i < matriz.size(); i++)
        {
            double pivo = matriz[i][i];
            for (size_t j = 0; j < matriz.size(); j++)
            {   
                if (pivo != 0) {
                    inversa[i][j] = identidade[i][j] / pivo;
                }else{
                    std::cout << "Erro:: Divisão por zero" << std::endl;
                    break;
                }
            }
        }
    }
    return inversa;
}
#endif