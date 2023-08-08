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

void simplex(LPInstance instance)
{
    instance = convert_obj_func_to_min(instance);

    if (!verify_instance(instance)) {
        instance = convert_to_standard_form(instance);
        auto A = create_constraint_matrix(instance);
        auto c = create_cost_vector(instance);
        auto b = create_bound_vector(instance);
    }
}
