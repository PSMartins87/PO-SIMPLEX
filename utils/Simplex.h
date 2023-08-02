#include "./InstanceReader.h"
#include "./inversa.h"

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