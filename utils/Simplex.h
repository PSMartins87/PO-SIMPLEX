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
    instance.is_artificial = false;
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
        }else{
            instance.is_artificial = true;
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

    if (instance.is_artificial){
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
    }

    return instance;
}

LPInstance solve_artificial_problem( LPInstance instance )
{
    bool solved = false;
    std::vector<double> c_copy = instance.objective;

    std::vector<std::vector<double>> I;             // Cria a matriz identidade [I]
    preencheIdentidade(I, instance.constraints.size());

    instance = create_artificial_problem(instance);

    instance.c_aux = instance.objective;
    for (size_t i = 0; i < c_copy.size(); i++){
        instance.c_aux[i] = c_copy[i];
    }

    while(!solved){
        // Obtem C (na forma de matriz), necessário para trabalhar com multiplicação de matrizes
        std::vector<double> c;
        for (size_t i = 0; i < instance.objective.size(); i++){
            c.push_back(instance.objective[i]);
        }
        // Ontem b (na forma de matriz)=======================================================
        std::vector<double> b;
        for (size_t i = 0; i < instance.constraints.size(); i++){
            b.push_back(instance.constraints[i].bound);
        }
        // Obtem [B] A - N  ===================================================================
        for (size_t i = 0; i < instance.constraints.size() ; i++){
            std::vector<double> b_line;
            for (size_t j = instance.var_n; j < instance.constraints[i].coefficients.size(); j++){
                b_line.push_back(instance.constraints[i].coefficients[j]);
            }
            instance.reverseB.push_back(b_line);
            b_line.clear();
        }
        mostrarMatrizB(instance);

        // Obtem B inversa
        instance = calcularInversa(instance, instance.reverseB, I, instance.reverseB.size());
        mostrarMatrizInversaB(instance);

        // Obtem X chapéu
        std::vector<double> hatX = multiplicaMatrizVetor(instance.reverseB, b);
        print_vector(hatX, "hatX");

        // Separa trecho [CtB]
        std::vector<double> ctb;
        for (size_t i = instance.var_n; i < instance.objective.size(); i++){
            ctb.push_back(instance.objective[i]);
        }
        print_vector(ctb, "CtB");

        // Obtem Lambda T
        std::vector<double> lambdaT = multiplicaMatrizVetor(instance.reverseB, ctb);
        print_vector(lambdaT, "LambdaT");

        // Separa [N] = [A] - [B]
        std::vector<double> N_line;
        std::vector<std::vector<double>> N_matrix;
        for (size_t i = 0; i < instance.constraints.size(); i++){
            for (size_t j = 0; j < instance.var_n; j++){
                N_line.push_back(instance.constraints[i].coefficients[j]);
            }
            N_matrix.push_back(N_line);
            N_line.clear();
        }

        // Declarações de variaveis a serem usadas
        std::vector<double> hatC, N_column;
        int result, exit_var, entry_var, smallest_result = 1;

        // Iteração para encontrar C chapéu da função
        for (size_t i = 0; i < instance.var_n; i++){
            for (size_t n_i = 0; n_i <  N_matrix.size(); n_i++){
                for (size_t n_j = 0; n_j < N_matrix[n_i].size(); n_j++){
                    if (n_j == i){
                        N_column.push_back(N_matrix[n_i][n_j]);
                    }
                }
            }
            result = instance.objective[i] - multiplicarVetorVetor(lambdaT, N_column);
            if (result < smallest_result){
                smallest_result = result;
                entry_var = i;
            }
            hatC.push_back(result);
            result = 0;
            N_column.clear();
        }
        print_vector(hatC, "hatC");

        // Verifica se o problema foi resolvido, caso seja corta a execução
        solved = true;
        for (size_t i = 0; i < hatC.size(); i++){
            if (hatC[i] < 0){
                solved = false;
            }
        }
        if (solved){ break; }

        for (size_t a_i = 0; a_i < instance.constraints.size(); a_i++){
            for (size_t a_j = 0; a_j < instance.constraints[a_i].coefficients.size(); a_j++){
                if (a_j == entry_var){
                    N_column.push_back(instance.constraints[a_i].coefficients[a_j]);
                }
            }
        }
        // Obtem vetor Y
        std::vector<double> Y;
        Y = multiplicaMatrizVetor(instance.reverseB, N_column);
        print_vector(Y, "Y");

        double result2 = 9999;
        
        for (size_t i = 0; i < Y.size(); i++){
            if ( ( hatX[i] / Y[i] ) < result2){
                result2 = hatX[i] / Y[i];
                exit_var = (instance.var_n + i);
            }
        }

        std::cout << "Sai pos:" << exit_var << " Entra pos:" << entry_var << std::endl;
        instance = trocaColunas(instance, entry_var, exit_var);

        instance.reverseB.clear();
    }
    // Texto de resolução
    std::cout << "=========================================================" << std::endl;
    std::cout << "Problema artificial resolvido" << std::endl;
    std::cout << "=========================================================" << std::endl;

    // Exclui coluna das variaveis artificiais
    for (size_t i = 0; i < instance.objective.size(); i++){
        if (instance.objective[i] == 1){
            instance = eliminaColuna(instance, i);
            if (i < instance.c_aux.size()){
                instance.c_aux.erase(instance.c_aux.begin() + i);
            }
            instance.var_n--;
        }
    }
    // Faz alterações finais
    instance.objective = instance.c_aux;
    instance.c_aux.clear();
    instance.reverseB.clear();
    instance.is_artificial = false;

    return instance;
}


