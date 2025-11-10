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
    SufffixNode* sufffixLink=nullptr; //nullptr si es root;

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

    int remaining=0;//Declaramos las variables necesarias, insertamos el primer elemento
    SufffixNode** activeNode=&raiz;
    char activeEdge;
    int activeLenght=0;
    int end=-1;
    SufffixNode* s_linkt_to=nullptr;

    SufffixTree(string word) {
        tamanio=word.size();
        raiz=new SufffixNode(); //inicializamos la raiz
        palabra=word;
    }

    void creaHoja(int i, char caracter) {
        (*activeNode)->idx=0;

        SufffixNode* sufffixNode=new SufffixNode(i,end); //Creamos el nuevo nodo
        if (activeLenght==0) {
            (*activeNode)->hijos[caracter]=sufffixNode; //Le agregamos como hijo su valor;
        }

        else {
            SufffixNode* currentNode=(*activeNode)->hijos[activeEdge]; //El nodo antiguo

            currentNode->fin=currentNode->inicio+(activeLenght-1);
            SufffixNode* recortadoNode=new SufffixNode(currentNode->fin+1,end);
            (*activeNode)->hijos[caracter]=sufffixNode; //Le agregamos como hijo su valor;

        }

    }

    void crearNodoInterno() {

    }

    void buildSuffix() {
        for (int i=0; i<tamanio;i++) {
            remaining++; //Aumentamos el remaining
            end++; //Aumentos el end

            char caracter=palabra[i]; //caracter del string
            int ubicacionSuffix;

            activeEdge=palabra[i-(remaining-1)];
            cout<<"Active Edge: "<<activeEdge<<endl;

            if (!(*activeNode)->hijos.contains(caracter)) { //Si no existe un camino, crearlo, segundaReglaExtension
                (*activeNode)->idx=0; //Ya no va a ser hoja, ahora va a ser un nodo interno;
                creaHoja(i, caracter); //Creamos la hoja
                remaining--;
            }
            else {
                remaining++;
                activeLenght++;
            }

            while (remaining>0) {
                remaining--;
            };
        }
    }

    void imprimirPorNiveles() {
        queue<pair<char,SufffixNode*>> cola;
        cout<<"tamanio de raiz:" <<raiz->hijos.size()<<endl;
        for (auto valor: raiz->hijos) {
            cola.push(valor);
        }

        int contador=0;
        while (!cola.empty()) {
            cout<<"\n";
            contador=cola.size();

            for (int i=0; i<contador; i++) {
                auto val=cola.front();
                SufffixNode* nodo=val.second;

                int finCadena=nodo->fin;
                if (nodo->idx==-1) {
                    finCadena=end;
                }
                cout<<val.first<<" "<<"["<<nodo->inicio<<";"<<finCadena<<"]"<<"----";

                if (nodo->idx!=-1) {
                    for (auto hijo: nodo->hijos) {
                        cola.push(hijo);
                    }
                }
                cola.pop();
            }
        }

    }

};



int main() {
    string palabra="abc$";

    SufffixTree sufffix_tree(palabra);

    sufffix_tree.buildSuffix();

    sufffix_tree.imprimirPorNiveles();

    return 0;
}