#include <bits/stdc++.h>
#include <boost/algorithm/string.hpp>

using namespace std;

class Tokens {
private:
    string token;
    int id;

public:
    void setToken(string tokenA) {
        token = tokenA;
    }

    void setId(int idA) {
        id = idA;
    }

    string getToken() {
        return token;

    }

    int getId() {
        return id;
    }
};

int q, q0, x, y, id = 500, error = 0, errorS = 0, cantf;
int valores[] = {180, 181, 182};
int tiposDato[] = {150, 151, 154, 155};
int tipoRetorno[] = {150, 151, 154, 155, 179};
int tiposEncapsulamiento[] = {160, 161, 162};
int comparativos[] = {143, 144, 145, 146, 177, 178};
int aritmeticos[] = {135, 136, 169, 170, 171, 172, 173};
int igualacion[] = {175, 137, 138};
int logicos[] = {147, 148};
int matriz[200][97];
vector<string> alfabeto, entrada, auxV;
vector<Tokens> tokens;
vector<string> errores;
string salida = "", texto = "entero jonas = \"100 y 2000\";\njonas = 200;", aux = "";
int cont, linea = 0;
bool bloque = false, si = false;


string leer(int linea) {
    int count = 0;
    string line;
    ifstream acceso;
    acceso.open("D:/Documentos/Tecno/8vo Semestre/Lenguajes y automatas 2/Compilador/Reglas.txt");
    if (acceso.is_open()) {
        while (getline(acceso, line)) {
            if (count == linea)
                return line;
            count++;
        }
        acceso.close();
    } else cout << "Error al abrir";
}

int esFinal(int finales[]) {
    int salir = 0;
    for (int i = 0; i < cantf; i++) {
        if (finales[i] == q)
            if (finales[i] == 198)
                salir = -1;
            else
                salir = 1;
    }
    return salir;
}

bool esTipoDato(int ID) {
    for (int i = 0; i <= 3; i++) {
        if (tiposDato[i] == ID)
            return true;
    }
    return false;
}

bool esValor(int ID) {
    for (int i = 0; i <= 2; i++) {
        if (valores[i] == ID)
            return true;
    }
    return false;
}

bool esTipoRetorno(int ID) {
    for (int i = 0; i <= 4; i++) {
        if (tipoRetorno[i] == ID)
            return true;
    }
    return false;
}

bool esTipoEncapsulamiento(int ID) {
    for (int i = 0; i <= 2; i++) {
        if (tiposEncapsulamiento[i] == ID)
            return true;
    }
    return false;
}

bool esComparativo(int ID) {
    for (int i = 0; i <= 5; i++) {
        if (comparativos[i] == ID)
            return true;
    }
    return false;
}

bool esAritmetico(int ID) {
    for (int i = 0; i <= 6; i++) {
        if (aritmeticos[i] == ID)
            return true;
    }
    return false;
}

bool esIgualacion(int ID) {
    for (int i = 0; i <= 2; i++) {
        if (igualacion[i] == ID)
            return true;
    }
    return false;
}

bool esLogico(int ID) {
    for (int i = 0; i <= 1; i++) {
        if (logicos[i] == ID)
            return true;
    }
    return false;
}

string encontrarToken(int ID) {
    if (esTipoDato(ID))
        return "Tipo de dato";
    else if (esValor(ID))
        return "Valor (numerico o cadena)";
    else if (esTipoEncapsulamiento(ID))
        return "Metodo de encapsulamiento";
    else if (esTipoRetorno(ID))
        return "Tipo de retorno";
    else if (esLogico(ID))
        return "Operador Logico";
    else if (esAritmetico(ID))
        return "Operador Aritmetico";
    else if (esComparativo(ID))
        return "Operador Comparativo";
    else if (esIgualacion(ID))
        return "Operador de Comparativo";
    else
        return "Variable";
}

int encontrarIndex(char aux) {
    for (int i = 0; i < alfabeto.size(); i++) {
        if (alfabeto[i][0] == aux) {
            return i;
        }
    }
    return -1;
}

void analisisLexico(int finales[]) {
    string palabra = "", palabraAnt = "", erroresS = "";
    int index;
    bool variable = false;
    int estado;
    vector<string> entrada;
    boost::split(entrada, texto, boost::is_any_of("\n"));
    for (int i = 0; i < entrada.size(); i++) {
        q = q0;
        texto = entrada[i];
        for (int j = 0; j <= texto.length(); j++) {
            estado = esFinal(finales);
            if (estado != -1) {
                if (estado == 1 && q != 134) {
                    variable = true;
                    Tokens token;
                    boost::replace_all(palabraAnt, " ", "");
                    boost::replace_all(palabraAnt, ";", "");
                    if (q == 182)
                        token.setToken(palabra);
                    else
                        token.setToken(palabraAnt);
                    if (q == 199) {
                        for (int a = 0; a < tokens.size(); a++) {
                            if (boost::iequals(tokens[a].getToken(), palabraAnt)) {
                                token.setId(tokens[a].getId());
                                variable = false;
                            }
                        }
                        if (variable) {
                            token.setId(id);
                            id++;
                        }
                    } else
                        token.setId(q);
                    tokens.push_back(token);
                    q = q0;
                    palabra = "";
                    palabraAnt = "";
                    j--;
                } else {
                    if (j < texto.length()) {
                        index = encontrarIndex(texto[j]);
                        if (index != -1) {

                            palabraAnt = palabra;
                            palabra += texto.at(j);
                            q = matriz[q][index];
                        } else {
                            erroresS = "Error lexico en la linea: " + to_string((i + 1)) + ":" + to_string(j);
                            errores.push_back(erroresS + "\n");
                            error++;
                            palabra = "";
                            break;
                        }
                    }
                }
            } else {
                erroresS = "Error lexico en la linea: " + to_string((i + 1)) + ":" + to_string(j);
                errores.push_back(erroresS + "\n");
                error++;
                palabra = "";
                break;
            }
        }
    }
}

int main() {
    aux = leer(0);
    boost::split(alfabeto, aux, boost::is_any_of("@"));
    aux = leer(1);
    q0 = stoi(aux);
    aux = leer(3);
    boost::split(auxV, aux, boost::is_any_of(","));
    int finales[auxV.size()];
    cantf = auxV.size();
    for (int i = 0; i < auxV.size(); i++)
        finales[i] = stoi(auxV[i]);
    aux = leer(2);
    boost::split(auxV, aux, boost::is_any_of(" "));
    x = stoi(auxV[0]);
    y = stoi(auxV[1]);
    for (int i = 0; i < x; i++) {
        aux = leer(i + 4);
        for (int j = 0; j < y; j++) {
            boost::split(auxV, aux, boost::is_any_of(","));
            matriz[i][j] = stoi(auxV[j]);
        }
    }
    analisisLexico(finales);

    cout << "ID  |    Token    |  Tipo" << endl;
    cout << "--------------------------" << endl;
    for (int i = 0; i < tokens.size(); i++) {
        cout << tokens[i].getId();
        cout << " | ";
        cout << tokens[i].getToken();
        cout << " | ";
        cout << encontrarToken(tokens[i].getId()) << endl;
    }
    if (errores.size() > 0) {
        for (int i = 0; i < errores.size(); i++) {
            cout << errores[i];
        }
    } else
        cout << "El codigo no tiene errores lexicos\n";
    return 0;
}

