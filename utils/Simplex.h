#include "./InstanceReader.h"
#include "./matriz.h"

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
            instance.objective.push_back(0);
            for (size_t index = 0; index < instance.constraints.size(); index++){
                if (index == i){
                    instance.constraints[index].coefficients.push_back(-1);
                }else{
                    instance.constraints[index].coefficients.push_back(0);
                }
            }
        }         
        else if (instance.constraints[i].signal == "<=" || instance.constraints[i].signal == "<"){
            instance.objective.push_back(0);
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

LPInstance create_artificial_problem(LPInstance instance)
{
    bool artificial_line = true;

    for (size_t c_index = 0; c_index < instance.objective.size(); c_index++){
        instance.objective[c_index] = 0;
    }

    for (size_t i = 0; i < instance.constraints.size(); i++){
        for (size_t j = instance.var_n; j < instance.constraints[i].coefficients.size(); j++){
            if (instance.constraints[i].coefficients[j] != 0){
                artificial_line = false;
            }
        }
        if (artificial_line == true){
            for (size_t index = 0; index < instance.constraints.size(); index++){
                if (index == i){
                    instance.constraints[index].coefficients.push_back(1);
                    instance.objective.push_back(1);
                }else{
                    instance.constraints[index].coefficients.push_back(0);
                }
            }
        }
        artificial_line = true;
    }

    return instance;
}

LPInstance solve_artificial_problem( LPInstance instance )
{
    bool solved = false;
    std::vector<std::vector<double>> I;             // Cria a matriz identidade [I]
    preencheIdentidade(I, instance.constraints.size());

    //mostrarVetorObjetivo(instance);  
    //mostrarMatrizA(instance);
    instance = create_artificial_problem(instance);

    mostrarVetorObjetivo(instance);  
    mostrarMatrizA(instance);

    while(!solved){
        // Obtem C (na forma de matriz), necessário para trabalhar com multiplicação de matrizes
        std::vector<std::vector<double>> c_matrix;
        std::vector<double> c_matrix_line;

        for (size_t i = 0; i < instance.objective.size(); i++){
            c_matrix_line.push_back(instance.objective[i]);
        }
        c_matrix.push_back(c_matrix_line);
        // Ontem b (na forma de matriz)
        std::vector<std::vector<double>> b_matrix;
        std::vector<double> b_matrix_line;
        for (size_t i = 0; i < instance.constraints.size(); i++){
            b_matrix_line.push_back(instance.constraints[i].bound);
        }
        b_matrix.push_back(b_matrix_line);
        // Obtem [B] A - N
        std::vector<std::vector<double>> reverseB;
        for (size_t i = 0; i < instance.constraints.size() ; i++){
            std::vector<double> b_line;
            for (size_t j = instance.var_n; j < instance.constraints[i].coefficients.size(); j++){
                b_line.push_back(instance.constraints[i].coefficients[j]);
            }
            reverseB.push_back(b_line);
            b_line.clear();
        }
        std::cout << "B = " << std::endl;
        mostrarMatriz(reverseB);
        // Parte com problema
        // Obtem B inversa
        calcularInversa(reverseB, I, reverseB.size());
        std::cout << "reverseB = " << std::endl;
        mostrarMatriz(reverseB);
        // RESOLVER ESSE PROBLEMA ACIMA
        // CORRIGIR calcularInversa

        std::vector<std::vector<double>> hatX = multiplicarMatrizes(reverseB, b_matrix);
        std::vector<std::vector<double>> lambdaT = multiplicarMatrizes(c_matrix, reverseB);

        std::vector<double> hatC;
        std::vector<std::vector<double>> subA;
        int result, exit_var, entry_var, smallest_result = 1;

        for (size_t i = 0; i < instance.var_n; i++){
            for (size_t a_i = 0; a_i < instance.constraints.size(); a_i++){
                for (size_t a_j = 0; a_j < instance.constraints[a_i].coefficients.size(); a_j++){
                    if (a_j == i){
                        //subA.push_back(instance.constraints[a_i].coefficients[a_j]);
                    }
                }
            }
            // Resolver incompatibilidade multiplicarMatrizes na forma de double
            mostrarMatriz(multiplicarMatrizes(lambdaT, subA));
            //result = instance.objective[i] - multiplicarMatrizes(lambdaT, subA);
            if (result < smallest_result){
                smallest_result = result;
                entry_var = i;
            }
            hatC.push_back(result);
            result = 0;
            subA.clear();
        }
        
        solved = true;
        for (size_t i = 0; i < hatC.size(); i++){
            if (hatC[i] < 0){
                solved = false;
            }
        }

        if (solved){
            break;
        }

        for (size_t a_i = 0; a_i < instance.constraints.size(); a_i++){
            for (size_t a_j = 0; a_j < instance.constraints[a_i].coefficients.size(); a_j++){
                if (a_j == entry_var){
                    subA[a_i].push_back(instance.constraints[a_i].coefficients[a_j]);
                }
            }
        }

        std::vector<std::vector<double>> Y = multiplicarMatrizes(reverseB, subA);
        double hatE;
        result = 9999;
        for (size_t i = 0; i < Y.size(); i++){
            if ( ( hatX[0][i] / Y[0][i] ) < result){
                result = hatX[0][i] / Y[0][i];
                exit_var = i;
            }
        }

        instance = trocaColunas(instance, entry_var, exit_var);
    }

    for (size_t i = 0; i < instance.objective.size(); i++){
        if (instance.objective[i] == 1){
            instance = eliminaColuna(instance, i);
            instance.var_n--;
        }
    }

    return instance;
}

void simplex(LPInstance instance)   //Seria fase 1 apenas?
{
    instance = convert_obj_func_to_min(instance);
    instance.var_n = instance.objective.size();

    if (!verify_instance(instance)) {                   // Verificação de problema artificial
        instance = convert_to_standard_form(instance);
        instance = solve_artificial_problem(instance);
    }
    // solve_problem(lpinstance);
}
