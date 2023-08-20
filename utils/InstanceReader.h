#ifndef INSTANCEREADER_H
#define INSTANCEREADER_H
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <regex>
#include <limits>

/**
 * @struct VariableBound
 * @brief Estrutura que representa os limites de uma variável.
 */
struct VariableBound
{
    std::string lower_bound_sign; // Sinal do limite inferior ("<", ">", "<=" ou ">=").
    double lower_bound;           // Limite inferior da variável.
    std::string upper_bound_sign; // Sinal do limite superior ("<", ">", "<=" ou ">=").
    double upper_bound;           // Limite superior da variável.
};

/**
 * @struct Constraint
 * @brief Estrutura que representa uma restrição.
 */
struct Constraint
{
    std::string signal;               // Sinal da restrição ("<", ">", "<=", ">=" ou "=").
    std::vector<double> coefficients; // Coeficientes das variáveis na restrição.
    double bound;                     // Limite da restrição.
};

/**
 * @struct LPInstance
 * @brief Estrutura que representa uma instância.
 */
struct LPInstance
{
    bool type;                           // Tipo do problema (true para maximização, false para minimização).
    bool is_artificial;
    int var_n;
    std::vector<double> objective;       // Coeficientes da função objetivo.
    std::vector<Constraint> constraints; // Vetor de restrições do problema.
    std::vector<VariableBound> bounds;   // Vetor de limites das variáveis do problema.
};

/**
 * @brief Extrai os coeficientes das variáveis de uma expressão de restrição.
 *
 * Esta função recebe uma expressão de restrição no formato "ax1 + bx2 - cx3 + ...",
 * onde 'a', 'b', 'c', ... são coeficientes e 'x1', 'x2', 'x3', ... são variáveis.
 * Os coeficientes são extraídos e retornados em um vetor.
 *
 * @param expression A expressão de restrição a ser processada.
 * @param numVariables O número total de variáveis possíveis (opcional).
 * @return Um vetor contendo os coeficientes das variáveis extraídas da expressão.
 */
std::vector<double> extractCoefficients(const std::string &expression, int numVariables = 0)
{
    std::string expressionWithoutSpaces = std::regex_replace(expression, std::regex("\\s+"), "");     // Remove espaços em branco.
    std::vector<double> coefficients;                                                                 // Vetor para armazenar os coeficientes.
    std::regex pattern(R"(([+-]?\d*(\.\d+)?)[xX](\d+))");                                             // Padrão de expressão regular para detectar coeficientes de variáveis.
    std::sregex_iterator it(expressionWithoutSpaces.begin(), expressionWithoutSpaces.end(), pattern); // Iterador para percorrer as correspondências na expressão.
    std::sregex_iterator end;                                                                         // Fim do iterador.
    coefficients = std::vector<double>(numVariables, 0.0);

    while (it != end) // Processa cada correspondência encontrada na expressão.
    {
        double coefficient = 0.0;
        if ((*it)[1].str() == "+" || (*it)[1].str().empty())
        {
            coefficient = 1.0; // Atribui valor a coeficientes ocultos, por exemplo x1 se torna 1 x1.
        }
        else if ((*it)[1].str() == "-")
        {
            coefficient = -1.0; // Atribui valor a coeficientes ocultos, por exemplo -x1 se torna -1 x1.
        }
        else
        {
            coefficient = std::stod((*it)[1].str()); // O coeficiente é extraído.
        }
        size_t index = std::stoi((*it)[3].str()); // Obtém o índice da variável.
        if (index > coefficients.size())       // Redimensiona o vetor de coeficientes se necessário.
        {
            coefficients.resize(index, 0);
        }
        coefficients[index - 1] = coefficient;
        ++it; // Avança para a próxima correspondência.
    }
    return coefficients; // Retorna o vetor de coeficientes das variáveis.
}

/**
 * @brief Carrega uma instância a partir da entrada padrão.
 *
 * Esta função lê dados da entrada padrão e carrega uma instância.
 * Ela processa as informações sobre o tipo do problema, a função objetivo, as restrições e os limites das variáveis.
 *
 * @return Uma instância do tipo LPInstance.
 */
