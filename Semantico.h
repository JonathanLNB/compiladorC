//
// Created by alan on 13/07/2019.
//
#ifndef COMPILADOR_SEMANTICO_H
#define COMPILADOR_SEMANTICO_H
#include "Token.h"
#include <boost/algorithm/string.hpp>
class Semantico {
public:
    bool sameFamily(Token tk1, Token tk2){
        int tipoTk1, tipoTk2;
        int familia1=0,familia2=0;
        tipoTk1=tk1.getTipo();
        tipoTk2=tk2.getTipo();
        familia1=setFamily(tipoTk1);
        familia2=setFamily(tipoTk2);
        if(familia1==familia2)
            return true;
        return false;
    }
    int setFamily(int type){
        int family;
        switch (type){
            case 180:
            case 181:
                family=1;
                break;
            case 182:
                family=2;
                break;
        }
        return family;
    }
    bool isFloat(Token identificador, Token asignado){
        if(sameFamily(identificador,asignado)){
            if(identificador.getTipo()!=181){
                if(identificador.getTipo()!=asignado.getTipo())
                    return true;
                return false;
            }
            return true;
        }
        return false;
    }
};
//para numeros 180 y 181
//para cadenas 182

#endif //COMPILADOR_SEMANTICO_H
