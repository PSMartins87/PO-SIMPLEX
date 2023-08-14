#ifndef SIMPLEX_H
#define SIMPLEX_H

#include <string>
#include <vector>
#include <regex>

#include "tabela.h"
//#include "opera_matrizes.h"
//#include "obtem_inversa.h"
#include "exibicao.h"

// Assinaturas de funções
void formalizar_tabela(Tabela& tabela);
void artificializar(Tabela& tabela);

bool checa_solucao_otima(std::vector<double> C_chapeu){
    for (int elemento : C_chapeu) {
        if (elemento < 0) {
            return false;
        }
    }
    return true;
}

void formalizar_tabela(Tabela& tabela){

    std::regex padraoVariaveisNBasicas("x+\\d*");
    int numVariaveisNBasicas = 0;
    for (size_t i = 0; i < tabela.X.size(); i++){
        if(std::regex_match(tabela.X[i], padraoVariaveisNBasicas)){
            numVariaveisNBasicas++;
        }
    }
    tabela.num_var = numVariaveisNBasicas;

    int linha = 0;
    int indice_limite = 0;

    for(std::string &limite : tabela.limites){
        tabela.linha_artificial.push_back(true);
        if ( (limite == "<" || limite == ">") || (limite == "<=" || limite == ">=")){

            tabela.num_var++;
            std::string xB = "b" + std::to_string(tabela.num_var);
            tabela.X.push_back(xB);
            tabela.C.push_back(0.0);

            tabela.linha_artificial[linha] = false;
            if( !tabela.matrizA.empty()){
                for (size_t i = 0; i < tabela.coluna_b.size(); i++){
                    if( i == linha ){
                        tabela.matrizA[i].push_back(1.0);
                    }else{
                        tabela.matrizA[i].push_back(0.0);
                    }
                }
            }else{
                for (size_t i = 0; i < tabela.coluna_b.size(); i++){

                    std::vector<double> nova_linha;

                    if( i == linha ){
                        nova_linha.push_back(1.0);
                        tabela.matrizA.push_back(nova_linha);
                    }else{
                        nova_linha.push_back(0.0);
                        tabela.matrizA.push_back(nova_linha);
                    }
                    nova_linha.clear();
                }
            }

            tabela.limites[indice_limite] = "=";
        }else{
            tabela.problema_artificial = true;
        }
        indice_limite++;
        linha++;
    }
}

void artificializar(Tabela& tabela){
    
    // Artificializa o problema
    for (size_t i = 0; i < tabela.C.size(); i++){
        tabela.C[i] = 0.0;
    }
    int linha = 0;
    for(std::string &limite : tabela.limites){

        if (limite == "=" && tabela.linha_artificial[linha]){

            tabela.num_var++;
            std::string xA = "a" + std::to_string(tabela.num_var);
            tabela.X.push_back(xA);
            tabela.C.push_back(1.0);

            if( !tabela.matrizA.empty()){
                for (size_t i = 0; i < tabela.coluna_b.size(); i++){
                    if( i == linha ){
                        tabela.matrizA[i].push_back(1.0);
                    }else{
                        tabela.matrizA[i].push_back(0.0);
                    }
                }
            }else{
                for (size_t i = 0; i < tabela.coluna_b.size(); i++){

                    std::vector<double> nova_linha;

                    if( i == linha ){
                        nova_linha.push_back(1.0);
                        tabela.matrizA.push_back(nova_linha);
                    }else{
                        nova_linha.push_back(0.0);
                        tabela.matrizA.push_back(nova_linha);
                    }
                    nova_linha.clear();
                }
            }
        }
        linha++;
    }
}

