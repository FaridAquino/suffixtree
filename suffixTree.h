//
// Created by WINDOWS 11 on 26/11/2025.
//

#ifndef SUFFIXTREE_H
#define SUFFIXTREE_H

#include <iostream>
#include <unordered_map>
#include <queue>
#include <string>

using namespace std;

// Estructura del Nodo
struct SufffixNode {
    int inicio;
    int fin;
    int idx;
    mutable int id;
    SufffixNode* suffixLink = nullptr;

    unordered_map<char, SufffixNode*> hijos;

    SufffixNode(int _inicio = -1, int _fin = -1) {
        inicio = _inicio;
        fin = _fin;
        idx = -1;
    }
};

// Estructura del Árbol
struct SufffixTree {
    int tamanio = -1;
    string palabra;
    SufffixNode* raiz = new SufffixNode();

    int remaining = 0;
    SufffixNode* activeNode = raiz;
    int activeEdge = -1;
    int activeLenght = 0;
    int end = -1;
    SufffixNode* s_linkt_to = nullptr;

    SufffixTree(string word) {
        tamanio = word.size();
        palabra = word;
    }

    void asignarId() {
        queue<SufffixNode*> cola;
        int contador = 0;
        raiz->id = contador;

        for (auto valor : raiz->hijos) {
            cola.push(valor.second);
        }

        while (!cola.empty()) {
            auto nodo = cola.front();
            cola.pop();
            contador++;
            nodo->id = contador;

            for (auto hijo : nodo->hijos) {
                cola.push(hijo.second);
            }
        }
    }

    void imprimirPorNiveles() const {
        queue<pair<char, SufffixNode*>> cola;
        cout << "tamanio de raiz:" << raiz->hijos.size() << endl;
        for (auto valor : raiz->hijos) {
            cola.push(valor);
        }

        int contador = 0;
        int nivel = 0;
        while (!cola.empty()) {
            nivel++;
            cout << "\n";
            contador = cola.size();
            cout << "tamanio del nivel " << nivel << ": " << contador << endl;
            for (int i = 0; i < contador; i++) {

                auto val = cola.front();
                const SufffixNode* nodo = val.second;

                int finCadena = nodo->fin;
                if (nodo->idx == -1) {
                    finCadena = end;
                }

                char letraSuffix = '#';
                if (nodo->suffixLink != nullptr && nodo->suffixLink != raiz) {
                    letraSuffix = palabra[nodo->suffixLink->inicio];
                }
                cout << val.first << " " << "[" << nodo->inicio << ";" << finCadena << "]" << "(" << nodo->hijos.size() << ")" << "(" << letraSuffix << ")" << "----";

                if (nodo->idx != -1) {
                    for (auto hijo : nodo->hijos) {
                        cola.push(hijo);
                    }
                }
                cola.pop();
            }
        }
    }

    void imprimirJson(SufffixNode* nodo, string textoCompleto) {
        int currentFin = (nodo->idx == -1) ? end : nodo->fin;

        cout << "{" << endl;
        cout << "\"name\": \"" << nodo->id << "\"," << endl;

        if (nodo == raiz) {
            cout << "\"edgeLabel\": \"ROOT\"," << endl;
        } else {
            string sub = textoCompleto.substr(nodo->inicio, currentFin - nodo->inicio + 1);
            cout << "\"edgeLabel\": \"" << sub << "\"," << endl;
            cout << "\"indices\": [" << nodo->inicio << ", " << currentFin << "]," << endl;
        }

        if (nodo->suffixLink != nullptr && nodo->suffixLink != raiz) {
            cout << "\"suffixLink\": \"" << nodo->suffixLink->id << "\"," << endl;
        } else {
            cout << "\"suffixLink\": null," << endl;
        }

        cout << "\"children\": [" << endl;
        int i = 0;
        for (auto const& [key, hijo] : nodo->hijos) {
            if (i > 0) cout << "," << endl;
            imprimirJson(hijo, textoCompleto);
            i++;
        }
        cout << "]" << endl;
        cout << "}";
    }

    int calcularTamanio(const SufffixNode* nodo) {
        int final = end;
        if (nodo->idx != -1) {
            final = nodo->fin;
        }
        return final - nodo->inicio + 1;
    }

