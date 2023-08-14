#ifndef EXIBICAO_H
#define EXIBICAO_H

#include <iostream>
#include <vector>
#include <string>

void exibeTabela(Tabela tabela);
void exibeMatriz_double(std::vector<std::vector<double>> matriz, std::string nome);
void exibeVetor_double(std::vector<double> vetor, std::string nome);
void exibeVetor_string(std::vector<std::string> vetor, std::string nome);

void exibeTabela(Tabela tabela){
    exibeVetor_string(tabela.X, "X");
    exibeMatriz_double(tabela.matrizA, "A");
    exibeVetor_double(tabela.C, "C");
}

void exibeMatriz_double(std::vector<std::vector<double>> matriz, std::string nome){
    std::cout << "Matriz " << nome << std::endl;
    for (size_t i = 0; i < matriz.size(); i++){
        for (size_t j = 0; j < matriz[i].size(); j++){
            std::cout << matriz[i][j] << " ";
        }
        std::cout << "\n";
    }
}
void exibeVetor_double(std::vector<double> vetor, std::string nome){
    std::cout << "Vetor " << nome << std::endl;
    for (size_t i = 0; i < vetor.size(); i++){
        std::cout << vetor[i] << " ";
    }
    std::cout << std::endl;
}
void exibeVetor_string(std::vector<std::string> vetor, std::string nome){
    std::cout << "Vetor " << nome << std::endl;
    for (size_t i = 0; i < vetor.size(); i++){
        std::cout << vetor[i] << " ";
    }
    std::cout << std::endl;
}

#endif