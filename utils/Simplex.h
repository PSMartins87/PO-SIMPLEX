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

// void create_artificial_problem(LPInstance instance)
// {
//     LPInstance aritificial_problem;

//     for (int i = 0; i < instance.objective.size(); i++)
//     {
//         aritificial_problem.objective[i] = 0
//     }
// }

void simplex(LPInstance instance)
{
    instance = convert_obj_func_to_min(instance);
    instance = convert_to_standard_form(instance);

    // if (verify_instance(converted_instance))
    //     /*LPInstance artificial_problem = */create_artificial_problem(instance);
}