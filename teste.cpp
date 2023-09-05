#include "utils/InstanceReader.h"
#include "utils/Simplex.h"
//#include "utils/matriz.h"

int main()
{
    LPInstance instance = loadFile();
    simplex(instance);

    return 0;
}