#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
using namespace std;

struct Constraint
{
    string name;
    string expression;
};

struct VariableBound
{
    string name;
    double lower_bound;
    double upper_bound;
};

struct LPInstance
{
    bool type;
    vector<Constraint> constraints;
    vector<VariableBound> bounds;
};

LPInstance analyzeOptimizationType(const string &filename)
{
    LPInstance instance;
    ifstream file(filename);
    if (!file.is_open())
    {
        cout << "Erro ao abrir o arquivo: " << filename << endl;
    }

    string line;
    while (getline(file, line))
    {
        if (line.find("Maximize") != string::npos)
        {
            instance.type = 1;
        }
        else if (line.find("Minimize") != string::npos)
        {
            instance.type = 0;
        }
    }

    file.close();
    return instance;
}

int main()
{
    string filename = "seu_arquivo.lp"; // Substitua pelo nome correto do arquivo.
    LPInstance instance = analyzeOptimizationType(filename);
    if (instance.type == 1)
    {
        cout << "O arquivo é de maximização." << endl;
    }
    else
    {
        cout << "O arquivo é de minimização." << endl;
    }
    return 0;
}
