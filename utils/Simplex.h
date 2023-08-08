#include "./InstanceReader.h"
#include "./matriz.h"

LPInstance convert_obj_func_to_min(LPInstance instance)
{
    if (instance.type)
    {
        LPInstance new_instance = instance;
        instance.type = false;
        for (int i = 0; i < instance.objective.size(); i++)
            new_instance.objective[i] = new_instance.objective[i] * (-1);
        return new_instance;
    }
    return instance;
}

// Checa a necessidade de formular o problema artificial, false se precisar e true caso contrário
bool verify_instance(LPInstance instance)
{
    for (auto constraint: instance.constraints)
        if (constraint.signal == ">" or constraint.signal == ">=" or constraint.signal == "=")
            return false;
    return true;
}

LPInstance convert_to_standard_form(LPInstance instance)
{    
    for (int i = 0; i < instance.constraints.size(); i++) {
        if (instance.constraints[i].signal == ">=" or instance.constraints[i].signal == ">")
            instance.constraints[i].coefficients.push_back(-1);
        else if (instance.constraints[i].signal == "<=" or instance.constraints[i].signal == "<")
            instance.constraints[i].coefficients.push_back(1);
        instance.constraints[i].signal = "=";
    }

    return instance;
}

std::vector<double>
create_cost_vector(LPInstance instance)
{
    std::vector<double> c (instance.objective);

    return c;
}

std::vector<double>
create_bound_vector(LPInstance instance)
{
    int m = instance.constraints.size();
    std::vector<double> b;

    for (int i = 0; i < m; i++)
        b.push_back(instance.constraints[i].bound);

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
            if ((i + m) == j)
                (*A)[i].push_back(1);
            else
                (*A)[i].push_back(0);            
        }
    }
}

// TODO: RETIRAR ESSA FUNÇÃO DAQUI!!!
void print_vector(std::vector<double> v) {
    for (auto e: v)
        std::cout << e << std::endl;
}

void solve_artificial_problem(std::vector<std::vector<double>> A, std::vector<double> c, std::vector<double> b)
{
    create_artificial_problem(&A, &c);
    print_vector(c);
    mostrarMatriz(A);
    // TODO: Implementar a resolução do problema artificial
}

void simplex(LPInstance instance)
{
    instance = convert_obj_func_to_min(instance);

    if (!verify_instance(instance)) {
        instance = convert_to_standard_form(instance);
        auto A = create_constraint_matrix(instance);
        auto c = create_cost_vector(instance);
        auto b = create_bound_vector(instance);
        solve_artificial_problem(A, c, b);
    }
}
