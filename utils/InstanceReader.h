#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <regex>

struct VariableBound
{
    double lower_bound;
    double upper_bound;
};

struct Constraint
{
    std::string signal;
    std::vector<double> coefficients;
    double bound;
};

struct LPInstance
{
    bool type;
    std::vector<double> objective;
    std::vector<Constraint> constraints;
    std::vector<VariableBound> bounds;
};

std::vector<double> extractCoefficients(const std::string &expression)
{
    std::vector<double> coefficients;
    std::regex pattern(R"(([+-]?\s*\d+(\.\d+)?)\s*[xX](\d+))");
    std::sregex_iterator it(expression.begin(), expression.end(), pattern);
    std::sregex_iterator end;

    while (it != end)
    {
        std::string coef_str = (*it)[1].str();
        coef_str = std::regex_replace(coef_str, std::regex(R"(\s)"), ""); // Remove espaços em branco
        double coefficient = 1.0;
        if (!coef_str.empty())
        {
            coefficient = std::stod(coef_str);
        }

        if ((*it)[3].matched)
        {
            int index = std::stoi((*it)[3].str());
            if (index > coefficients.size())
            {
                coefficients.resize(index, 0);
            }
            coefficients[index - 1] = coefficient;
        }
        else
        {
            coefficients.push_back(coefficient);
        }

        ++it;
    }

    return coefficients;
}

LPInstance loadFile(const std::string &filename)
{
    LPInstance instance;
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cout << "Erro ao abrir o arquivo: " << filename << std::endl;
    }

    std::string line;
    std::string objective;
    std::string constraint;
    bool inBounds = false;
    while (std::getline(file, line))
    {
        if (line.find("Maximize") != std::string::npos)
        {
            instance.type = true; // 1 representa maximizar
        }
        else if (line.find("Minimize") != std::string::npos)
        {
            instance.type = false; // 0 representa minimizar
        }
        if (line.find("obj:") != std::string::npos)
        {
            objective = line;
            instance.objective = extractCoefficients(objective);
        }
        else if (line.find("Subject To") != std::string::npos)
        {
            inBounds = false;
            while (std::getline(file, line) && line.find("Bounds") == std::string::npos)
            {
                Constraint c;
                std::regex signPattern("([<>]=?)\\s?(-?\\d+)");
                std::smatch signMatch;
                if (std::regex_search(line, signMatch, signPattern))
                {
                    c.signal = signMatch[1].str();
                    c.coefficients = extractCoefficients(line);
                    c.bound = std::stod(signMatch[2].str());
                    instance.constraints.push_back(c);
                }
            }
        }
        else if (line.find("Bounds") != std::string::npos)
        {
            inBounds = true;
        }
        else if (inBounds && line.find("End") == std::string::npos)
        {
            VariableBound v;
            std::regex boundPattern(R"(\s*(\d+)\s*<=\s*x(\d+)\s*<=\s*(\d+))");
            std::smatch boundMatch;
            if (std::regex_search(line, boundMatch, boundPattern))
            {
                int index = std::stoi(boundMatch[2].str());
                if (index > instance.bounds.size())
                {
                    instance.bounds.resize(index, VariableBound{0, 0});
                }
                instance.bounds[index - 1].lower_bound = std::stod(boundMatch[1].str());
                instance.bounds[index - 1].upper_bound = std::stod(boundMatch[3].str());
            }
        }
    }

    file.close();
    return instance;
}