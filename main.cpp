#include <iostream>
#include "utils/Simplex.h"
int main()
{
    LPInstance instance = convert_obj_func_to_min(loadFile("a.lp"));
    simplex(instance);

    return 0;
    if (instance.type)
    {
        std::cout << "Maximize" << std::endl;
    }
    else
    {
        std::cout << "Minimize" << std::endl;
    }
    std::cout << "\n";
    std::cout << "Funcao objetivo: ";
    for (size_t i = 0; i < instance.objective.size(); ++i)
    {
        std::cout << instance.objective[i] << "x" << (i + 1);
        if (i != instance.objective.size() - 1)
        {
            std::cout << " + ";
        }
    }
    std::cout << "\n";
    std::cout << "\n";
    std::cout << "Restricoes: " << std::endl;
    for (const auto &constraint : instance.constraints)
    {
        for (size_t i = 0; i < constraint.coefficients.size(); ++i)
        {
            std::cout << constraint.coefficients[i] << "x" << (i + 1);
            if (i != constraint.coefficients.size() - 1)
            {
                std::cout << " ";
            }
        }
        std::cout << " " << constraint.signal << " " << constraint.bound << std::endl;
    }
    std::cout << "\n";
    std::cout << "Bounds:\n";
    for (size_t i = 0; i < instance.bounds.size(); ++i)
    {
        std::cout << instance.bounds[i].lower_bound << " <= x" << (i + 1) << " <= " << instance.bounds[i].upper_bound << std::endl;
    }
    return 0;
}