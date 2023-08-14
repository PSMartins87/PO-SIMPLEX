#ifndef GET_SUBMATRIZES_H
#define GET_SUBMATRIZES_H

#include <vector>
#include <string>

std::vector<std::string> getXN(
    std::vector<std::string> X, int corte
){
    std::vector<std::string> XN;
    for (size_t i = 0; i < corte; i++){
        XN.push_back(X[i]);
    }
    return XN;
}
std::vector<std::string> getXB(
    std::vector<std::string> X, int corte
){
    std::vector<std::string> XB;
    for (size_t i = 0; i < X.size(); i++){
        if (i >= corte){
            XB.push_back(X[i]);
        }
    }
    return XB;
}

std::vector<double> getCtn(
    std::vector<double> C, int corte
){
    std::vector<double> Ctn;
    for (size_t i = 0; i < corte; i++){
        Ctn.push_back(C[i]);
    }
    return Ctn;
}
std::vector<double> getCtb(
    std::vector<double> C, int corte
){
    std::vector<double> Ctb;
    for (size_t i = 0; i < C.size(); i++){
        if (i >= corte){
            Ctb.push_back(C[i]);
        }
    }
    return Ctb;
}

std::vector<std::vector<double>> getMatrizN(
    std::vector<std::vector<double>> matrizA, int corte
){
    std::vector<std::vector<double>> matrizN;
    std::vector<double> linha;
    for(size_t i = 0; i < matrizA.size(); i++){
        for(size_t j = 0; j < corte; j++){
            linha.push_back(matrizA[i][j]);
        }
        matrizN.push_back(linha);
    }
    return matrizN;
}
std::vector<std::vector<double>> getMatrizB(
    std::vector<std::vector<double>> matrizA, int corte
){
    std::vector<std::vector<double>> matrizB;
    std::vector<double> linha;
    for(size_t i = 0; i < matrizA.size(); i++){
        for(size_t j = 0; j < matrizA[i].size(); j++){
            if (j >= corte){
                linha.push_back(matrizA[i][j]);
            }

        }
        matrizB.push_back(linha);
    }
    return matrizB;
}

#endif