LPInstance loadFile()
{
    LPInstance instance; // Cria uma instância vazia.
    std::string line;    // Armazena a linha atual lida da entrada.
    bool inBounds = false;
    int numVariables = 0;
    while (std::getline(std::cin, line)) // Loop para ler as linhas da entrada padrão.
    {
        line = std::regex_replace(line, std::regex("\\s+"), ""); // Remove espaços em branco.
        try
        {
            if (line.find("Maximize") != std::string::npos)
            {
                instance.type = true; // Define o tipo do problema como maximização.
            }
            if (line.find("Minimize") != std::string::npos)
            {
                instance.type = false; // Define o tipo do problema como minimização.
            }
            if (line.find("obj:") != std::string::npos) // Verifica se a linha contém "obj:".
            {
                instance.objective = extractCoefficients(line); // Extrai coeficientes da função objetivo.
                numVariables = instance.objective.size();       // Define o número de variáveis.
            }
            if (line.find("SubjectTo") != std::string::npos) // Verifica se a linha contém "SubjectTo".
            {
                while (std::getline(std::cin, line) && line.find("Bounds") == std::string::npos) // Lê as restrições até encontrar a seção "Bounds".
                {
                    Constraint c;                                   // Cria uma nova restrição.
                    std::regex signPattern("([=<>]+)\\s?(-?\\d+)"); // Padrão para sinal e valor da restrição.
                    std::smatch signMatch;                          // Armazena as correspondências.
                    if (std::regex_search(line, signMatch, signPattern))
                    {
                        c.signal = signMatch[1].str();                            // Define o sinal da restrição.
                        c.coefficients = extractCoefficients(line, numVariables); // Extrai coeficientes.
                        c.bound = std::stod(signMatch[2].str());                  // Define o limite da restrição.
                        instance.constraints.push_back(c);                        // Adiciona à lista de restrições.
                    }
                }
            }
            if (line.find("Bounds") != std::string::npos)
            {
                inBounds = true;
            }
            else if (inBounds && line.find("End") != std::string::npos)
            {
                inBounds = false;
            }
            else if (inBounds)
            {
                std::smatch match;
                std::regex boundPattern(R"((-\inf|-\d+|inf|\d+)?\s*(<=|>=|<|>|=)?\s*x(\d+)\s*(<=|>=|<|>|=)?\s*(-\inf|-\d+|inf|\d+)?)"); // Padrão de expressão regular para detectar limites de variáveis.
                if (std::regex_search(line, match, boundPattern))                                                                       // Verifica se a linha corresponde ao padrão de limite de variável.
                {
                    VariableBound bound;                                                                                                                                                                         // Cria um novo limite de variável.
                    bound.lower_bound_sign = (match[2].str() == "") ? "<" : match[2].str();                                                                                                                      // Define o sinal do limite inferior.
                    bound.lower_bound = (match[1].str() == "-inf") ? -std::numeric_limits<double>::infinity() : ((match[1].str() == "") ? -std::numeric_limits<double>::infinity() : std::stod(match[1].str())); // Define o limite inferior.
                    bound.upper_bound_sign = (match[4].str() == "") ? "<" : match[4].str();                                                                                                                      // Define o sinal do limite superior.
                    bound.upper_bound = (match[5].str() == "inf") ? std::numeric_limits<double>::infinity() : ((match[5].str() == "") ? std::numeric_limits<double>::infinity() : std::stod(match[5].str()));    // Define o limite superior.
                    instance.bounds.push_back(bound);                                                                                                                                                            // Adiciona o limite à instância.
                }
            }
        }
        catch (const std::exception &e) // Tratamento de exceção em caso de erro.
        {
            std::cerr << "Excecao capturada na linha: " << line << std::endl;
            std::cerr << "Mensagem de erro: " << e.what() << std::endl;
        }
    }
    return instance; // Retorna a instância de problema de programação linear carregada.
}
#endif // INSTANCEREADER_H