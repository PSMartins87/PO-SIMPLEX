#include <iostream>
#include <vector>

void check(){
    std::cout << "CHECK" << std::endl;
}

void mostrarMatrizA(LPInstance instance){
    std::cout << "A = " << std::endl;
    for (size_t i = 0; i < instance.constraints.size(); i++){
        for (size_t j = 0; j < instance.constraints[i].coefficients.size(); j++){
            std::cout << instance.constraints[i].coefficients[j] << " ";
        }
        std::cout << std::endl;
    }
}
void mostrarMatrizB(LPInstance instance){
    std::cout << "B = " << std::endl;
    for (size_t i = 0; i < instance.reverseB.size(); i++){
        for (size_t j = 0; j < instance.reverseB[i].size(); j++){
            std::cout << instance.reverseB[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

void mostrarMatrizInversaB(LPInstance instance){
    std::cout << "reverseB = " << std::endl;
    for (size_t i = 0; i < instance.reverseB.size(); i++){
        for (size_t j = 0; j < instance.reverseB[i].size(); j++){
            std::cout << instance.reverseB[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

void mostrarVetorObjetivo(LPInstance instance){
    std::cout << "C = [";
    for (size_t i = 0; i < instance.objective.size(); i++){
        std::cout << instance.objective[i] << ", ";
    }
    std::cout << "]" << std::endl;
}

void mostrarVetorLimite(LPInstance instance){
    std::cout << "b = [";
    for (size_t i = 0; i < instance.constraints.size(); i++){
        std::cout << instance.constraints[i].bound << ", ";
    }
    std::cout << "]" << std::endl;
}

/**
 * @brief Preenche uma matriz com a matriz identidade.
 *
 * @param matriz A matriz a ser preenchida.
 * @param tamanhoMatriz O tamanho da matriz (número de linhas e colunas).
 */
void preencheIdentidade(std::vector<std::vector<double>> &matriz, int tamanhoMatriz)
{
    matriz.assign(tamanhoMatriz, std::vector<double>(tamanhoMatriz, 0.0));
    for (int i = 0; i < tamanhoMatriz; i++)
    {
        matriz[i][i] = 1;
    }
}

double multiplicarVetorVetor(
    std::vector<double> vetor1, std::vector<double> vetor2
){
    std::vector<double> vetor_resultante;
    double soma = 0;

    for (size_t i = 0; i < vetor1.size(); i++){
        soma += (vetor1[i] * vetor2[i]);      
    }
    return soma;
}

std::vector<double> multiplicaMatrizVetor(
    std::vector<std::vector<double>> matriz, std::vector<double> vetor
){
    std::vector<double> vetor_resultante;
    double soma = 0;

    for (size_t i = 0; i < matriz.size(); i++){
        for (size_t j = 0; j < matriz[i].size(); j++){
            soma += (matriz[i][j] * vetor[j]);      
        }
        vetor_resultante.push_back(soma);
        soma = 0;
    }

    return vetor_resultante;
}

LPInstance eliminaColuna(LPInstance instance, int coluna){
    // Elimina elemento em C
    if (coluna < instance.objective.size()){
        instance.objective.erase(instance.objective.begin() + coluna);
    }


    for (size_t i = 0; i < instance.constraints.size(); i++) {
        if (coluna < instance.constraints[i].coefficients.size()) {
            instance.constraints[i].coefficients.erase(instance.constraints[i].coefficients.begin() + coluna);
        }
    }

    return instance;
}
/**
 * @brief Multiplica duas matrizes.
 *
 * @param matriz1 A primeira matriz.
 * @param matriz2 A segunda matriz.
 * @return O resultado da multiplicação das matrizes.
 */
std::vector<std::vector<double>> multiplicarMatrizes(const std::vector<std::vector<double>> &matriz1, const std::vector<std::vector<double>> &matriz2)
{
    int linhasMatriz1 = matriz1.size();
    int colunasMatriz1 = matriz1[0].size();
    int linhasMatriz2 = matriz2.size();
    int colunasMatriz2 = matriz2[0].size();

    if (colunasMatriz1 != linhasMatriz2)
    {
        std::cout << "O número de colunas da matriz1 deve ser igual ao número de linhas da matriz2." << std::endl;
        return std::vector<std::vector<double>>();
    }
    std::vector<std::vector<double>> resultado(linhasMatriz1, std::vector<double>(colunasMatriz2, 0.0));

    for (int i = 0; i < linhasMatriz1; i++)
    {
        for (int j = 0; j < colunasMatriz2; j++)
        {
            for (int k = 0; k < colunasMatriz1; k++)
            {
                resultado[i][j] += matriz1[i][k] * matriz2[k][j];
            }
        }
    }

    return resultado;
}

/**
 * @brief Mostra os elementos de uma matriz.
 *
 * @param matriz A matriz a ser exibida.
 */
void mostrarMatriz(const std::vector<std::vector<double>> &matriz)
{
    for (const auto &linha : matriz)
    {
        for (double elemento : linha)
        {
            std::cout << elemento << "    ";
        }
        std::cout << std::endl;
    }
}

/**
 * @brief Troca duas linhas em uma matriz.
 *
 * @param matriz A matriz na qual as linhas serão trocadas.
 * @param identidade A matriz identidade correspondente.
 * @param linha1 O índice da primeira linha.
 * @param linha2 O índice da segunda linha.
 * @param tamanhoMatriz O tamanho da matriz (número de linhas e colunas).
 */
void trocarLinhas(std::vector<std::vector<double>> &matriz, std::vector<std::vector<double>> &identidade, int linha1, int linha2, int tamanhoMatriz)
{
    for (int k = 0; k < tamanhoMatriz; k++)
    {
        std::swap(matriz[linha1][k], matriz[linha2][k]);
        std::swap(identidade[linha1][k], identidade[linha2][k]);
    }
}

/**
 * @brief Elimina um elemento de uma matriz para fazer a diagonal se tornar 1 e abaixo dela zeros.
 *
 * @param matriz A matriz na qual a eliminação será realizada.
 * @param identidade A matriz identidade correspondente.
 * @param linha1 A linha alvo (para se tornar 1).
 * @param linha2 A linha na qual a eliminação será feita (para se tornar zero).
 * @param coluna A coluna alvo.
 * @param tamanhoMatriz O tamanho da matriz (número de linhas e colunas).
 */
void eliminarElemento(std::vector<std::vector<double>> &matriz, std::vector<std::vector<double>> &identidade, int linha1, int linha2, int coluna, int tamanhoMatriz)
{
    double fator = matriz[linha2][coluna] / matriz[linha1][coluna];
    for (int k = 0; k < tamanhoMatriz; k++)
    {
        matriz[linha2][k] -= fator * matriz[linha1][k];
        identidade[linha2][k] -= fator * identidade[linha1][k];
    }
}

/**
 * @brief Zera os elementos acima da diagonal principal de uma matriz.
 *
 * @param matriz A matriz a ser processada.
 * @param identidade A matriz identidade correspondente.
 * @param tamanhoMatriz O tamanho da matriz (número de linhas e colunas).
 */
void zerarElementosAcimaDiagonal(std::vector<std::vector<double>> &matriz, std::vector<std::vector<double>> &identidade, int tamanhoMatriz)
{
    for (int i = 0; i < tamanhoMatriz; i++)
    {
        for (int j = i + 1; j < tamanhoMatriz; j++)
        {
            eliminarElemento(matriz, identidade, i, j, i, tamanhoMatriz);
        }
    }
}

/**
 * @brief Zera os elementos abaixo da diagonal principal de uma matriz.
 *
 * @param matriz A matriz a ser processada.
 * @param identidade A matriz identidade correspondente.
 * @param tamanhoMatriz O tamanho da matriz (número de linhas e colunas).
 */
void zerarElementosAbaixoDiagonal(std::vector<std::vector<double>> &matriz, std::vector<std::vector<double>> &identidade, int tamanhoMatriz)
{
    for (int i = tamanhoMatriz - 1; i >= 0; i--)
    {
        for (int j = i - 1; j >= 0; j--)
        {
            eliminarElemento(matriz, identidade, i, j, i, tamanhoMatriz);
        }
    }
}

bool checkCasoEspecial(
    std::vector<std::vector<double>> inversa
){
    bool special_case = false;
    int num_ones = 0,  num_zeros = 0;

    for (size_t i = 0; i < inversa.size(); i++){
        for (size_t j = 0; j < inversa[i].size(); j++){
            if (inversa[i][j] == 1){
                num_ones++;
            }else if (inversa[i][j] == 0){
                num_zeros++;
            }
        }
        //std::cout << "N0: " << num_zeros << " N1: " << num_ones << std::endl;
        // Verifica se a matriz possui propriedades de identidade
        // (cada linha com um '1' e o restante de 0)
        if ( (num_zeros == (inversa[i].size() - 1)) || (num_ones == 1)){
            special_case = true;
        }else{
            special_case = false;
            return special_case;
        }
        num_ones = 0;
        num_zeros = 0;
    }
    return special_case;
}

LPInstance trocaColunas(
    LPInstance instance, int entrada, int saida, bool swapB
){
    std::swap(instance.objective[entrada], instance.objective[saida]);
    std::swap(instance.c_aux[entrada], instance.c_aux[saida]);
    for (size_t i = 0; i < instance.constraints.size(); i++){
        std::swap(
            instance.constraints[i].coefficients[entrada], instance.constraints[i].coefficients[saida]
        );
        // Trecho para garantir que posições de B só serão acessadas no caso especial da matriz
        if ( swapB ){
            std::swap(
                instance.reverseB[i][(entrada - instance.var_n)], instance.reverseB[i][(saida - instance.var_n)]
            );
        }
    }
    return instance;
}

LPInstance inversaCasoEspecial(
    LPInstance instance
){
    for (size_t i = 0; i < instance.reverseB.size(); i++){
        for (size_t j = 0; j < instance.reverseB[i].size(); j++){
            if (instance.reverseB[i][j] == 1 && i != j){
                instance = trocaColunas(instance, (instance.var_n + i), (instance.var_n + j), true);
            }
        }
    }
    return instance;
}

/**
 * @brief Calcula a matriz inversa usando eliminação de Gauss-Jordan.
 *
 * @param matriz A matriz para a qual a inversa será calculada.
 * @param identidade A matriz identidade correspondente.
 * @param tamanhoMatriz O tamanho da matriz (número de linhas e colunas).
 */
LPInstance calcularInversa(
    LPInstance instance,
    std::vector<std::vector<double>> &matriz, 
    std::vector<std::vector<double>> &identidade, 
    int tamanhoMatriz
)
{
    bool casoEspecial = false;
    casoEspecial = checkCasoEspecial(matriz);

    if (casoEspecial){
        std::cout << "Caso Especial de Matriz inversa (Identidade deslocada)" << std::endl;
        instance = inversaCasoEspecial(instance);
        return instance;
    }else{
        zerarElementosAcimaDiagonal(matriz, identidade, tamanhoMatriz);
        zerarElementosAbaixoDiagonal(matriz, identidade, tamanhoMatriz);
        for (int i = 0; i < tamanhoMatriz; i++)
        {
            double pivo = matriz[i][i];
            for (int j = 0; j < tamanhoMatriz; j++)
            {
                instance.reverseB[i][j] = identidade[i][j] / pivo;
            }
        }
        return instance;
    }
}

/**
 * @brief Calcula o determinante de uma matriz usando o método de Laplace.
 *
 * @param matriz A matriz para a qual o determinante será calculado.
 * @param tamanhoMatriz O tamanho da matriz (número de linhas e colunas).
 * @return O determinante da matriz.
 */
double calculaDeterminante(const std::vector<std::vector<double>> &matriz, int tamanhoMatriz)
{
    if (tamanhoMatriz == 1)
    {
        return matriz[0][0];
    }
    else
    {
        double determinante = 0.0;
        for (int i = 0; i < tamanhoMatriz; i++)
        {
            if (matriz[0][i] != 0)
            {
                std::vector<std::vector<double>> aux(tamanhoMatriz - 1, std::vector<double>(tamanhoMatriz - 1, 0.0));
                int iAux = 0;
                int jAux = 0;
                for (int j = 1; j < tamanhoMatriz; j++)
                {
                    for (int k = 0; k < tamanhoMatriz; k++)
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

void print_vector(std::vector<double> vetor, std::string text) {
    std::cout << text << " = [";
    for (auto e: vetor)
        std::cout << "," << e;
    std::cout << "] \n";
}