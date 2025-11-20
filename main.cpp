#include <iostream>
#include <unordered_map>
#include <random>
#include <queue>

using namespace std;
using namespace std;

int CharToInt(const char& s) {
    int hash = static_cast<char>(s);
    return hash;
}

template <typename T, typename R>
struct Node {
    T key;
    R value;
    Node* next;
    Node(T k, R v) : key(k), value(v), next(nullptr) {}
};

template <typename K, typename V>
class HashTable {
private:
    Node<K,V> ** table;
    int capacity;
    int size;
    float loadFactor;
    size_t a, b, p;

    int keyToInt(const K& key) const {
        if constexpr (is_integral<K>::value) {
            return key;
        } else {
            return stringToInt(key);
        }
    }

    int getIndex(const K& key) const {
        int k = keyToInt(key);
        return (a * k + b) % p % capacity;
    }

    // Rehashing
    void rehash() {
        random_device rd;
        mt19937_64 gen(rd());
        uniform_int_distribution<size_t> dist_a(1, p-1);
        uniform_int_distribution<size_t> dist_b(0, p-1);

        a = dist_a(gen);
        b = dist_b(gen);

        int oldCapacity = capacity;
        capacity *= 2;
        Node<K,V>** oldTable = table;

        table = new Node<K,V>*[capacity];
        for (int i = 0; i < capacity; i++) table[i] = nullptr;
        size = 0;

        // Reinsertar todos los nodos
        for (int i = 0; i < oldCapacity; i++) {
            Node<K,V>* node = oldTable[i];
            while (node) {
                insert(node->key, node->value);
                Node<K,V>* toDelete = node;
                node = node->next;
                delete toDelete;
            }
        }
        delete[] oldTable;
    }

public:
    HashTable(int cap = 8, float lf = 0.75f, int prime = 2147483647)
        : capacity(cap), size(0), loadFactor(lf), p(prime) {

        table = new Node<K,V>*[capacity];
        for (int i = 0; i < capacity; i++) table[i] = nullptr;

        random_device rd;
        mt19937_64 gen(rd());
        uniform_int_distribution<size_t> dist_a(1, p-1);
        uniform_int_distribution<size_t> dist_b(0, p-1);
        a = dist_a(gen);
        b = dist_b(gen);
    } //Bien

    ~HashTable() {
        for (int i = 0; i < capacity; i++) {
            Node<K,V>* node = table[i];
            while (node) {
                Node<K,V>* toDelete = node;
                node = node->next;
                delete toDelete;
            }
        }
        delete[] table;
    } //Bien

    void insert(const K& key, const V& value) {

        int idx = getIndex(key);

        Node<K,V>* node = table[idx];

        // Si ya existe la clave, mostrar en un cout que no se hizo la inserción
        while (node) {
            if (node->key == key) {
                cout<<"La llave ya existe, por ende no fue insertada";
                return;
            }
            node = node->next;
        }

        Node<K,V>* newNode=new Node<K,V>(key,value);
        newNode->next=table[idx];
        table[idx]=newNode;
        size++;


        // Verificar factor de carga
        if ((float)size / capacity > loadFactor) {
            rehash();
        }
    }


    V getFromKey(const K& key) {
        int idx=getIndex(key);

        Node<K,V>* node=table[idx];

        while (node) {
            if (node->key==key) {
                return node->value;
            }

            node=node->next;
        }

        throw out_of_range("La llave no existe en la tabla hash");
    }

    void erase(const K& key) {
        int idx = getIndex(key);
        Node<K,V>* node = table[idx];
        Node<K,V>* prev = nullptr;

        while (node) {
            if (node->key == key) {
                if (prev) prev->next = node->next;
                else table[idx] = node->next;
                delete node;
                size--;
                return;
            }
            prev = node;
            node = node->next;
        }
    }
};

