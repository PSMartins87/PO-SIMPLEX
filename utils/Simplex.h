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
void print_vector(std::vector<int> v) {
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

    for (int i = 0; i < m; i++) {
        for (int j = n; j < (n + m); j++) {
            if ((i + n) == j) {
                if (instance.constraints[i].signal == ">=" || instance.constraints[i].signal == ">") {
                    instance.constraints[i].coefficients.push_back(-1);
                    instance.objective.push_back(0);
                }
                else if (instance.constraints[i].signal == "<=" || instance.constraints[i].signal == "<") {
                    instance.constraints[i].coefficients.push_back(1);
                    instance.objective.push_back(0);
                }
                else if (instance.constraints[i].signal == "=") {
                    instance.constraints[i].coefficients.push_back(0);
                    instance.objective.push_back(0);
                }
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
    std::vector<double> c;

    for (int i = 0; i < instance.objective.size(); i++)
        c.push_back(instance.objective[i]);

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
    int n = instance.constraints.size();
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
    for (int i = abs(n - m); i < n; i++)
        (*B).push_back(i);
}

void create_non_basic_partition(std::vector<int> *N, int n, int m)
{
    for (int i = 0; i < abs(n - m); i++)
        (*N).push_back(i);
}

void create_B_matrix(std::vector<std::vector<double>> A, std::vector<std::vector<double>> *B, std::vector<int> basic_partition)
{
    for (int i = 0; i < basic_partition.size(); i++) {
        //std::cout << basic_partition[i] << std::endl;
        for (int j = 0; j < basic_partition.size(); j++) 
            (*B)[i][j] = A[i][basic_partition[j]];
    }
}

void create_ctb(std::vector<double> c, std::vector<int> basic_partition, std::vector<std::vector<double>> *ctb)
{
    for (int i = 0; i < basic_partition.size(); i++)
        (*ctb)[0][i] = c[basic_partition[i]];
}

int calculate_relative_costs(
    std::vector<std::vector<double>> A,
    std::vector<std::vector<double>> *relative_costs,
    std::vector<std::vector<double>> lambda_t, 
    std::vector<double> c, 
    std::vector<int> non_basic_partition)
{
    int goes_in = std::numeric_limits<int>::max();
    double min_relative_cost = std::numeric_limits<double>::max();

    for (int i = 0; i < non_basic_partition.size(); i++) {
        (*relative_costs)[i][0] = (c[non_basic_partition[i]] - (multiplicarMatrizes(lambda_t, pegaColuna(A, non_basic_partition[i]))[0][0]));
        if ((*relative_costs)[i][0] < min_relative_cost) {
            goes_in = i;
            min_relative_cost = (*relative_costs)[i][0];
        }
        //std::cout << (*relative_costs)[i][0] << std::endl;
    }
    return goes_in;
}

bool is_optimal(std::vector<std::vector<double>> relative_costs)
{
    for (auto matrix: relative_costs)
        for (auto e: matrix)
            if (e < 0)
                return false;
    return true;
}

void simplex_direction(std::vector<std::vector<double>> A, std::vector<std::vector<double>> B, int k, std::vector<std::vector<double>> *y)
{
    (*y) = multiplicarMatrizes(B, pegaColuna(A, k));
}

bool check_simplex_direction(std::vector<std::vector<double>> y)
{
    for (auto matrix: y)
        for (auto e: matrix)
            if (e > 0)
                return false;
    return true;
}

int check_which_variable_goes_off(std::vector<int> basic_partition, std::vector<std::vector<double>> y, std::vector<std::vector<double>> x_hat)
{
    int goes_off = std::numeric_limits<int>::max();
    double min = std::numeric_limits<double>::max();

    for (int i = 0; i < x_hat.size(); i++) {
        if ((x_hat[i][0] / y[i][0] < min) and y[i][0] > 0) {
            goes_off = i;
            min = x_hat[i][0] / y[i][0]; 
        }
    }


    return goes_off;
}

void swap_partitions(std::vector<int> *basic_partition, std::vector<int> *non_basic_partition, int goes_in, int goes_off)
{
    int pos_goes_in = 0, pos_goes_off = 0, aux = 0;

    for (int i = 0; i < (*basic_partition).size(); i++)
        if (i == goes_off) {
            pos_goes_off = i;
            break;
        }

    for (int i = 0; i < (*non_basic_partition).size(); i++)
        if (i == goes_in) {
            pos_goes_in = i;
            break;
        }

    aux = (*basic_partition)[pos_goes_off];
    (*basic_partition)[pos_goes_off] = (*non_basic_partition)[pos_goes_in];
    (*non_basic_partition)[pos_goes_in] = aux;
}

double solve(std::vector<std::vector<double>> A, std::vector<double> c, std::vector<std::vector<double>> b)
{
    int m = A.size();
    int n = c.size();
    double result = 0;
    std::vector<int> non_basic_partition;
    std::vector<int> basic_partition;
    std::vector<std::vector<double>> B (m, std::vector<double> (m, 0));
    std::vector<std::vector<double>> B_inv (m, std::vector<double> (m, 0));
    std::vector<std::vector<double>> x_hat_b;
    std::vector<std::vector<double>> lambda_t;

    create_non_basic_partition(&non_basic_partition, n, m);
    create_basic_partition(&basic_partition, n, m);
    create_B_matrix(A, &B, basic_partition);

    std::vector<std::vector<double>> ctb (1, std::vector<double> (basic_partition.size(), 0));
    std::vector<std::vector<double>> relative_costs (non_basic_partition.size(), std::vector<double> (1, 0));
    std::vector<std::vector<double>> y (B[0].size(), std::vector<double> (1, 0));

    create_ctb(c, basic_partition, &ctb);

    while (true)
    {
        B_inv = gaussJordan(B);
        x_hat_b = multiplicarMatrizes(B_inv, b);
        lambda_t = multiplicarMatrizes(ctb, B_inv);

        int goes_in = calculate_relative_costs(A, &relative_costs, lambda_t, c, non_basic_partition);

        if (is_optimal(relative_costs)) {
            std::cout << "Solucao otima encontrada!" << std::endl;
            break;
        }

        simplex_direction(A, B_inv, goes_in, &y);

        if (check_simplex_direction(y)) {
            std::cout << "Nao existe soluçao otima finita!" << std::endl;
            return -1;
        }

        int goes_off = check_which_variable_goes_off(basic_partition, y, x_hat_b);

        swap_partitions(&basic_partition, &non_basic_partition, goes_in, goes_off);

        create_B_matrix(A, &B, basic_partition);
    }

    for (int i = 0; i < basic_partition.size(); i++)    
        result += c[basic_partition[i]] * x_hat_b[i][0];
    return result;      
}

void simplex(LPInstance instance)   //Seria fase 1 apenas?
{
    double result = 0.0;
    instance = convert_obj_func_to_min(instance);
    instance = convert_to_standard_form(instance);
    auto A = create_constraint_matrix(instance);    
    auto c = create_cost_vector(instance);    
    auto b = create_bound_matrix(instance);         
    result = solve(A, c, b) ;
    std::cout << "Solucao otima eh " << result << std::endl;
}

// TODO: Algoritmo para encontrar solução ótima
// TODO: Encontrar vetores B e N
// TODO: Separar matrizes B e N
// TODO: Calculo de solução Básica (zerando não básicas)
