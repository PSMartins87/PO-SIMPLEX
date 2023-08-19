#include "./InstanceReader.h"
#include "./matriz.h"

LPInstance convert_obj_func_to_min(LPInstance instance)
{   
    if (instance.type)
    {
        std::cout << "Tipo Maximizar - converte para minimizar" << std::endl;
        LPInstance new_instance = instance;
        instance.type = false;
        for (size_t i = 0; i < instance.objective.size(); i++)
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
            std::cout << "Necessita problema artificial" << std::endl;
            return false;
        }
    return true;
}

LPInstance convert_to_standard_form(LPInstance instance)
{    
    for (size_t i = 0; i < instance.constraints.size(); i++) {
        if (instance.constraints[i].signal == ">=" || instance.constraints[i].signal == ">"){
            for (size_t index = 0; index < instance.constraints.size(); index++){
                if (index == i){
                    instance.constraints[index].coefficients.push_back(-1);
                }else{
                    instance.constraints[index].coefficients.push_back(0);
                }
            }
        }         
        else if (instance.constraints[i].signal == "<=" || instance.constraints[i].signal == "<"){
            for (size_t index = 0; index < instance.constraints.size(); index++){
                if (index == i){
                    instance.constraints[index].coefficients.push_back(1);
                }else{
                    instance.constraints[index].coefficients.push_back(0);
                }
            }
        }
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

void create_artificial_problem(std::vector<std::vector<double>> *A, std::vector<double> *c, int n)
{
    bool artificial_line = true;

    for (size_t c_index = 0; c_index < (*c).size(); c_index++){
        (*c)[c_index] = 0;
    }

    for (size_t i = 0; i < (*A).size(); i++){
        for (size_t j = n; j < (*A)[i].size(); j++){
            if ((*A)[i][j] != 0){
                artificial_line = false;
            }
        }
        if (artificial_line == true){
            for (size_t index = 0; index < (*A).size(); index++){
                if (index == i){
                    (*A)[index].push_back(1);
                    (*c).push_back(1);
                }else{
                    (*A)[index].push_back(0);
                }
            }
        }
    }
}

// TODO: RETIRAR ESSA FUNÇÃO DAQUI!!!
void print_vector(std::vector<double> v) {
    std::cout << "C = [";
    for (auto e: v)
        std::cout << "," << e;
    std::cout << "] \n";
}

void solve_artificial_problem(
    std::vector<std::vector<double>> A, std::vector<double> c, std::vector<double> b, int n
    )
{
    bool solved = false;
    create_artificial_problem(&A, &c, n);
    print_vector(c);
    std::cout << "A = " << std::endl;
    mostrarMatriz(A);
    std::vector<std::vector<double>> I;
    preencheIdentidade(I, A.size());

    while(!solved){
        // Obtem B
        std::vector<std::vector<double>> reverseB;
        for (size_t i = 0; i < A.size(); i++){
            std::vector<double> b_line;
            for (size_t j = n; j < A[i].size(); j++){
                b_line.push_back(A[i][j]);
            }
            reverseB.push_back(b_line);
            b_line.clear();
        }
        std::cout << "B = " << std::endl;
        mostrarMatriz(reverseB);
        // Obtem B inversa
        calcularInversa(reverseB, I, reverseB.size());
        std::cout << "reverseB = " << std::endl;
        mostrarMatriz(reverseB);
    }


}

void simplex(LPInstance instance)   //Seria fase 1 apenas?
{
    instance = convert_obj_func_to_min(instance);

    if (!verify_instance(instance)) {                   // Verificação de problema artificial
        int n = instance.objective.size();
        instance = convert_to_standard_form(instance);
        auto A = create_constraint_matrix(instance);    // Matriz [A] = [N | B]
        auto c = create_cost_vector(instance);          // Vetor [Ctn | Ctb]
        auto b = create_bound_vector(instance);         // Coluna [b]
        solve_artificial_problem(A, c, b, n);
    }
}
