#ifndef LPREADER_H
#define LPREADER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <regex>

double match_coeficiente( std::string token_sinal = "+", std::string token_coef = "1"){

    // Trecho para garantir que caso nada seja recebido, o sinal e coeficiente serão tratados como 1
    if (token_sinal.empty()){
        token_sinal = "+";
    }
    if (token_coef.empty()){
        token_coef = "1";
    }

    if (token_sinal == "-"){
        if (token_coef[0] == '-'){
            token_coef = token_coef.substr(1);
        }else{
            token_coef = "-" + token_coef;
        }
    }

    return std::stod(token_coef);
}

std::vector<std::string> getTokens(const std::string &linha) {
    std::vector<std::string> tokens;
    size_t start = 0;
    size_t end = 0;

    while (end != std::string::npos) {
        end = linha.find(' ', start);
        std::string token = linha.substr(start, end - start);
        tokens.push_back(token);
        start = end + 1;
    }

    return tokens;
}

Tabela lerArquivo(const std::string &nomeArquivo) {

    Tabela tabela;
    std::ifstream arquivo(nomeArquivo);

    // Tratamento de falha na leitura
    if (!arquivo.is_open()) {
        std::cout << "Não foi possível abrir o arquivo." << std::endl;
        return tabela;
    }

    // Variáveis usadas para tratar casos de variáveis sem coeficiente e inversão de valor (- -) 
    std::string sinal;
    std::string coeficiente;
    // Variável para identificar restrições
    bool inBounds = false;
    // ...Para trabalhar com as linhas da entrada
    std::string linha;
    // Padrões utilizados pelo algoritmo
    std::regex padraoOperadores("[+-]");
    std::regex padraoCoeficiente("[+-]?\\d+(\\.\\d+)?");
    std::regex padraoVariavel("[A-Za-z]+\\d?");
    std::regex padraoEquacoes("<|>|=|<=|>=");
    std::regex padraoRestricao("c\\d+");

    while (std::getline(arquivo, linha)) {
        if(linha.find("Maximize") != std::string::npos){
            tabela.tipo_funcao = false;
        }
        if(linha.find("Minimize") != std::string::npos){
            tabela.tipo_funcao = true;
        }
        if(linha.find("obj:") != std::string::npos){

            // Separa a linha obtida em um conjunto de tokens
            std::vector<std::string> tokens = getTokens(linha);

            // Categoriza os tokens e realiza as respectivas ações
            // obj: (ignora)
            // sinal de operação ()
            // número (adiciona aos coeficientes)
            // variável (adiciona as variáveis
            for (const std::string &token : tokens) {
                if(token == "obj:"){
                }else if(std::regex_match(token, padraoOperadores)){
                    sinal = token;
                }else if(std::regex_match(token, padraoCoeficiente)){
                    coeficiente = token;
                }else if(std::regex_match(token, padraoVariavel)){
                    tabela.X.push_back(token);
                    tabela.C.push_back(
                        match_coeficiente(sinal, coeficiente)
                    );
                    sinal.clear();
                    coeficiente.clear();
                }

            }
        }   // Fim da função objetivo
        if(linha.find("Subject To:") != std::string::npos){
        }
        if(std::regex_search(linha, padraoRestricao)){

            std::vector<std::string> tokens = getTokens(linha);
            std::vector<double> linhaRestricao;
            bool limiteEncontrado = false;  // Utilizado para separar N e b

            for (const std::string &token : tokens) {
                if(std::regex_match(token, padraoOperadores)){
                    sinal = token;
                } else if(std::regex_match(token, padraoCoeficiente)){
                    if(limiteEncontrado == false){
                        coeficiente = token;
                    }else{
                        tabela.coluna_b.push_back(std::stod(token));// Adiciona coeficiente coluna b
                    }
                }else if(std::regex_match(token, padraoVariavel)){
                    // Adiciona a coeficiente ao vetor após tratar possíveis problemas,
                    // garante que toda variável tenha um coeficiente
                    linhaRestricao.push_back(
                        match_coeficiente(sinal, coeficiente)
                    );
                    sinal.clear();
                    coeficiente.clear();
                }else if(std::regex_match(token, padraoEquacoes)){
                    tabela.limites.push_back(token);    // Insere o tipo de limite da restrição
                    limiteEncontrado = true;
                }
            }

            // Insere coeficientes na matriz N 
            // e garante que a nova linha estara vazia para proxima restrição
            if(!linhaRestricao.empty()){
                tabela.matrizA.push_back(linhaRestricao);
                linhaRestricao.clear();
            }
        }else if (linha.find("Bounds") != std::string::npos){
            inBounds = true;
        }else if (inBounds && linha.find("End") != std::string::npos){
            inBounds = false;
        }else if (inBounds){
            std::smatch match;
            std::regex boundPattern(R"((-\inf|-\d+|inf|\d+)?\s*(<=|>=|<|>|=)?\s*x(\d+)\s*(<=|>=|<|>|=)?\s*(-\inf|-\d+|inf|\d+)?)"); // Padrão de expressão regular para detectar limites de variáveis.
            if (std::regex_search(linha, match, boundPattern))                                                                       // Verifica se a linha corresponde ao padrão de limite de variável.
            {   
                tabela.restricoes.push_back(linha);
                /*
                VariableBound bound;                                                                                                                                                                         // Cria um novo limite de variável.
                bound.lower_bound_sign = (match[2].str() == "") ? "<" : match[2].str();                                                                                                                      // Define o sinal do limite inferior.
                bound.lower_bound = (match[1].str() == "-inf") ? -std::numeric_limits<double>::infinity() : ((match[1].str() == "") ? -std::numeric_limits<double>::infinity() : std::stod(match[1].str())); // Define o limite inferior.
                bound.upper_bound_sign = (match[4].str() == "") ? "<" : match[4].str();                                                                                                                      // Define o sinal do limite superior.
                bound.upper_bound = (match[5].str() == "inf") ? std::numeric_limits<double>::infinity() : ((match[5].str() == "") ? std::numeric_limits<double>::infinity() : std::stod(match[5].str()));    // Define o limite superior.
                instance.bounds.push_back(bound);      
                */                                                                                                                                                      // Adiciona o limite à instância.
            }
        }
    }
    
    return tabela;
}

#endif