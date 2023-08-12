#ifndef INSTANCEREADER_H
#define INSTANCEREADER_H
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <regex>
#include <limits>
#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <limits>

struct VariableBound
{
    std::string lower_bound_sign;
    double lower_bound;
    std::string upper_bound_sign;
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

std::vector<double> extractCoefficients(const std::string &expression, int numVariables = 0)
{
    std::string expressionWithoutSpaces = std::regex_replace(expression, std::regex("\\s+"), "");
    std::vector<double> coefficients;
    std::regex pattern(R"(([+-]?\d*(\.\d+)?)[xX](\d+))");
    std::sregex_iterator it(expressionWithoutSpaces.begin(), expressionWithoutSpaces.end(), pattern);
    std::sregex_iterator end;
    coefficients = std::vector<double>(numVariables, 0.0);
    while (it != end)
    {
        double coefficient = 0.0;
        if ((*it)[1].str() == "+" || (*it)[1].str().empty())
        {
            coefficient = 1.0;
        }
        else if ((*it)[1].str() == "-")
        {
            coefficient = -1.0;
        }
        else
        {
            coefficient = std::stod((*it)[1].str());
        }

        int index = std::stoi((*it)[3].str());
        if (index > coefficients.size())
        {
            coefficients.resize(index, 0);
        }
        coefficients[index - 1] = coefficient;

        ++it;
    }
    return coefficients;
}

LPInstance loadFile()
{
    LPInstance instance;
    std::string line;
    bool inBounds = false;
    int numVariables = 0;
    while (std::getline(std::cin, line))
    {
        line = std::regex_replace(line, std::regex("\\s+"), ""); // Remove espaços em branco
        try
        {
            if (line.find("Maximize") != std::string::npos)
            {
                instance.type = true;
            }
            if (line.find("Minimize") != std::string::npos)
            {
                instance.type = false;
            }
            if (line.find("obj:") != std::string::npos)
            {
                instance.objective = extractCoefficients(line);
                numVariables = instance.objective.size();
            }
            if (line.find("SubjectTo") != std::string::npos)
            {
                while (std::getline(std::cin, line) && line.find("Bounds") == std::string::npos)
                {
                    Constraint c;
                    std::regex signPattern("([=<>]+)\\s?(-?\\d+)");
                    std::smatch signMatch;
                    if (std::regex_search(line, signMatch, signPattern))
                    {
                        c.signal = signMatch[1].str();
                        c.coefficients = extractCoefficients(line, numVariables);
                        c.bound = std::stod(signMatch[2].str());
                        instance.constraints.push_back(c);
                    }
                }
            }
            if (line.find("Bounds") != std::string::npos)
            {
                inBounds = true;
            }
            else if (inBounds && line.find("End") != std::string::npos)
            {
                inBounds = false;
            }
            else if (inBounds)
            {
                std::smatch match;

                std::regex boundPattern(R"((-\inf|-\d+|inf|\d+)?\s*(<=|>=|<|>|=)?\s*x(\d+)\s*(<=|>=|<|>|=)?\s*(-\inf|-\d+|inf|\d+)?)");

                if (std::regex_search(line, match, boundPattern))
                {
                    VariableBound bound;
                    bound.lower_bound_sign = (match[2].str() == "") ? "<" : match[2].str();
                    bound.lower_bound = (match[1].str() == "-inf") ? -std::numeric_limits<double>::infinity() : ((match[1].str() == "") ? -std::numeric_limits<double>::infinity() : std::stod(match[1].str()));
                    bound.upper_bound_sign = (match[4].str() == "") ? "<" : match[4].str();
                    bound.upper_bound = (match[5].str() == "inf") ? std::numeric_limits<double>::infinity() : ((match[5].str() == "") ? std::numeric_limits<double>::infinity() : std::stod(match[5].str()));

                    instance.bounds.push_back(bound);
                }
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "Excecao capturada na linha: " << line << std::endl;
            std::cerr << "Mensagem de erro: " << e.what() << std::endl;
        }
    }
    return instance;
}

#endif // INSTANCEREADER_H