void simplex(LPInstance instance)   //Seria fase 1 apenas?
{
    bool is_solved = false;
    instance = convert_obj_func_to_min(instance);
    instance.var_n = instance.objective.size();

    if (!verify_instance(instance)) {                   // Verificação de problema artificial
        instance = convert_to_standard_form(instance);
        instance = solve_artificial_problem(instance);
    }
    std::vector<std::vector<double>> I;             // Cria a matriz identidade [I]
    preencheIdentidade(I, instance.constraints.size());
    while(!is_solved){
        // Obtem C (na forma de matriz), necessário para trabalhar com multiplicação de matrizes
        std::vector<double> c;
        for (size_t i = 0; i < instance.objective.size(); i++){
            c.push_back(instance.objective[i]);
        }
        // Ontem b (na forma de matriz)=======================================================
        std::vector<double> b;
        for (size_t i = 0; i < instance.constraints.size(); i++){
            b.push_back(instance.constraints[i].bound);
        }
        // Obtem [B] A - N  ===================================================================
        for (size_t i = 0; i < instance.constraints.size() ; i++){
            std::vector<double> b_line;
            for (size_t j = instance.var_n; j < instance.constraints[i].coefficients.size(); j++){
                b_line.push_back(instance.constraints[i].coefficients[j]);
            }
            instance.reverseB.push_back(b_line);
            b_line.clear();
        }
        mostrarMatrizB(instance);

        // Obtem B inversa
        instance = calcularInversa(instance, instance.reverseB, I, instance.reverseB.size());
        mostrarMatrizInversaB(instance);

        // Obtem X chapéu
        std::vector<double> hatX = multiplicaMatrizVetor(instance.reverseB, b);
        print_vector(hatX, "hatX");

        // Separa trecho [CtB]
        std::vector<double> ctb;
        for (size_t i = instance.var_n; i < instance.objective.size(); i++){
            ctb.push_back(instance.objective[i]);
        }
        print_vector(ctb, "CtB");

        // Obtem Lambda T
        std::vector<double> lambdaT = multiplicaMatrizVetor(instance.reverseB, ctb);
        print_vector(lambdaT, "LambdaT");

        // Separa [N] = [A] - [B]
        std::vector<double> N_line;
        std::vector<std::vector<double>> N_matrix;
        for (size_t i = 0; i < instance.constraints.size(); i++){
            for (size_t j = 0; j < instance.var_n; j++){
                N_line.push_back(instance.constraints[i].coefficients[j]);
            }
            N_matrix.push_back(N_line);
            N_line.clear();
        }

        // Declarações de variaveis a serem usadas
        std::vector<double> hatC, N_column;
        int result, exit_var, entry_var, smallest_result = 1;

        // Iteração para encontrar C chapéu da função
        for (size_t i = 0; i < instance.var_n; i++){
            for (size_t n_i = 0; n_i <  N_matrix.size(); n_i++){
                for (size_t n_j = 0; n_j < N_matrix[n_i].size(); n_j++){
                    if (n_j == i){
                        N_column.push_back(N_matrix[n_i][n_j]);
                    }
                }
            }
            result = instance.objective[i] - multiplicarVetorVetor(lambdaT, N_column);
            if (result < smallest_result){
                smallest_result = result;
                entry_var = i;
            }
            hatC.push_back(result);
            result = 0;
            N_column.clear();
        }
        print_vector(hatC, "hatC");

        // Verifica se o problema foi resolvido, caso seja corta a execução
        is_solved = true;
        for (size_t i = 0; i < hatC.size(); i++){
            if (hatC[i] < 0){
                is_solved = false;
            }
        }
        if (is_solved){ break; }

        for (size_t a_i = 0; a_i < instance.constraints.size(); a_i++){
            for (size_t a_j = 0; a_j < instance.constraints[a_i].coefficients.size(); a_j++){
                if (a_j == entry_var){
                    N_column.push_back(instance.constraints[a_i].coefficients[a_j]);
                }
            }
        }
        // Obtem vetor Y
        std::vector<double> Y;
        Y = multiplicaMatrizVetor(instance.reverseB, N_column);
        print_vector(Y, "Y");

        double result2 = 9999;
        
        for (size_t i = 0; i < Y.size(); i++){
            if ( ( hatX[i] / Y[i] ) < result2){
                result2 = hatX[i] / Y[i];
                exit_var = (instance.var_n + i);
            }
        }

        std::cout << "Sai pos:" << exit_var << " Entra pos:" << entry_var << std::endl;
        instance = trocaColunas(instance, entry_var, exit_var);

        instance.reverseB.clear();
    }
    std::cout << "=========================================================" << std::endl;
    std::cout << "Problema Resolvido" << std::endl;
    std::cout << "=========================================================" << std::endl;
    mostrarMatrizA(instance);
    mostrarVetorObjetivo(instance);
}
