//
// Created by alan on 13/07/2019.
//
#ifndef COMPILADOR_TOKEN_H
#define COMPILADOR_TOKEN_H
#include <boost/algorithm/string.hpp>
using namespace std;
class Token {
private:
    string token;
    int id;
    int tipo;
public:
    void setToken(string tokenA) {
        token = tokenA;
    }

    void setId(int idA) {
        id = idA;
    }
    void setTipo(int type){
        tipo=type;
    }
    string getToken() {
        return token;

    }

    int getId() {
        return id;
    }
    int getTipo(){
        return tipo;
    }
};


#endif //COMPILADOR_TOKEN_H
