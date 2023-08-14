#include "utils/tabela.h"
#include "utils/lpreader.h"
#include "utils/simplex.h"
#include "utils/exibicao.h"

#include <iostream>
#include <string>

int main(){
    // Leitura do arquivo .lp
    std::string nomeArquivo = "C:/src/VSCode_Workspace/C/NewVersion_TrabSimplex/lista_problemas/exerc57.lp";
    // Cria a instância da tabela
    Tabela tabela = lerArquivo(nomeArquivo);
    gera_identidade(tabela);

    // Tabela Original do problema
    if(tabela.passo_a_passo){
        exibeTabela(tabela);
    }

    // Transforma a função para a forma padrão
    formalizar_tabela(tabela);
    if(tabela.passo_a_passo){
        std::cout << "Formaliza Tabela" << std::endl;
        exibeTabela(tabela);
    }

    // Se necessário, transforma a tabela em um problema artificial
    // Salva uma cópia da coluna C caso haja problema artificial que altera C
    std::vector<double> C_copy = tabela.C;
    if (tabela.problema_artificial){
        artificializar(tabela);
        if(tabela.passo_a_passo){
            std::cout << "Necessita Problema Artificial" << std::endl;
            exibeTabela(tabela);
        }
    }
    // Transforma a matriz B em matriz identidade para facilitar a manipulação
    //checa_identidade_B(tabela);

    //resolve_problema_artificial(tabela);

    while(tabela.problema){
        std::cout << std::endl;
    }

    return 0;
}