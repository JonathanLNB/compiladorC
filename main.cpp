#include <bits/stdc++.h>
#include <boost/algorithm/string.hpp>
#include "Token.h"
#include "Semantico.h"

//ya quedooooo 

using namespace std;

int q, q0, x, y, id = 500, error = 0, errorS = 0,
        errorSema = 0, cantf;
int valores[] = {180, 181, 182};
int tiposDato[] = {150, 151, 154, 155};
int tipoRetorno[] = {150, 151, 154, 155, 179};
int tiposEncapsulamiento[] = {160, 161, 162};
int comparativos[] = {143, 144, 145, 146, 177, 178};
int aritmeticos[] = {135, 136, 169, 170, 171, 172, 173};
int igualacion[] = {175, 137, 138};
int logicos[] = {147, 148};
int agrupacion[] = {125, 126, 127, 128, 129, 130};
int matriz[201][97];
vector<string> alfabeto, auxV;
vector<Token> tokens;
vector<string> errores;
string salida = "", texto = "", aux = "", erroresS = "";
char ultimoC;
int cont = 0, contS = 0, linea = 0;
bool bloquebool = false, sibool = false, forbool = false;

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

string leerCodigo() {
    string line = "", total = "";
    ifstream acceso;
    acceso.open("D:/Documentos/Tecno/8vo Semestre/Lenguajes y automatas 2/Compilador/Codigo.gsh");
    if (acceso.is_open()) {
        while (getline(acceso, line))
            total += line + "\n";
        acceso.close();
        return total;
    } else cout << "Error al abrir";;
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

bool esAgrupacion(int ID) {
    for (int i = 0; i <= 5; i++) {
        if (agrupacion[i] == ID)
            return true;
    }
    return false;
}

int prioridad(char op) {
    switch (op) {
        case '^':
            return 3;
        case '*':
        case '/':
            return 2;
        case '+':
        case '-':
            return 1;
        case ')':
            return -1;
        default:
            return 0;
    }
}

string convertir(string in) {
    stack<char> pila;
    string posf = "";
    for (int i = 0; i < in.size(); i++) {
        switch (in[i]) {
            case '(':
                pila.push('(');
                break;
            case ')':
                while (!pila.empty() && pila.top() != '(') {
                    posf += string(1, pila.top()) + " ";
                    pila.pop();
                }
                pila.pop();
                break;
            case '+':
            case '-':
            case '*':
            case '/':
            case '^':
                while (!pila.empty() && prioridad(in[i]) <= prioridad(pila.top())) {
                    posf += string(1, pila.top()) + " ";
                    pila.pop();
                }
                pila.push(in[i]);
                break;
            default:
                while (isdigit(in[i]) || in[i] == '.')
                    posf += string(1, in[i++]);
                posf += " ";
                i--;
        }
    }
    while (!pila.empty()) {
        posf += string(1, pila.top()) + " ";
        pila.pop();
    }
    return posf;
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
        return "Operador de Asignacion";
    else if (esAgrupacion(ID))
        return "Operador de agrupacion";
    else if (ID >= 500)
        return "Variable";
    else if (ID == 134)
        return "Delimitador";
    else
        return "Palabra reservada";
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
    cout << texto << endl;
    boost::split(entrada, texto, boost::is_any_of("\n"));
    for (int i = 0; i < entrada.size(); i++) {
        q = q0;
        texto = entrada[i];
        for (int j = 0; j <= texto.length(); j++) {
            estado = esFinal(finales);
            if (estado != -1) {
                if (estado == 1 && q != 134) {
                    variable = true;
                    Token token;
                    boost::replace_all(palabraAnt, " ", "");
                    boost::replace_all(palabraAnt, ";", "");
                    if (q == 182)
                        token.setToken(palabra);
                    else if (esAgrupacion(q)) {
                        boost::replace_all(palabra, " ", "");
                        boost::replace_all(palabra, ";", "");
                        token.setToken(palabra);
                    } else if (q == 131)
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
                    if (ultimoC == ';') {
                        Token token;
                        token.setId(134);
                        token.setToken(";");
                        tokens.push_back(token);
                    }
                    q = q0;
                    palabra = "";
                    palabraAnt = "";
                    j--;
                } else {
                    if (q == 134) {
                        Token token;
                        token.setId(134);
                        token.setToken(";");
                        tokens.push_back(token);
                    } else {
                        if (j < texto.length()) {
                            index = encontrarIndex(texto[j]);
                            if (index != -1) {
                                palabraAnt = palabra;
                                ultimoC = texto.at(j);
                                palabra += ultimoC;
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

void analisisDelTipo();

bool incrementar() {
    if (cont < tokens.size() - 1) {
        cont++;
        return true;
    }
    return false;
}

bool incrementarS() {
    if (contS < tokens.size() - 1) {
        contS++;
        return true;
    }
    return false;
}

void declaracion();

void si();

void para();

void mandarSalida();

void operacion();

void analisisSintactico();

bool consultarFin();

void lectura();

void analisisSemantico();

int obtenerTipoDato(int id);

void revisarPrecedencia();

void analisisDelBloque() {
    linea++;
    if (esTipoDato(tokens[cont].getId())) {
        declaracion();
    }
    if (tokens[cont].getId() == 164) {
        sibool = true;
        si();
    }
    if (tokens[cont].getId() == 163) {
        mandarSalida();
    }

    if (tokens[cont].getId() == 156) {
        lectura();
    }
    if (tokens[cont].getId() >= 500) {
        operacion();
    }
    if (tokens[cont].getId() == 128) {
        bloquebool = false;
        if (incrementar())
            analisisSintactico();
        else {
            errorS++;
            erroresS = "Error de sintaxis en la linea: " +
                       to_string(linea);
            errores.push_back(erroresS + "\n");
            return;
        }
    }
}

void analisisDelFor() {
    linea++;
    if (esTipoDato(tokens[cont].getId())) {
        declaracion();
    }
    if (tokens[cont].getId() == 164) {
        sibool = true;
        si();
    }
    if (tokens[cont].getId() == 157) {
        forbool = true;
        para();
    }
    if (tokens[cont].getId() == 163) {
        mandarSalida();
    }

    if (tokens[cont].getId() == 156) {
        lectura();
    }
    if (tokens[cont].getId() >= 500) {
        operacion();
    }
    if (tokens[cont].getId() == 128) {
        forbool = false;
        if (incrementar())
            analisisSintactico();
        else {
            errorS++;
            erroresS = "Error de sintaxis en la linea: " +
                       to_string(linea);
            errores.push_back(erroresS + "\n");
            return;
        }
    }
}

void lectura() {
    if (incrementar()) {
        if (tokens[cont].getId() == 125) {
            do {
                if (incrementar()) {
                    if (tokens[cont].getId() >= 500) {
                        if (!incrementar()) {
                            errorS++;
                            erroresS = "Error de sintaxis en la linea: " + to_string(linea);
                            errores.push_back(erroresS + "\n");
                            return;
                        }
                    } else {
                        errorS++;
                        erroresS = "Error de sintaxis en la linea: " + to_string(linea);
                        errores.push_back(erroresS + "\n");
                        return;
                    }
                } else {
                    errorS++;
                    erroresS = "Error de sintaxis en la linea: " + to_string(linea);
                    errores.push_back(erroresS + "\n");
                    return;
                }
            } while (tokens[cont].getId() == 131);
            if (tokens[cont].getId() == 130) {
                if (incrementar()) {
                    if (tokens[cont].getId() == 134) {
                        if (!incrementar()) {
                            errorS++;
                            erroresS = "Error de sintaxis en la linea: " + to_string(linea);
                            errores.push_back(erroresS + "\n");
                            return;
                        }
                    } else {
                        errorS++;
                        erroresS = "Error de sintaxis en la linea: " + to_string(linea);
                        errores.push_back(erroresS + "\n");
                        return;
                    }
                } else {
                    errorS++;
                    erroresS = "Error de sintaxis en la linea: " + to_string(linea);
                    errores.push_back(erroresS + "\n");
                    return;
                }
            } else {
                errorS++;
                erroresS = "Error de sintaxis en la linea: " + to_string(linea);
                errores.push_back(erroresS + "\n");
                return;
            }
        } else {
            errorS++;
            erroresS = "Error de sintaxis en la linea: " + to_string(linea);
            errores.push_back(erroresS + "\n");
            return;
        }
    } else {
        errorS++;
        erroresS = "Error de sintaxis en la linea: " + to_string(linea);
        errores.push_back(erroresS + "\n");
        return;
    }
}

void analisisDelSi() {
    linea++;
    if (esTipoDato(tokens[cont].getId())) {
        declaracion();
    }
    if (tokens[cont].getId() == 156) {
        lectura();
    }
    if (tokens[cont].getId() == 163) {
        mandarSalida();
    }
    if (tokens[cont].getId() >= 500) {
        operacion();
    }
    if (tokens[cont].getId() == 128) {
        sibool = false;
        if (incrementar()) {
            if (bloquebool)
                analisisDelBloque();
            else
                analisisSintactico();
        }
    } else {
        linea++;
        if (sibool) {
            errorS++;
            erroresS = "Error de sintaxis en la linea: " + to_string(linea);
            errores.push_back(erroresS + "\n");
            return;
        }
    }

}

void declaracion() {
    if (incrementar()) {
        if (tokens[cont].getId() >= 500) {
            if (incrementar()) {
                if (tokens[cont].getId() == 175) {
                    if (incrementar()) {
                        if (esValor(tokens[cont].getId())) {
                            if (incrementar()) {
                                if (tokens[cont].getId() == 134) {
                                    if (consultarFin()) {
                                        analisisSintactico();
                                    }
                                    return;
                                } else {
                                    if (tokens[cont].getId() == 131)
                                        declaracion();
                                    else {
                                        if (esAritmetico(tokens[cont].getId())) {
                                            do {
                                                if (incrementar()) {
                                                    if (tokens[cont].getId() >= 500 || esValor(tokens[cont].getId())) {
                                                        if (incrementar()) {
                                                            if (!esAritmetico(tokens[cont].getId())) {
                                                                if (tokens[cont].getId() == 134) {
                                                                    if (incrementar())
                                                                        analisisSintactico();
                                                                    return;
                                                                } else {
                                                                    errorS++;
                                                                    erroresS = "Error de sintaxis en la linea: " +
                                                                               to_string(linea);
                                                                    errores.push_back(erroresS + "\n");
                                                                    return;
                                                                }
                                                            }
                                                        } else {
                                                            errorS++;
                                                            erroresS = "Error de sintaxis en la linea: " +
                                                                       to_string(linea);
                                                            errores.push_back(erroresS + "\n");
                                                            return;
                                                        }
                                                    } else {
                                                        errorS++;
                                                        erroresS = "Error de sintaxis en la linea: " + to_string(linea);
                                                        errores.push_back(erroresS + "\n");
                                                        return;
                                                    }
                                                } else {
                                                    errorS++;
                                                    erroresS = "Error de sintaxis en la linea: " + to_string(linea);
                                                    errores.push_back(erroresS + "\n");
                                                    return;
                                                }
                                            } while (tokens[cont].getId() != 134 && tokens[cont].getId() != 131);
                                            if (tokens[cont].getId() == 134) {
                                                if (consultarFin()) return;
                                            } else {
                                                if (tokens[cont].getId() == 131)
                                                    declaracion();
                                                else {
                                                    errorS++;
                                                    erroresS = "Error de sintaxis en la linea: " + to_string(linea);
                                                    errores.push_back(erroresS + "\n");
                                                    return;
                                                }
                                            }
                                        } else {
                                            errorS++;
                                            erroresS = "Error de sintaxis en la linea: " + to_string(linea);
                                            errores.push_back(erroresS + "\n");
                                            return;
                                        }
                                    }
                                }
                            } else {
                                errorS++;
                                erroresS = "Error de sintaxis en la linea: " + to_string(linea);
                                errores.push_back(erroresS + "\n");
                                return;
                            }
                        } else {
                            if (tokens[cont].getId() >= 500) {
                                for (int a = 0; a < tokens.size(); a++) {
                                    if (tokens[a].getId() == tokens[cont].getId()) {
                                        if (incrementar()) {
                                            if (tokens[cont].getId() == 134) {
                                                if (consultarFin()) return;
                                            } else {
                                                errorS++;
                                                erroresS = "Error de sintaxis en la linea: " + to_string(linea);
                                                errores.push_back(erroresS + "\n");
                                                return;
                                            }
                                        } else {
                                            errorS++;
                                            erroresS = "Error de sintaxis en la linea: " + to_string(linea);
                                            errores.push_back(erroresS + "\n");
                                            return;
                                        }
                                    }
                                }
                            } else {
                                errorS++;
                                erroresS = "Error de sintaxis en la linea: " + to_string(linea);
                                errores.push_back(erroresS + "\n");
                                return;
                            }
                        }
                    }
                } else {
                    if (tokens[cont].getId() == 131) {
                        declaracion();
                    }
                    if (tokens[cont].getId() == 134 || tokens[cont].getId() == 128) {
                        if (incrementar()) {
                            if (bloquebool)
                                if (sibool)
                                    analisisDelSi();
                                else
                                    analisisDelBloque();
                            else if (sibool)
                                analisisDelSi();
                            else
                                analisisSintactico();
                        }
                        return;
                    } else {
                        errorS++;
                        erroresS = "Error de sintaxis en la linea: " + to_string(linea);
                        errores.push_back(erroresS + "\n");
                        return;
                    }
                }
            } else {
                errorS++;
                erroresS = "Error de sintaxis en la linea: " + to_string(linea);
                errores.push_back(erroresS + "\n");
            }
        } else {
            if (esTipoDato(tokens[cont].getId())) {
                if (incrementar()) {
                    if (tokens[cont].getId() == 175) {
                        if (incrementar()) {
                            if (esValor(tokens[cont].getId())) {
                                if (consultarFin()) return;
                            } else {
                                for (int a = 0; a < tokens.size(); a++) {
                                    if (tokens[a].getId() == tokens[cont].getId()) {
                                        if (consultarFin()) return;
                                    }
                                }
                            }
                        }
                    } else {
                        if (tokens[cont].getId() == 131) {
                            declaracion();
                        }
                        if (tokens[cont].getId() == 134 || tokens[cont].getId() == 128) {
                            if (incrementar()) {
                                if (bloquebool)
                                    if (sibool)
                                        analisisDelSi();
                                    else
                                        analisisDelBloque();
                                else if (sibool)
                                    analisisDelSi();
                                else
                                    analisisSintactico();
                            }
                            return;
                        } else {
                            errorS++;
                            erroresS = "Error de sintaxis en la linea: " + to_string(linea);
                            errores.push_back(erroresS + "\n");
                            return;
                        }
                    }
                } else {
                    errorS++;
                    erroresS = "Error de sintaxis en la linea: " + to_string(linea);
                    errores.push_back(erroresS + "\n");
                }
            } else {
                errorS++;
                erroresS = "Error de sintaxis en la linea: " + to_string(linea);
                errores.push_back(erroresS + "\n");
            }
        }
    } else {
        errorS++;
        erroresS = "Error de sintaxis en la linea: " + to_string(linea);
        errores.push_back(erroresS + "\n");
    }
}

void bloque() {
    if (incrementar()) {
        if (esTipoRetorno(tokens[cont].getId())) {
            if (incrementar()) {
                if (tokens[cont].getId() >= 500) {
                    if (incrementar()) {
                        if (tokens[cont].getId() == 125) {
                            do {
                                if (incrementar()) {
                                    if (esTipoDato(tokens[cont].getId())) {
                                        if (incrementar()) {
                                            if (tokens[cont].getId() < 500) {
                                                errorS++;
                                                erroresS = "Error de sintaxis en la linea: " + to_string(linea);
                                                errores.push_back(erroresS + "\n");
                                                return;
                                            }
                                            if (!incrementar()) {
                                                errorS++;
                                                erroresS = "Error de sintaxis en la linea: " + to_string(linea);
                                                errores.push_back(erroresS + "\n");
                                                return;
                                            }
                                        }
                                    }
                                }
                            } while (tokens[cont].getId() == 131);
                            if (tokens[cont].getId() == 130) {
                                if (incrementar()) {
                                    if (tokens[cont].getId() == 127) {
                                        if (incrementar()) {
                                            analisisDelBloque();
                                        }
                                    } else {
                                        errorS++;
                                        erroresS = "Error de sintaxis en la linea: " + to_string(linea);
                                        errores.push_back(erroresS + "\n");
                                        return;
                                    }
                                } else {
                                    errorS++;
                                    erroresS = "Error de sintaxis en la linea: " + to_string(linea);
                                    errores.push_back(erroresS + "\n");
                                    return;
                                }
                            } else {
                                errorS++;
                                erroresS = "Error de sintaxis en la linea: " + to_string(linea);
                                errores.push_back(erroresS + "\n");
                                return;
                            }
                        }
                    }
                }
            }
        }
    }
}

void operacion() {
    if (incrementar()) {
        if (esIgualacion(tokens[cont].getId())) {
            if (tokens[cont].getId() == 175) {
                do {
                    if (incrementar()) {
                        if (tokens[cont].getId() >= 500 || esValor(tokens[cont].getId())) {
                            if (incrementar()) {
                                if (!esAritmetico(tokens[cont].getId())) {
                                    if (tokens[cont].getId() == 134) {
                                        if (incrementar())
                                            analisisSintactico();
                                        return;
                                    } else {
                                        errorS++;
                                        erroresS = "Error de sintaxis en la linea: " + to_string(linea);
                                        errores.push_back(erroresS + "\n");
                                        return;
                                    }
                                }
                            } else {
                                errorS++;
                                erroresS = "Error de sintaxis en la linea: " + to_string(linea);
                                errores.push_back(erroresS + "\n");
                                return;
                            }
                        } else {
                            errorS++;
                            erroresS = "Error de sintaxis en la linea: " + to_string(linea);
                            errores.push_back(erroresS + "\n");
                            return;
                        }
                    } else {
                        errorS++;
                        erroresS = "Error de sintaxis en la linea: " + to_string(linea);
                        errores.push_back(erroresS + "\n");
                        return;
                    }
                } while (tokens[cont].getId() != 134);
            }
            if (tokens[cont].getId() == 137 || tokens[cont].getId() == 138) {
                if (incrementar()) {
                    if (!(tokens[cont].getId() >= 500 || esValor(tokens[cont].getId()))) {
                        errorS++;
                        erroresS = "Error de sintaxis en la linea: " + to_string(linea);
                        errores.push_back(erroresS + "\n");
                        return;
                    }
                }
            }
        }
    }
}

void mandarSalida() {
    if (incrementar()) {
        if (tokens[cont].getId() == 125) {
            do {
                if (incrementar()) {
                    if (tokens[cont].getId() == 181 || tokens[cont].getId() == 182 || tokens[cont].getId() >= 500) {
                        if (!incrementar()) {
                            errorS++;
                            erroresS = "Error de sintaxis en la linea: " + to_string(linea);
                            errores.push_back(erroresS + "\n");
                            return;
                        }
                    } else {
                        errorS++;
                        erroresS = "Error de sintaxis en la linea: " + to_string(linea);
                        errores.push_back(erroresS + "\n");
                        return;
                    }
                } else {
                    errorS++;
                    erroresS = "Error de sintaxis en la linea: " + to_string(linea);
                    errores.push_back(erroresS + "\n");
                    return;
                }
            } while (tokens[cont].getId() == 169);
            if (tokens[cont].getId() == 130) {
                if (incrementar()) {
                    if (tokens[cont].getId() == 134) {
                        if (!incrementar()) {
                            errorS++;
                            erroresS = "Error de sintaxis en la linea: " + to_string(linea);
                            errores.push_back(erroresS + "\n");
                            return;
                        }
                    } else {
                        errorS++;
                        erroresS = "Error de sintaxis en la linea: " + to_string(linea);
                        errores.push_back(erroresS + "\n");
                        return;
                    }
                } else {
                    errorS++;
                    erroresS = "Error de sintaxis en la linea: " + to_string(linea);
                    errores.push_back(erroresS + "\n");
                    return;
                }
            } else {
                errorS++;
                erroresS = "Error de sintaxis en la linea: " + to_string(linea);
                errores.push_back(erroresS + "\n");
                return;
            }
        } else {
            errorS++;
            erroresS = "Error de sintaxis en la linea: " + to_string(linea);
            errores.push_back(erroresS + "\n");
            return;
        }
    } else {
        errorS++;
        erroresS = "Error de sintaxis en la linea: " + to_string(linea);
        errores.push_back(erroresS + "\n");
        return;
    }
}

void si() {
    if (incrementar()) {
        if (tokens[cont].getId() == 125) {
            do {
                if (incrementar()) {
                    if (esValor(tokens[cont].getId()) || tokens[cont].getId() >= 500) {
                        if (incrementar()) {
                            if (esComparativo(tokens[cont].getId()) && tokens[cont].getId() != 175) {
                                if (incrementar()) {
                                    if (esValor(tokens[cont].getId()) || tokens[cont].getId() >= 500) {
                                        if (!incrementar()) {
                                            errorS++;
                                            erroresS = "Error de sintaxis en la linea: " + to_string(linea);
                                            errores.push_back(erroresS + "\n");
                                            return;
                                        }
                                    } else {
                                        errorS++;
                                        erroresS = "Error de sintaxis en la linea: " + to_string(linea);
                                        errores.push_back(erroresS + "\n");
                                        return;
                                    }
                                } else {
                                    errorS++;
                                    erroresS = "Error de sintaxis en la linea: " + to_string(linea);
                                    errores.push_back(erroresS + "\n");
                                    return;
                                }
                            } else {
                                errorS++;
                                erroresS = "Error de sintaxis en la linea: " + to_string(linea);
                                errores.push_back(erroresS + "\n");
                                return;
                            }
                        } else {
                            errorS++;
                            erroresS = "Error de sintaxis en la linea: " + to_string(linea);
                            errores.push_back(erroresS + "\n");
                            return;
                        }
                    } else {
                        errorS++;
                        erroresS = "Error de sintaxis en la linea: " + to_string(linea);
                        errores.push_back(erroresS + "\n");
                        return;
                    }
                } else {
                    errorS++;
                    erroresS = "Error de sintaxis en la linea: " + to_string(linea);
                    errores.push_back(erroresS + "\n");
                    return;
                }
            } while (esLogico(tokens[cont].getId()));
            if (tokens[cont].getId() == 130) {
                if (incrementar()) {
                    if (tokens[cont].getId() == 127) {
                        if (incrementar())
                            analisisDelSi();
                        else {
                            errorS++;
                            erroresS = "Error de sintaxis en la linea: " + to_string(linea);
                            errores.push_back(erroresS + "\n");
                            return;
                        }
                    } else {
                        errorS++;
                        erroresS = "Error de sintaxis en la linea: " + to_string(linea);
                        errores.push_back(erroresS + "\n");
                        return;
                    }
                } else {
                    errorS++;
                    erroresS = "Error de sintaxis en la linea: " + to_string(linea);
                    errores.push_back(erroresS + "\n");
                    return;
                }
            } else {
                errorS++;
                erroresS = "Error de sintaxis en la linea: " + to_string(linea);
                errores.push_back(erroresS + "\n");
                return;
            }
        } else {
            errorS++;
            erroresS = "Error de sintaxis en la linea: " + to_string(linea);
            errores.push_back(erroresS + "\n");
            return;
        }
    } else {
        errorS++;
        erroresS = "Error de sintaxis en la linea: " + to_string(linea);
        errores.push_back(erroresS + "\n");
        return;
    }
}

void para() {
    if (incrementar()) {
        if (tokens[cont].getId() == 125) {
            if (incrementar()) {
                if (tokens[cont].getId() >= 500) {
                    if (incrementar()) {
                        if (tokens[cont].getId() == 175) {
                            if (incrementar()) {
                                if (esValor(tokens[cont].getId()) || tokens[cont].getId() >= 500) {
                                    if (incrementar()) {
                                        if (tokens[cont].getId() == 131) {
                                            do {
                                                if (incrementar()) {
                                                    if (esValor(tokens[cont].getId()) || tokens[cont].getId() >= 500) {
                                                        if (incrementar()) {
                                                            if (esComparativo(tokens[cont].getId())) {
                                                                if (incrementar()) {
                                                                    if (esValor(tokens[cont].getId()) ||
                                                                        tokens[cont].getId() >= 500) {
                                                                        if (!incrementar()) {
                                                                            errorS++;
                                                                            erroresS =
                                                                                    "Error de sintaxis en la linea: " +
                                                                                    to_string(linea);
                                                                            errores.push_back(erroresS + "\n");
                                                                            return;
                                                                        }
                                                                    } else {
                                                                        errorS++;
                                                                        erroresS = "Error de sintaxis en la linea: " +
                                                                                   to_string(linea);
                                                                        errores.push_back(erroresS + "\n");
                                                                        return;
                                                                    }
                                                                } else {
                                                                    errorS++;
                                                                    erroresS = "Error de sintaxis en la linea: " +
                                                                               to_string(linea);
                                                                    errores.push_back(erroresS + "\n");
                                                                    return;
                                                                }
                                                            } else {
                                                                errorS++;
                                                                erroresS = "Error de sintaxis en la linea: " +
                                                                           to_string(linea);
                                                                errores.push_back(erroresS + "\n");
                                                                return;
                                                            }
                                                        } else {
                                                            errorS++;
                                                            erroresS = "Error de sintaxis en la linea: " +
                                                                       to_string(linea);
                                                            errores.push_back(erroresS + "\n");
                                                            return;
                                                        }
                                                    } else {
                                                        errorS++;
                                                        erroresS = "Error de sintaxis en la linea: " + to_string(linea);
                                                        errores.push_back(erroresS + "\n");
                                                        return;
                                                    }
                                                } else {
                                                    errorS++;
                                                    erroresS = "Error de sintaxis en la linea: " + to_string(linea);
                                                    errores.push_back(erroresS + "\n");
                                                    return;
                                                }
                                            } while (esLogico(tokens[cont].getId()));
                                            if (tokens[cont].getId() == 131) {
                                                if (incrementar()) {
                                                    if (tokens[cont].getId() >= 500) {
                                                        if (incrementar()) {
                                                            if (tokens[cont].getId() == 135) {
                                                                if (incrementar()) {
                                                                    if (tokens[cont].getId() == 130) {
                                                                        if (incrementar()) {
                                                                            if (tokens[cont].getId() == 127) {
                                                                                if (incrementar())
                                                                                    analisisDelFor();
                                                                                else {
                                                                                    errorS++;
                                                                                    erroresS =
                                                                                            "Error de sintaxis en la linea: " +
                                                                                            to_string(linea);
                                                                                    errores.push_back(erroresS + "\n");
                                                                                    return;
                                                                                }
                                                                            } else {
                                                                                errorS++;
                                                                                erroresS =
                                                                                        "Error de sintaxis en la linea: " +
                                                                                        to_string(linea);
                                                                                errores.push_back(erroresS + "\n");
                                                                                return;
                                                                            }
                                                                        } else {
                                                                            errorS++;
                                                                            erroresS =
                                                                                    "Error de sintaxis en la linea: " +
                                                                                    to_string(linea);
                                                                            errores.push_back(erroresS + "\n");
                                                                            return;
                                                                        }
                                                                    } else {
                                                                        errorS++;
                                                                        erroresS = "Error de sintaxis en la linea: " +
                                                                                   to_string(linea);
                                                                        errores.push_back(erroresS + "\n");
                                                                        return;
                                                                    }
                                                                } else {
                                                                    errorS++;
                                                                    erroresS = "Error de sintaxis en la linea: " +
                                                                               to_string(linea);
                                                                    errores.push_back(erroresS + "\n");
                                                                    return;
                                                                }
                                                            } else {
                                                                errorS++;
                                                                erroresS = "Error de sintaxis en la linea: " +
                                                                           to_string(linea);
                                                                errores.push_back(erroresS + "\n");
                                                                return;
                                                            }
                                                        } else {
                                                            errorS++;
                                                            erroresS = "Error de sintaxis en la linea: " +
                                                                       to_string(linea);
                                                            errores.push_back(erroresS + "\n");
                                                            return;
                                                        }
                                                    } else {
                                                        errorS++;
                                                        erroresS = "Error de sintaxis en la linea: " + to_string(linea);
                                                        errores.push_back(erroresS + "\n");
                                                        return;
                                                    }
                                                } else {
                                                    errorS++;
                                                    erroresS = "Error de sintaxis en la linea: " + to_string(linea);
                                                    errores.push_back(erroresS + "\n");
                                                    return;
                                                }
                                            } else {
                                                errorS++;
                                                erroresS = "Error de sintaxis en la linea: " + to_string(linea);
                                                errores.push_back(erroresS + "\n");
                                                return;
                                            }
                                        } else {
                                            errorS++;
                                            erroresS = "Error de sintaxis en la linea: " + to_string(linea);
                                            errores.push_back(erroresS + "\n");
                                            return;
                                        }
                                    } else
                                        errorS++;
                                    erroresS = "Error de sintaxis en la linea: " + to_string(linea);
                                    errores.push_back(erroresS + "\n");
                                    return;
                                }
                            } else {
                                errorS++;
                                erroresS = "Error de sintaxis en la linea: " + to_string(linea);
                                errores.push_back(erroresS + "\n");
                                return;
                            }
                        } else {
                            errorS++;
                            erroresS = "Error de sintaxis en la linea: " + to_string(linea);
                            errores.push_back(erroresS + "\n");
                            return;
                        }
                    } else {
                        errorS++;
                        erroresS = "Error de sintaxis en la linea: " + to_string(linea);
                        errores.push_back(erroresS + "\n");
                        return;
                    }
                } else {
                    if (esTipoDato(tokens[cont].getId())) {
                        if (incrementar()) {
                            if (tokens[cont].getId() >= 500) {
                                if (incrementar()) {
                                    if (tokens[cont].getId() == 175) {
                                        if (incrementar()) {
                                            if (esValor(tokens[cont].getId()) || tokens[cont].getId() >= 500) {
                                                if (incrementar()) {
                                                    if (tokens[cont].getId() == 131) {
                                                        do {
                                                            if (incrementar()) {
                                                                if (esValor(tokens[cont].getId()) ||
                                                                    tokens[cont].getId() >= 500) {
                                                                    if (incrementar()) {
                                                                        if (esComparativo(tokens[cont].getId())) {
                                                                            if (incrementar()) {
                                                                                if (esValor(tokens[cont].getId()) ||
                                                                                    tokens[cont].getId() >= 500) {
                                                                                    if (!incrementar()) {
                                                                                        errorS++;
                                                                                        erroresS =
                                                                                                "Error de sintaxis en la linea: " +
                                                                                                to_string(linea);
                                                                                        errores.push_back(
                                                                                                erroresS + "\n");
                                                                                        return;
                                                                                    }
                                                                                } else {
                                                                                    errorS++;
                                                                                    erroresS =
                                                                                            "Error de sintaxis en la linea: " +
                                                                                            to_string(linea);
                                                                                    errores.push_back(erroresS + "\n");
                                                                                    return;
                                                                                }
                                                                            } else {
                                                                                errorS++;
                                                                                erroresS =
                                                                                        "Error de sintaxis en la linea: " +
                                                                                        to_string(linea);
                                                                                errores.push_back(erroresS + "\n");
                                                                                return;
                                                                            }
                                                                        } else {
                                                                            errorS++;
                                                                            erroresS =
                                                                                    "Error de sintaxis en la linea: " +
                                                                                    to_string(linea);
                                                                            errores.push_back(erroresS + "\n");
                                                                            return;
                                                                        }
                                                                    } else {
                                                                        errorS++;
                                                                        erroresS = "Error de sintaxis en la linea: " +
                                                                                   to_string(linea);
                                                                        errores.push_back(erroresS + "\n");
                                                                        return;
                                                                    }
                                                                } else {
                                                                    errorS++;
                                                                    erroresS = "Error de sintaxis en la linea: " +
                                                                               to_string(linea);
                                                                    errores.push_back(erroresS + "\n");
                                                                    return;
                                                                }
                                                            } else {
                                                                errorS++;
                                                                erroresS = "Error de sintaxis en la linea: " +
                                                                           to_string(linea);
                                                                errores.push_back(erroresS + "\n");
                                                                return;
                                                            }
                                                        } while (esLogico(tokens[cont].getId()));
                                                        if (tokens[cont].getId() == 131) {
                                                            if (incrementar()) {
                                                                if (tokens[cont].getId() >= 500) {
                                                                    if (incrementar()) {
                                                                        if (tokens[cont].getId() == 135) {
                                                                            if (incrementar()) {
                                                                                if (tokens[cont].getId() == 130) {
                                                                                    if (incrementar()) {
                                                                                        if (tokens[cont].getId() ==
                                                                                            127) {
                                                                                            if (incrementar())
                                                                                                analisisDelFor();
                                                                                            else {
                                                                                                errorS++;
                                                                                                erroresS =
                                                                                                        "Error de sintaxis en la linea: " +
                                                                                                        to_string(
                                                                                                                linea);
                                                                                                errores.push_back(
                                                                                                        erroresS +
                                                                                                        "\n");
                                                                                                return;
                                                                                            }
                                                                                        } else {
                                                                                            errorS++;
                                                                                            erroresS =
                                                                                                    "Error de sintaxis en la linea: " +
                                                                                                    to_string(linea);
                                                                                            errores.push_back(
                                                                                                    erroresS + "\n");
                                                                                            return;
                                                                                        }
                                                                                    } else {
                                                                                        errorS++;
                                                                                        erroresS =
                                                                                                "Error de sintaxis en la linea: " +
                                                                                                to_string(linea);
                                                                                        errores.push_back(
                                                                                                erroresS + "\n");
                                                                                        return;
                                                                                    }
                                                                                } else {
                                                                                    errorS++;
                                                                                    erroresS =
                                                                                            "Error de sintaxis en la linea: " +
                                                                                            to_string(linea);
                                                                                    errores.push_back(erroresS + "\n");
                                                                                    return;
                                                                                }
                                                                            } else {
                                                                                errorS++;
                                                                                erroresS =
                                                                                        "Error de sintaxis en la linea: " +
                                                                                        to_string(linea);
                                                                                errores.push_back(erroresS + "\n");
                                                                                return;
                                                                            }
                                                                        } else {
                                                                            errorS++;
                                                                            erroresS =
                                                                                    "Error de sintaxis en la linea: " +
                                                                                    to_string(linea);
                                                                            errores.push_back(erroresS + "\n");
                                                                            return;
                                                                        }
                                                                    } else {
                                                                        errorS++;
                                                                        erroresS = "Error de sintaxis en la linea: " +
                                                                                   to_string(linea);
                                                                        errores.push_back(erroresS + "\n");
                                                                        return;
                                                                    }
                                                                } else {
                                                                    errorS++;
                                                                    erroresS = "Error de sintaxis en la linea: " +
                                                                               to_string(linea);
                                                                    errores.push_back(erroresS + "\n");
                                                                    return;
                                                                }
                                                            } else {
                                                                errorS++;
                                                                erroresS = "Error de sintaxis en la linea: " +
                                                                           to_string(linea);
                                                                errores.push_back(erroresS + "\n");
                                                                return;
                                                            }
                                                        } else {
                                                            errorS++;
                                                            erroresS = "Error de sintaxis en la linea: " +
                                                                       to_string(linea);
                                                            errores.push_back(erroresS + "\n");
                                                            return;
                                                        }
                                                    } else {
                                                        errorS++;
                                                        erroresS = "Error de sintaxis en la linea: " + to_string(linea);
                                                        errores.push_back(erroresS + "\n");
                                                        return;
                                                    }
                                                } else {
                                                    errorS++;
                                                    erroresS = "Error de sintaxis en la linea: " + to_string(linea);
                                                    errores.push_back(erroresS + "\n");
                                                    return;
                                                }
                                            }
                                        } else {
                                            errorS++;
                                            erroresS = "Error de sintaxis en la linea: " + to_string(linea);
                                            errores.push_back(erroresS + "\n");
                                            return;
                                        }
                                    } else {
                                        errorS++;
                                        erroresS = "Error de sintaxis en la linea: " + to_string(linea);
                                        errores.push_back(erroresS + "\n");
                                        return;
                                    }
                                } else {
                                    errorS++;
                                    erroresS = "Error de sintaxis en la linea: " + to_string(linea);
                                    errores.push_back(erroresS + "\n");
                                    return;
                                }
                            } else {
                                errorS++;
                                erroresS = "Error de sintaxis en la linea: " + to_string(linea);
                                errores.push_back(erroresS + "\n");
                                return;
                            }
                        } else {
                            errorS++;
                            erroresS = "Error de sintaxis en la linea: " + to_string(linea);
                            errores.push_back(erroresS + "\n");
                            return;
                        }
                    } else {
                        errorS++;
                        erroresS = "Error de sintaxis en la linea: " + to_string(linea);
                        errores.push_back(erroresS + "\n");
                        return;
                    }
                }
            } else {
                errorS++;
                erroresS = "Error de sintaxis en la linea: " + to_string(linea);
                errores.push_back(erroresS + "\n");
                return;
            }
        } else {
            errorS++;
            erroresS = "Error de sintaxis en la linea: " + to_string(linea);
            errores.push_back(erroresS + "\n");
            return;
        }

    } else {
        errorS++;
        erroresS = "Error de sintaxis en la linea: " + to_string(linea);
        errores.push_back(erroresS + "\n");
        return;
    }
}

void analisisSintactico() {
    linea++;
    if (esTipoEncapsulamiento(tokens[cont].getId())) {
        bloquebool = true;
        bloque();
    }
    if (esTipoDato(tokens[cont].getId())) {
        declaracion();
        linea++;
    }
    if (tokens[cont].getId() == 157) {
        forbool = true;
        para();
    }
    if (tokens[cont].getId() == 164) {
        sibool = true;
        si();
    }
    if (tokens[cont].getId() == 163) {
        mandarSalida();
    }
    if (tokens[cont].getId() == 156) {
        lectura();
    }
    if (tokens[cont].getId() >= 500) {
        operacion();
    }
}

bool consultarFin() {
    if (incrementar()) {
        if (tokens[cont].getId() == 131) {
            declaracion();
        }
        if (tokens[cont].getId() == 134) {
            if (incrementar()) {
                if (bloquebool) {
                    if (sibool)
                        analisisDelSi();
                    else
                        analisisDelBloque();
                } else {
                    if (sibool)
                        analisisDelSi();
                    else
                        analisisSintactico();
                }
            }
        }
        return true;
    }
    return false;
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
    texto = leerCodigo();
    analisisLexico(finales);
    analisisDelTipo();
    cout << "ID  |    Token    |  Tipo" << endl;
    cout << "--------------------------" << endl;
    for (int i = 0; i < tokens.size(); i++) {
        cout << tokens[i].getId();
        cout << " | ";
        cout << tokens[i].getToken();
        cout << " | ";
        cout << tokens[i].getTipo();
        cout << " | ";
        cout << encontrarToken(tokens[i].getId()) << endl;
    }
    cont = 0;
    analisisSintactico();
    if (errores.size() > 0) {
        for (int i = 0; i < errores.size(); i++) {
            cout << errores[i];
        }
    } else {
        analisisSemantico();
        if (errores.size() > 0) {
            for (int i = 0; i < errores.size(); i++) {
                cout << errores[i];
            }
        } else {
            cout << "El codigo no tiene errores lexicos, ni sintacticos, ni semanticos\n";
        }
    }
    return 0;
}

void analisisDelTipo() {
    int tipo = -1;
    do {
        if (tokens[contS].getId() >= 500) {
            if (tokens[contS].getTipo() == 0) {
                if (tipo != -1)
                    tokens[contS].setTipo(tipo);
                else {
                    for (int a = 0; a < tokens.size(); a++) {
                        if (boost::iequals(tokens[a].getToken(), tokens[contS].getToken())) {
                            tokens[contS].setTipo(tokens[a].getTipo());
                            break;
                        }
                    }
                }
            }
        }
        if (tokens[contS].getId() == 134 || tokens[contS].getId() == 127)
            tipo = -1;
        if (esTipoDato(tokens[contS].getId()))
            tipo = obtenerTipoDato(tokens[contS].getId());
        if (esValor(tokens[contS].getId()))
            tokens[contS].setTipo(tokens[contS].getId());
    } while (incrementarS());
}

void analisisSemantico() {
    contS = 0;
    Semantico semantico;
    do {
        if (tokens[contS].getId() == 163) {
            incrementarS();
            incrementarS();
            if (tokens[contS].getTipo() != 182) {
                "Error Semantico: Datatype not match " +
                semantico.getDataType(tokens[contS - 1].getTipo()) +
                ":" + semantico.getDataType(182);
                errores.push_back(erroresS + "\n");
            }
        }
        if (esComparativo(tokens[contS].getId())) {
            if (tokens[contS - 1].getTipo() == 0 && tokens[contS - 1].getId() >= 500) {
                errorSema++;
                erroresS = "Error Semantico: Unexpected " + tokens[contS - 1].getToken();
                errores.push_back(erroresS + "\n");
            }
            if (tokens[contS + 1].getTipo() == 0 && tokens[contS + 1].getId() >= 500) {
                errorSema++;
                erroresS = "Error Semantico: Unexpected " + tokens[contS - 1].getToken();
                errores.push_back(erroresS + "\n");
            }
            if (semantico.sameFamily(tokens[contS - 1], tokens[contS + 1])) {
                incrementarS();
            } else {
                errorSema++;
                erroresS =
                        "Error Semantico: Datatype not match " +
                        semantico.getDataType(tokens[contS - 1].getTipo()) +
                        ":" + semantico.getDataType(tokens[contS + 1].getTipo());
                errores.push_back(erroresS + "\n");
            }
        }
        if (tokens[contS].getId() == 175) {
            if (tokens[contS + 1].getTipo() == 0 && tokens[contS + 1].getId() >= 500) {
                errorSema++;
                erroresS = "Error Semantico: Unexpected " + tokens[contS - 1].getToken();
                errores.push_back(erroresS + "\n");
            }
            if (tokens[contS - 1].getTipo() == 0 && tokens[contS - 1].getId() >= 500) {
                errorSema++;
                erroresS = "Error Semantico: Unexpected " + tokens[contS - 1].getToken();
                errores.push_back(erroresS + "\n");
            } else {
                int contA = contS - 1;
                if (tokens[contA].getTipo() == 182) {
                    if (tokens[contS + 1].getTipo() != 182) {
                        errorSema++;
                        erroresS = "Error Semantico: Datatype not match " +
                                   semantico.getDataType(tokens[contA].getTipo()) +
                                   ":" + semantico.getDataType(tokens[contS].getTipo());
                        errores.push_back(erroresS + "\n");

                    }
                } else {
                    string operacion = "";
                    bool op = true;
                    while (incrementarS() && tokens[contS].getId() != 134 && tokens[contS].getId() != 131) {
                        operacion += tokens[contS].getToken();
                        if (tokens[contS].getId() >= 500)
                            op = false;
                        if (!esAritmetico(tokens[contS].getId())) {
                            if (semantico.isFloat(tokens[contA], tokens[contS])) {
                                if (!semantico.sameFamily(tokens[contA], tokens[contS])) {
                                    op = false;
                                    errorSema++;
                                    erroresS =
                                            "Error Semantico: Datatype not match " +
                                            semantico.getDataType(tokens[contA].getTipo()) +
                                            ":" + semantico.getDataType(tokens[contS].getTipo());
                                    errores.push_back(erroresS + "\n");
                                }
                            } else {
                                if (tokens[contA].getTipo() == 180) {
                                    if (tokens[contA].getTipo() != tokens[contS].getTipo()) {
                                        op = false;
                                        errorSema++;
                                        erroresS =
                                                "Error Semantico: Datatype not match " +
                                                semantico.getDataType(tokens[contA].getTipo()) +
                                                ":" + semantico.getDataType(tokens[contS].getTipo());
                                        errores.push_back(erroresS + "\n");
                                    }
                                }
                            }
                        }
                    }
                    if (op)
                        cout << "Notacion Polaca: " << convertir(operacion) << endl;
                }
            }
        }
    } while (incrementarS());
}

int obtenerTipoDato(int id) {
    int tipo;
    switch (id) {
        case 155:
            tipo = 180;
            break;
        case 154:
            tipo = 181;
            break;
        case 151:
            tipo = 182;
            break;
    }
    return tipo;
}