/*
void resolve_problema_artificial(Tabela& tabela){

    // Trecho responsável por obter todas as informações relevantes para
    // a solução do problema
    std::regex padraoVariaveisNBasicas("x+\\d*");
    int numVariaveisNBasicas = 0;
    for (size_t i = 0; i < tabela.X.size(); i++){
        if(std::regex_match(tabela.X[i], padraoVariaveisNBasicas)){
            numVariaveisNBasicas++;
        }
    }
    // Conjunto de variaveis X = [XN, XB, XA]
    std::vector<std::string> XN;
    std::vector<std::string> XB;
    XN = getXN(tabela.X, numVariaveisNBasicas);
    XB = getXB(tabela.X, numVariaveisNBasicas);
    // Conjunto de matrizes A = [N|B] do escopo da função
    std::vector<std::vector<double>> matrizN;
    std::vector<std::vector<double>> matrizB;
    matrizN = getMatrizN(tabela.matrizA, numVariaveisNBasicas);
    matrizB = getMatrizB(tabela.matrizA, numVariaveisNBasicas);
    // Conjunto de Coeficientes C = [Ctn|Ctb] do escopo da função
    std::vector<double> Ctn;
    std::vector<double> Ctb;
    Ctn = getCtn(tabela.C, numVariaveisNBasicas);
    Ctb = getCtb(tabela.C, numVariaveisNBasicas);

    // Resolve o problema artificial
    bool soluca_otima = false;
    while(tabela.problema_artificial){

        std::vector<std::vector<double>> BInversa;
        std::vector<std::vector<double>> colunasN;
        std::vector<double> XB_chapeu;
        std::vector<double> CTB;
        std::vector<double> C_chapeu;
        std::vector<double> Lambda_t;

        BInversa = calcula_inversa(
            matrizB, tabela.matriz_I
        );  
        
        XB_chapeu = multiplicaMatrizes(
            BInversa, tabela.coluna_b
        );

        CTB.assign(tabela.C.begin() + Ctn.size(), tabela.C.end());
        Lambda_t = multiplicaMatrizes(
            CTB, BInversa
        );
        for (size_t i = 0; i < Ctn.size(); i++){
            colunasN.push_back(obtem_coluna(matrizN, i));
            C_chapeu.push_back(tabela.C[i] - multiplica_vetores(colunasN[i], Lambda_t));
        }
        std::cout << "\n";
        std::cout << "ColunasN" << std::endl;
        for (size_t i = 0; i < colunasN.size(); i++){
            for (size_t j = 0; j < colunasN[i].size(); j++){
                std::cout << colunasN[i][j] << " "; 
            }
            std::cout << "\n";
        }  
        
        exibeVetor_double(C_chapeu, "C_chapeu");

        soluca_otima = checa_solucao_otima(C_chapeu);
        if (soluca_otima){
            std::cout << "Solucao Basica Viavel (SBV) para o problema artificial encontrada! \n";
            tabela.problema_artificial = false;
        }else{
            int var_entrada = 0;
            for (size_t i = 0; i < C_chapeu.size(); i++){
                if (C_chapeu[i] < C_chapeu[var_entrada]){
                    var_entrada = i;
                }
            }

            std::vector<double> Y = multiplicaMatrizes(BInversa, colunasN[var_entrada]);
            int var_saida = 0;
            double E = 9999;
            for (size_t i = 0; i < XB_chapeu.size(); i++){
                if (XB_chapeu[i]/Y[i] >= 0 && XB_chapeu[i]/Y[i] < E){
                    var_saida = i;
                }   
            }

            std::cout << "Sai: " << tabela.X[var_saida + XN.size()] << std::endl;
            std::cout << "Entra: " << tabela.X[var_entrada] << std::endl;

            troca_coluna(tabela, tabela.matrizA, var_saida + XN.size(), var_entrada);
            troca_coluna(tabela, tabela.X, var_saida + XN.size(), var_entrada);
            troca_coluna(tabela, tabela.C, var_saida + XN.size(), var_entrada);
            matrizB = getMatrizB(tabela.matrizA, numVariaveisNBasicas);
            matrizN = getMatrizN(tabela.matrizA, numVariaveisNBasicas);


            
            std::cout << "Step" << std::endl;
            int command;
            std::cin >> command;
            
        }
    }
}
*/
#endif