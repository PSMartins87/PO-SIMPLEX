#include "./InstanceReader.h"
#include "./matriz.h"

void printMatrix(const std::vector<std::vector<double>>& matrix) {
    int rows = matrix.size();
    int cols = matrix[0].size();

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            std::cout << matrix[i][j] << "\t";
        }
        std::cout << std::endl;
    }
}

// TODO: RETIRAR ESSA FUNÇÃO DAQUI!!!
void print_vector(std::vector<double> v) {
    std::cout << "C = [";
    for (auto e: v)
        std::cout << "," << e;
    std::cout << "] \n";
}

LPInstance convert_obj_func_to_min(LPInstance instance)
{   
    if (instance.type)
    {
        std::cout << "Tipo Maximizar - converte para minimizar" << std::endl;
        LPInstance new_instance = instance;
        instance.type = false;
        for (int i = 0; i < instance.objective.size(); i++)
            new_instance.objective[i] = new_instance.objective[i] * (-1);   // Inversão da função objetivo
        return new_instance;
    }
    return instance;
}

// Checa a necessidade de formular o problema artificial, false se precisar e true caso contrário
bool verify_instance(LPInstance instance)
{
    for (auto constraint: instance.constraints)
        if ( constraint.signal != "<" || constraint.signal != "<="){
        // Função substituída por igualdade lógica
        //if (constraint.signal == ">" or constraint.signal == ">=" or constraint.signal == "=")  
            std::cout << "Necessita problema artificial" << std::endl;
            return false;
        }
    return true;
}

LPInstance convert_to_standard_form(LPInstance instance)
{    
    int m = instance.constraints.size();
    int n = instance.objective.size();

    for (int i = 0; i < instance.constraints.size(); i++) {
        for (int j = n; j < (n + m); j++) {
            if ((i + m) == j) {
                if (instance.constraints[i].signal == ">=" || instance.constraints[i].signal == ">")
                    instance.constraints[i].coefficients.push_back(-1);
                else if (instance.constraints[i].signal == "<=" || instance.constraints[i].signal == "<")
                    instance.constraints[i].coefficients.push_back(1);
                else if (instance.constraints[i].signal == "=")
                    instance.constraints[i].coefficients.push_back(0);
                instance.constraints[i].signal = "=";
            } else {
                instance.constraints[i].coefficients.push_back(0);
            }
        }
    }
    return instance;
}

std::vector<double>
create_cost_vector(LPInstance instance)
{
    std::vector<double> c (instance.objective);

    for (int i = 0; i < instance.constraints.size(); i++)
        c.push_back(0);

    return c;
}

std::vector<std::vector<double>>
create_bound_matrix(LPInstance instance)
{
    int m = instance.constraints.size();
    std::vector<std::vector<double>> b (m, std::vector<double> (1, 0));

    for (int i = 0; i < m; i++)
        for (int j = 0; j < 1; j++)
            b[i][j] = instance.constraints[i].bound;

    return b;
}

std::vector<std::vector<double>>
create_constraint_matrix(LPInstance instance)
{
    int n = instance.objective.size();
    std::vector<std::vector<double>> constraint_matrix;

    for (int i = 0; i < n; i++)
        constraint_matrix.push_back(instance.constraints[i].coefficients);
        
    return constraint_matrix;
}

void create_artificial_problem(std::vector<std::vector<double>> *A, std::vector<double> *c)
{
    int m = (*A).size();
    int n = (*c).size();

    // Zera os coeficientes da função objetivo e adiciona as variáveis artificiais
    for (int i = 0; i < (n + m); i++) {
        if (i >= n)
            (*c).push_back(1);
        else
            (*c)[i] = 0;
    }

    // Adiciona as variáveis artificiais nas restrições
    for (int i = 0; i < m; i++) {
        for (int j = n; j < (n + m); j++) {
            if ((i + n) == j)
                (*A)[i].push_back(1);
            else
                (*A)[i].push_back(0);            
        }
    }
}

void create_basic_partition(std::vector<int> *B, int n, int m)
{
    for (int i = n; i < (n + m); i++)
        (*B).push_back(i);
}

void create_non_basic_partition(std::vector<int> *N, int n)
{
    for (int i = 0; i < n; i++)
        (*N).push_back(i);
}

void create_B_matrix(std::vector<std::vector<double>> A, std::vector<std::vector<double>> *B, std::vector<int> basic_partition)
{
    for (int i = 0; i < basic_partition.size(); i++)
        for (int j = 0; j < basic_partition.size(); j++)
            (*B)[i][j] = A[j][basic_partition[i]];
}

void solve_artificial_problem(std::vector<std::vector<double>> A, std::vector<double> c, std::vector<std::vector<double>> b)
{
    int m = A.size();
    int n = c.size();
    std::vector<int> non_basic_partition;
    std::vector<int> basic_partition;
    std::vector<std::vector<double>> B (m, std::vector<double> (m, 0));
    std::vector<std::vector<double>> x_hat_b;

    create_artificial_problem(&A, &c);
    create_non_basic_partition(&non_basic_partition, n);
    create_basic_partition(&basic_partition, n, m);
    create_B_matrix(A, &B, basic_partition);
    // Calcular a inversa de B
    x_hat_b = multiplicarMatrizes(B, b);
    printMatrix(x_hat_b);
}

void simplex(LPInstance instance)   //Seria fase 1 apenas?
{
    instance = convert_obj_func_to_min(instance);

    if (!verify_instance(instance)) {                   // Verificação de problema artificial
        instance = convert_to_standard_form(instance);
        auto A = create_constraint_matrix(instance);    // Matriz [A] = [N | B]
        auto c = create_cost_vector(instance);          // Vetor [Ctn | Ctb]
        auto b = create_bound_matrix(instance);         // Coluna [b]
        solve_artificial_problem(A, c, b);
        return;
    }
}

// TODO: Algoritmo para encontrar solução ótima
// TODO: Encontrar vetores B e N
// TODO: Separar matrizes B e N
// TODO: Calculo de solução Básica (zerando não básicas)