struct SufffixNode {
    int inicio; //El valor donde comienza el string, -1 para root
    int fin; //El valor donde termina el string, -1 para root
    int idx; //-1 si no es hoja, 0 si es interno
    SufffixNode* suffixLink=nullptr; //nullptr si es root

    //Primero vamos a hacerlo con unorderedmap
    //HashTable<char,SufffixNode*> hijos;
    unordered_map<char,SufffixNode*> hijos;

    SufffixNode(int _inicio=-1, int _fin=-1) {
        inicio=_inicio;
        fin=_fin;
        idx=-1;
    }
};

struct SufffixTree {
    int tamanio=-1;
    string palabra; //Palabra que se almacena
    SufffixNode* raiz=new SufffixNode();

    int remaining=0;//Declaramos las variables necesarias
    SufffixNode* activeNode=raiz;
    int activeEdge=-1; //lo trabajamos como entero
    int activeLenght=0;
    int end=-1;
    SufffixNode* s_linkt_to=nullptr;

    SufffixTree(string word) {
        tamanio=word.size();
        palabra=word;
    }

    void imprimirPorNiveles() const {
        queue<pair<char,SufffixNode*>> cola;
        cout<<"tamanio de raiz:" <<raiz->hijos.size()<<endl;
        for (auto valor: raiz->hijos) {
            cola.push(valor);
        }

        int contador=0;
        int nivel=0;
        while (!cola.empty()) {
            nivel++;
            cout<<"\n";
            contador=cola.size();
            cout<<"tamanio del nivel " <<nivel<<": "<<contador<<endl;
            for (int i=0; i<contador; i++) {

                auto val=cola.front();
                const SufffixNode* nodo=val.second;

                int finCadena=nodo->fin;
                if (nodo->idx==-1) {
                    finCadena=end;
                }

                char letraSuffix='#';
                if (nodo->suffixLink!=nullptr and nodo->suffixLink!=raiz) {
                    letraSuffix=palabra[nodo->suffixLink->inicio];
                }
                cout<<val.first<<" "<<"["<<nodo->inicio<<";"<<finCadena<<"]"<<"("<<nodo->hijos.size()<<")"<<"("<<letraSuffix<<")"<<"----";

                if (nodo->idx!=-1) {
                    for (auto hijo: nodo->hijos) {
                        cola.push(hijo);
                    }
                }
                cola.pop();
            }
        }

    }

    int calcularTamanio(const SufffixNode* nodo) {
        int final=end;

        if (nodo->idx!=-1) {
            final=nodo->fin;
        }

        return final-nodo->inicio+1;
    }

    SufffixNode* creaCamino(int i, char caracter) {
        cout<<"Se comienza la creacion de camino, entrando con active edge: "<<activeEdge<<"; active lenght: "<<activeLenght<<endl;
        auto* sufffixNode=new SufffixNode(i,end);//Creamos el nuevo nodo

        if (activeLenght==0) { //Solo debemos agregarle una hoja
            activeNode->hijos[caracter]=sufffixNode;//Le agregamos una hoja
            return nullptr;
        }

        //Tenemos que recortar el nodo y agregarle la nueva hoja
        SufffixNode* currentNode=activeNode->hijos[palabra[activeEdge]]; //El nodo que contiene al caracter

        auto* nuevoNode=new SufffixNode(currentNode->inicio,currentNode->inicio+(activeLenght-1));

        currentNode->inicio=nuevoNode->fin+1;

        activeNode->hijos[palabra[activeEdge]]=nuevoNode; //Actualizamos como hijo su valor

        nuevoNode->hijos[caracter]=sufffixNode;
        nuevoNode->hijos[palabra[currentNode->inicio]]=currentNode;
        nuevoNode->idx=0;

        return nuevoNode;
    }

