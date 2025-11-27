#include <iostream>
#include "suffixTree.h"
using namespace std;

int main() {
    // string palabra="ATTAAAGGTTTATACCTTCCCAGGTAACAAACCAACCAACTTTCGATCTCTTGTAGATCT$";
    string palabra = "abaaba$";

    SufffixTree sufffix_tree(palabra);

    sufffix_tree.buildSuffix();
    sufffix_tree.asignarId();

    sufffix_tree.imprimirPorNiveles();

    cout << endl;
    sufffix_tree.imprimirJson(sufffix_tree.raiz, palabra);

    cout << endl;
    cout << "Existe la palabra 'ATTA': " << (sufffix_tree.existePalabra("ATTA") ? "Si" : "No") << endl;

    cout << "Veces que se repite 'a': " << sufffix_tree.cantidadVeces("a") << endl;
    cout << "Veces que se repite 'aba': " << sufffix_tree.cantidadVeces("aba") << endl;

    return 0;
}