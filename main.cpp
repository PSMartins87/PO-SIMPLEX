#include <iostream>
#include "utils/InstanceReader.h"
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
        if (i != 0)
        {
            std::cout << " ";
        }
        std::cout << instance.objective[i] << "x" << (i + 1);
    }
    std::cout << std::endl;

    std::cout << "Restricoes:" << std::endl;
    for (const Constraint &constraint : instance.constraints)
    {
        for (size_t i = 0; i < constraint.coefficients.size(); ++i)
        {
            if (i != 0)
            {
                std::cout << " + ";
            }
            std::cout << constraint.coefficients[i] << "x" << (i + 1);
        }
        std::cout << " " << constraint.signal << " " << constraint.bound << std::endl;
    }

    std::cout << "Bounds:" << std::endl;
    for (size_t i = 0; i < instance.bounds.size(); ++i)
    {
        std::cout << instance.bounds[i].lower_bound << " " << instance.bounds[i].lower_bound_sign << " "
                  << " x" << (i + 1) << " " << instance.bounds[i].upper_bound_sign << " " << instance.bounds[i].upper_bound << std::endl;
    }
}
