#ifndef INSTANCEREADER_H
#define INSTANCEREADER_H
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <regex>
#include <limits>

struct VariableBound
{
    std::string lower_bound_sign; // "<", ">", "<=" or ">="
    double lower_bound;
    std::string upper_bound_sign; // "<", ">", "<=" or ">="
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

double parseDouble(const std::string &str)
{
    if (str == "inf")
    {
        return -std::numeric_limits<double>::infinity();
    }
    else if (str == "-inf")
    {
        return std::numeric_limits<double>::infinity();
    }
    else
    {
        return std::stod(str);
    }
}

std::vector<double> extractCoefficients(const std::string &expression)
{
    std::vector<double> coefficients;
    std::regex pattern(R"(([+-]?\s*(\d+(\.\d+)?)?)\s*[xX](\d+))");
    std::sregex_iterator it(expression.begin(), expression.end(), pattern);
    std::sregex_iterator end;

    while (it != end)
    {
        std::string coef_str = (*it)[1].str();
        coef_str = std::regex_replace(coef_str, std::regex(R"(\s)"), ""); // Remove espaços em branco
        double coefficient = coef_str.empty() ? 1.0 : std::stod(coef_str);

        int index = std::stoi((*it)[4].str());
        if (index > coefficients.size())
        {
            coefficients.resize(index, 0);
        }
        coefficients[index - 1] = coefficient;

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
        return instance;
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
        if (line.find("Minimize") != std::string::npos)
        {
            instance.type = false; // 0 representa minimizar
        }
        if (line.find("obj:") != std::string::npos)
        {
            objective = line;
            instance.objective = extractCoefficients(objective);
        }
        if (line.find("Subject To") != std::string::npos)
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
        if (line.find("Bounds") != std::string::npos)
        {
            inBounds = true;
        }
        else if (inBounds && line.find("End") == std::string::npos)
        {
            std::smatch match;

            // Padrão para verificar bounds do tipo "0 <= x1 <= 5" ou "x2 >= 8"
            std::regex pattern(R"(\s*(-?inf|\d*)\s*([<>=]*)\s*x(\d*)\s*([<>=]*)\s*(-?inf|\d*)\s*)");

            if (std::regex_match(line, match, pattern))
            {
                VariableBound bound;

                bound.lower_bound_sign = match[2].str().empty() ? "<" : match[2].str();
                bound.lower_bound = (match[1].str().empty() || match[1].str() == "-inf") ? -std::numeric_limits<double>::infinity() : parseDouble(match[1].str());
                bound.upper_bound_sign = match[4].str().empty() ? "<" : match[4].str();
                bound.upper_bound = (match[5].str().empty() || match[5].str() == "inf") ? std::numeric_limits<double>::infinity() : parseDouble(match[5].str());

                instance.bounds.push_back(bound);
            }
        }
    }
    return instance;
}
#endif // INSTANCEREADER_H