    void buildSuffix() {
        for (int i=0; i<tamanio;i++) {
            remaining++; //Aumentamos el remaining
            end++; //Aumentos el end

            char caracter=palabra[i]; //caracter nuevo del string

            while (remaining>0) {
                cout<<"i evaluado: "<<caracter<<endl;
                cout<<"Active Edge: "<<palabra[activeEdge]<<"---"<<"Active Lenght: "<<activeLenght<<" "<<"Valor Active Edge: "<<activeEdge<<endl;
                cout<<"Remaining: "<<remaining<<endl;

                if (activeNode->suffixLink) {
                    cout<<"Tiene suffixlink: "<<activeNode->suffixLink->inicio<<endl;
                }

                else {
                    cout<<"No tiene suffixlink"<<endl;
                }

                imprimirPorNiveles();
                cout<<endl;

                if (activeLenght>0) {
                    SufffixNode* nodoSeguir=activeNode->hijos[palabra[activeEdge]];

                    int tamanioNodo = calcularTamanio(nodoSeguir);

                    while (tamanioNodo <= activeLenght) {
                        activeNode = nodoSeguir;
                        activeLenght -= tamanioNodo;
                        activeEdge += tamanioNodo;

                        if (activeLenght == 0) {
                            break; // Salimos del bucle y re evaluamos si el active edge existe en ese nodo
                        }

                        nodoSeguir = activeNode->hijos[palabra[activeEdge]];
                        tamanioNodo = calcularTamanio(nodoSeguir);
                    }
                }

                if (activeLenght == 0) {
                    activeEdge = i; // El nuevo caracter es i

                    if (activeNode->hijos.find(caracter) == activeNode->hijos.end()) {

                        creaCamino(i, caracter);

                        if (s_linkt_to != nullptr) {
                            s_linkt_to->suffixLink = activeNode;
                            s_linkt_to = nullptr;
                        }
                    }

                    else {
                        activeEdge = activeNode->hijos[caracter]->inicio;
                        activeLenght = 1;

                        if (s_linkt_to != nullptr) {
                            s_linkt_to->suffixLink = activeNode;
                            s_linkt_to = nullptr;
                        }
                        break; //Terminamos la fase por la 3ra reggla de extension
                    }
                }

                else {
                    SufffixNode* nodoDirigirse=activeNode->hijos[palabra[activeEdge]];
                    char siguientePalabra=palabra[nodoDirigirse->inicio+activeLenght];

                    cout<<endl;
                    cout<<"Estamos en la contradicion"<<endl;
                    cout<<"caracter: "<<caracter<<" y palabra esperada "<<siguientePalabra<<endl;

                    if (siguientePalabra==caracter) {
                        cout<<"active edge anterior: "<<activeEdge<<endl;
                        cout<<"Concuerdan las letras"<<endl;
                        activeLenght++;

                        if (s_linkt_to != nullptr) {
                            s_linkt_to->suffixLink = activeNode;
                            s_linkt_to = nullptr;
                        }
                        break;
                    }

                    cout<<"active edge anterior: "<<activeEdge<<endl;
                    cout<<"No concuerdan las letras"<<endl;
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
        cout<<"Remaining final: "<<remaining<<endl;
        cout<<"Se construyo exitsamente"<<endl;
    }

    bool existePalabra(const string& buscar) {
        SufffixNode* camino=raiz;
        int indicePatron=0;
        int n = buscar.size();

        while (indicePatron<buscar.size()) {
            char caracterActual=buscar[indicePatron];
            if (!camino->hijos.contains(caracterActual)) {
                return false;
            }
            camino=camino->hijos[caracterActual];
            int tamanioCamino=calcularTamanio(camino);

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

    int cantidadVeces(const string& busccar) {


    }
};

int main() {
    //string palabra="ATTAAAGGTTTATACCTTCCCAGGTAACAAACCAACCAACTTTCGATCTCTTGTAGATCT$";
    string palabra="abaaba$";

    SufffixTree sufffix_tree(palabra);

    sufffix_tree.buildSuffix();

    sufffix_tree.imprimirPorNiveles();

    cout<<endl;
    cout<<"Existe la palabra: "<<sufffix_tree.existePalabra("ATTA")<<endl;

    return 0;
}