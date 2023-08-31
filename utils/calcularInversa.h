#include <iostream>
#include <vector>

#include "./InstanceReader.h"

void check(){
    std::cout << "CHECK" << std::endl;
}
// Functions Signatures (Assinaturas de Funções)
LPInstance relocateReverseB(LPInstance instance);
bool checkSpecialCase(std::vector<std::vector<double>> matriz);
LPInstance calculaInversa (LPInstance instance);

LPInstance relocateReverseB( LPInstance instance){
    for (size_t i = 0; i < instance.reverseB.size(); i++){
        for (size_t j = 0; j < instance.reverseB.size(); j++){
            if (instance.reverseB[i][j] == 1 && i != j){
                // Para cada linha de restrição, passa fazendo a troca (no escopo de [B])
                for (size_t a_i = 0; a_i < instance.constraints.size(); a_i++){
                    std::swap(
                        instance.constraints[i].coefficients[i + instance.var_n],
                        instance.constraints[i].coefficients[j + instance.var_n]
                    );   
                }
                std::swap(instance.reverseB[i][j], instance.reverseB[i][i]);
                std::swap(instance.objective[i], instance.objective[j]);
                std::swap(instance.c_aux[i], instance.c_aux[j]);
            }
        }
    }
    return instance;
}

bool checkSpecialCase(std::vector<std::vector<double>> matriz){
    bool special_case = false;
    int num_ones = 0,  num_zeros = 0;

    for (size_t i = 0; i < matriz.size(); i++){
        for (size_t j = 0; j < matriz[i].size(); j++){
            if (matriz[i][j] == 1){
                num_ones++;
            }else if (matriz[i][j] == 0){
                num_zeros++;
            }
        }
        if ( (num_zeros == (matriz[i].size() - 1)) || (num_ones == 1)){
            special_case = true;
        }else{
            special_case = false;
            return special_case;
        }
        num_ones = 0;
        num_zeros = 0;
    }
    std::cout << "Calculo em caso especial (Matriz Identidade ou Identidade deslocada)" << std::endl;
    return special_case;
}

LPInstance calculaInversa (LPInstance instance){

    int tamanho = instance.reverseB.size();

    if (checkSpecialCase(instance.reverseB)){
        instance = relocateReverseB(instance);
    }else{
        // Cria matriz aumentada [M|I]
        std::vector<std::vector<double>> matriz_aumentada(tamanho, std::vector<double>(2 * tamanho));
        check();
        for (int i = 0; i < tamanho; ++i) {
            for (int j = 0; j < tamanho; ++j) {
                matriz_aumentada[i][j] = instance.reverseB[i][j];
                matriz_aumentada[i][j + tamanho] = (i == j) ? 1.0 : 0.0;
            }
        }
        check();
        std::cout << "Matriz Aumentada criada" << std::endl;
        // Aplicar eliminação de Gauss-Jordan para transformar a matriz à esquerda em uma matriz identidade
        for (int i = 0; i < tamanho; ++i) {
            double pivo = matriz_aumentada[i][i];
            for (int j = 0; j < 2 * tamanho; ++j) {
                matriz_aumentada[i][j] /= pivo;
            }
            for (int k = 0; k < tamanho; ++k) {
                if (k != i) {
                    double fator = matriz_aumentada[k][i];
                    for (int j = 0; j < 2 * tamanho; ++j) {
                        matriz_aumentada[k][j] -= fator * matriz_aumentada[i][j];
                    }
                }
            }
        }
        // Extrair a matriz inversa da matriz aumentada
        for (int i = 0; i < tamanho; ++i) {
            for (int j = 0; j < tamanho; ++j) {
                instance.reverseB[i][j] = matriz_aumentada[i][j + tamanho];
            }
        }
    }
    return instance;
}