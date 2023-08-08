#include <iostream>
#include "utils/InstanceReader.h"
#include "utils/Simplex.h"

int main()
{
    std::string filename = "a.lp"; // Substitua "arquivo.lp" pelo caminho do arquivo .lp que deseja ler
    LPInstance instance = convert_obj_func_to_min(loadFile(filename));

    std::cout << "Tipo: " << (instance.type ? "Maximize" : "Minimize") << std::endl;

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