    SufffixNode* creaCamino(int i, char caracter) {
        cout << "Se comienza la creacion de camino, entrando con active edge: " << activeEdge << "; active lenght: " << activeLenght << endl;
        auto* sufffixNode = new SufffixNode(i, end);

        if (activeLenght == 0) {
            activeNode->hijos[caracter] = sufffixNode;
            return nullptr;
        }

        SufffixNode* currentNode = activeNode->hijos[palabra[activeEdge]];
        auto* nuevoNode = new SufffixNode(currentNode->inicio, currentNode->inicio + (activeLenght - 1));

        currentNode->inicio = nuevoNode->fin + 1;
        activeNode->hijos[palabra[activeEdge]] = nuevoNode;

        nuevoNode->hijos[caracter] = sufffixNode;
        nuevoNode->hijos[palabra[currentNode->inicio]] = currentNode;
        nuevoNode->idx = 0;

        return nuevoNode;
    }
    //Algorithm 1 Algoritmo de Construccion de Ukkonen
    void buildSuffix() {
        for (int i = 0; i < tamanio; i++) {
            remaining++;
            end++;

            char caracter = palabra[i];

            while (remaining > 0) {
                cout << "i evaluado: " << caracter << endl;
                cout << "Active Edge: " << palabra[activeEdge] << "---" << "Active Lenght: " << activeLenght << " " << "Valor Active Edge: " << activeEdge << endl;
                cout << "Remaining: " << remaining << endl;

                if (activeNode->suffixLink) {
                    cout << "Tiene suffixlink: " << activeNode->suffixLink->inicio << endl;
                } else {
                    cout << "No tiene suffixlink" << endl;
                }

                imprimirPorNiveles();
                cout << endl;

                if (activeLenght > 0) {
                    SufffixNode* nodoSeguir = activeNode->hijos[palabra[activeEdge]];
                    int tamanioNodo = calcularTamanio(nodoSeguir);

                    while (tamanioNodo <= activeLenght) {
                        activeNode = nodoSeguir;
                        activeLenght -= tamanioNodo;
                        activeEdge += tamanioNodo;

                        if (activeLenght == 0) {
                            break;
                        }

                        nodoSeguir = activeNode->hijos[palabra[activeEdge]];
                        tamanioNodo = calcularTamanio(nodoSeguir);
                    }
                }

                if (activeLenght == 0) {
                    activeEdge = i;

                    if (activeNode->hijos.find(caracter) == activeNode->hijos.end()) {
                        creaCamino(i, caracter);

                        if (s_linkt_to != nullptr) {
                            s_linkt_to->suffixLink = activeNode;
                            s_linkt_to = nullptr;
                        }
                    } else {
                        activeEdge = activeNode->hijos[caracter]->inicio;
                        activeLenght = 1;

                        if (s_linkt_to != nullptr) {
                            s_linkt_to->suffixLink = activeNode;
                            s_linkt_to = nullptr;
                        }
                        break;
                    }
                } else {
                    SufffixNode* nodoDirigirse = activeNode->hijos[palabra[activeEdge]];
                    char siguientePalabra = palabra[nodoDirigirse->inicio + activeLenght];

                    cout << endl;
                    cout << "Estamos en la contradicion" << endl;
                    cout << "caracter: " << caracter << " y palabra esperada " << siguientePalabra << endl;

                    if (siguientePalabra == caracter) {
                        cout << "active edge anterior: " << activeEdge << endl;
                        cout << "Concuerdan las letras" << endl;
                        activeLenght++;

                        if (s_linkt_to != nullptr) {
                            s_linkt_to->suffixLink = activeNode;
                            s_linkt_to = nullptr;
                        }
                        break;
                    }

                    cout << "active edge anterior: " << activeEdge << endl;
                    cout << "No concuerdan las letras" << endl;
                    SufffixNode* nuevoNodoInterno = creaCamino(i, caracter);

                    if (s_linkt_to != nullptr) {
                        s_linkt_to->suffixLink = nuevoNodoInterno;
                    }

                    s_linkt_to = nuevoNodoInterno;
                }

                remaining--;

                if (activeNode == raiz) {
                    if (activeLenght > 0) {
                        activeLenght--;
                    }
                    activeEdge = i - (remaining - 1);
                } else {
                    activeNode = activeNode->suffixLink;
                }
            }
        }
        cout << "Remaining final: " << remaining << endl;
        cout << "Se construyo exitsamente" << endl;
    }

    //Algorithm 2 Algoritmo de Busqueda de Patron
    bool existePalabra(const string& buscar) {
        SufffixNode* camino = raiz;
        int indicePatron = 0;
        int n = buscar.size();

        while (indicePatron < buscar.size()) {
            char caracterActual = buscar[indicePatron];
            if (!camino->hijos.contains(caracterActual)) {
                return false;
            }
            camino = camino->hijos[caracterActual];
            int tamanioCamino = calcularTamanio(camino);

            for (int i = 0; i < tamanioCamino; i++) {
                if (indicePatron == n) {
                    return true;
                }
                char charEnArbol = palabra[camino->inicio + i];
                char charEnPatron = buscar[indicePatron];

                if (charEnPatron != charEnArbol) {
                    return false;
                }
                indicePatron++;
            }
        }
        return true;
    }

    int contarHojas(SufffixNode* nodo) {
        if (nodo->hijos.empty()) {
            return 1;
        }
        int total = 0;
        for (auto const& [key, hijo] : nodo->hijos) {
            total += contarHojas(hijo);
        }
        return total;
    }

    ///Algorithm 3 Algoritmo de Busqueda de Patron y cantidad de veces que se repit
    int cantidadVeces(const string& buscar) {
        SufffixNode* camino = raiz;
        int n = buscar.size();
        int i = 0;

        while (i < n) {
            char charActual = buscar[i];
            if (camino->hijos.find(charActual) == camino->hijos.end()) {
                return 0;
            }

            camino = camino->hijos[charActual];
            int tamanioCamino = calcularTamanio(camino);

            for (int j = 0; j < tamanioCamino && i < n; j++) {
                char charEnArbol = palabra[camino->inicio + j];
                char charEnPatron = buscar[i];

                if (charEnPatron != charEnArbol) {
                    return 0;
                }
                i++;
            }
        }
        return contarHojas(camino);
    }
};

#endif //SUFFIXTREE_H
