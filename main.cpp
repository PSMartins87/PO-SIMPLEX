#include <iostream>
#include "utils/InstanceReader.h"
#include "utils/Simplex.h"

int executar_otimizacao_d(std::string filename, bool show_steps);
int executar_otimizacao_p(std::string filename);

int main(){
    bool show_steps = false;         // Altere para "false" para obter o resultado direto
    std::string filename = "C:/src/VSCode_Workspace/C/TrabSimplex/a.lp"; 
    executar_otimizacao_d(filename, show_steps);
    //executar_otimizacao_p(filename);
}

int executar_otimizacao_d(std::string filename, bool show_steps)
{
    LPInstance instance = convert_obj_func_to_min(loadFile(filename));

    if ( show_steps == false){
        std::cout << "Modo step-by-step Desativado" << std::endl;
        simplex(instance);
        return 0;
    }
    
    if (instance.type)
    {
        std::cout << "Tipo de função: Maximizar" << std::endl;
    }
    else
    {
        std::cout << "Tipo de função: Minimizar" << std::endl;
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
    simplex(instance);
    return 0;
}

int executar_otimizacao_p(std::string filename)
{
    LPInstance instance = loadFile(filename);

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
