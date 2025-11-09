#include <iostream>
#include <random>

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

class SufffixNode {
    size_t inicio; //El valor donde comienza el string
    int& fin; //El valor donde termina el string
    int idx; //-1 si no es hoja, igual puedo saber que es hoja si su fin es igual al tamaño del SuffixTree

    //Primero vamos a hacerlo con vectores sus hijos
    //HashTable<char,SufffixNode*> hijos;
    vector<SufffixNode*> hijos;

public:
    SufffixNode(int& end): fin(end) {
        idx = -1;
    }
};

class SufffixTree {
    int tamaño=-1;
    SufffixNode* raiz;
    string palabra; //Palabra que se almacena

public:
    SufffixTree(string word) {
        raiz=new SufffixNode(tamaño);
        palabra=word;
    }

    int& getTamano() {
        return tamaño;
    }

    string getPalabra() {
        return palabra;
    }

    SufffixNode* getRoot() {
        return raiz;
    }

};

//Tengo que comprobar que siempre los Root seran los correctos
bool NoPath(char letra, SufffixNode* aRoot, int aLenght, int aNode) {


    return false;
}

void UkkonenAlgorithm(SufffixTree tree) {
    string valor=tree.getPalabra();

    int remaining=0;
    SufffixNode* activeRoot=tree.getRoot();
    int activeNode=-1;
    int activeLenght=0;
    int& end=tree.getTamano();

    for (int i=0; i<valor.size();i++) {
        remaining++; //Aumentamos el remaining;

        if (NoPath(valor[i], activeRoot, activeLenght,activeNode)) {

        }
    }

}

int main() {
    return 0;
}