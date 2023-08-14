#ifndef TABELA_H
#define TABELA_H

#include <vector>
#include <iostream>
#include <string>

void gera_identidade(Tabela& tabela);

struct Tabela
{
    bool passo_a_passo = true;
    bool problema = false;              // Verifica se o problema já foi resolvido
    bool problema_artificial = false;   // Verifica se o problema artificial já foi resolvido
    std::vector<bool> linha_artificial; // Verifica se as linhas precisarão de variaveis artificiais

    std::vector<std::string> restricoes;
    std::vector<std::string> limites;
    int iteracao;
    int num_var;
    bool tipo_funcao;                   // Verifica se é Minimizar (true) ou Maximizar (false)

    std::vector<std::string> X;                 // Coluna [XN|XB]
    std::vector<double> C;                      // Coeficiente [CtN|CtB]
    std::vector<double> coluna_b;               // Coluna b
    std::vector<std::vector<double>> matrizA;   // Matriz A [B|N]
    std::vector<std::vector<double>> matriz_I;  // Matriz Identidade
};

void gera_identidade(Tabela& tabela){
    int tamanho = tabela.matrizA.size();
    std::vector<double> linha;
    for (int i = 0; i < tamanho; i++){
        for (int j = 0; j < tamanho; j++){
            if (i == j){
                linha.push_back(1.0);
            }else{
                linha.push_back(0.0);
            }
        }
        tabela.matriz_I.push_back(linha);
        linha.clear();
    }
}

#endif