#include <iostream>
#include <fstream> //libreria per leggere e scrivere su file
#include <sstream>//libreria per manipolare stringhe
#include <queue>//libreria per la queue, utilizzata nel BFS
#include <set> //libreria per il set per avere le liste di adiacenza in ordine crescente di nodo (etichetta)
#include <stack>//per lo stack per ricavare lo shortest-path
using namespace std;

/*
nella rappresentazione del grafo tramite liste di adiacenza lo facciamo con un array dinamico
contenente per elementi dei set ordinati di interi che rappresentano i nodi adiacenti.
*/

class Graph {

private:
    
    set<int> *adjList; //array dinamico
    int size;   

public:
    
    Graph(int n){ //costruttore
        adjList = new set<int>[n+1]; //sommiamo 1 ad n, perchè gli indici partono da 0
        size = n+1;
    }

    ~Graph(){ //distruttore
        delete[] adjList;
    }

    void addEdge(int u, int v){  //metodo per aggiungere gli adiacenti tramite l'utilizzo di 'insert'
        adjList[u].insert(v);
    } 

    //metodo per calcolare e poi stampare lo shortest path tra start e end (SE ESISTE)
    
    void shortestPath(int start, int end){

        stack<int> path; // stack che contiene il percorso più breve tra i nodi
        bool *visited = new bool[size]; // array per tenere traccia dei nodi visitati
        int *parent = new int[size]; // array per tenere traccia dei genitori dei nodi
        queue<int> q; // coda per gestire l'esplorazione BFS

        // Inizializzazione: segnare tutti i nodi come non visitati 
        for (int node = 0; node < size; node++) {
            visited[node] = false;
            parent[node] = 0;
        }

        // Inizializzazione del nodo di partenza(marchiamo il nostro nodo di partenza)
        visited[start] = true;
        q.push(start);

        // Esecuzione del BFS
        while (!q.empty()) {
            int v = q.front(); // prende il primo elemento dalla coda
            q.pop();//lo estrae

            // Esplorare tutti i vicini non visitati del nodo v
            for (int u : adjList[v]) {
                if (!visited[u]) {
                    visited[u] = true; // segnare u come visitato
                    parent[u] = v; // impostare v come genitore di u
                    q.push(u); // inserire u nella coda

                    // Se abbiamo raggiunto il nodo di destinazione, possiamo terminare
                    if (u == end) {
                        break;
                    }
                }
            }
        }

        /*Ricostruzione del percorso dal nodo end al nodo start utilizzando il predecessore, si noti 
        che il percorso esiste solo se i due nodi sono connessi, quindi solo se 
        la BFS a partire dal nodo start ha visitato il nodo end*/

        if(visited[end] != false){ //se visited[end] è true allora significa che esiste un percorso dal nodo 'start' al nodo 'end'
            //aggiunge i nodi in ordine inverso alla pila
            while (end != start) {
                path.push(end); //viene aggiunto alla pila il nodo end
                end = parent[end]; //il valore di end viene aggiornato per diventare il nodo predecessore del nodo corrente nel percorso trovato
            }
            path.push(start); //il nodo 'start' viene aggiunto alla pila

            //stampa dello shortest path visitando la pila dalla cima al fondo
            while(! path.empty()) {
                cout << path.top() << " ";
                path.pop();
            }
            cout << endl;
        } else{
            cout<<"Path non esiste"<<endl;
        }
    }    
};

int main() {
    ifstream inputFile("input.txt" ); // "input.txt" //"input1 - Grafo Connesso.txt" // "input2 - Grafo Non Connesso (NO PATH).txt" //"input3 - Grafo Connesso Multi-Path.txt"
    if (!inputFile.is_open()) {
        cout << "Impossibile aprire il file!" << endl;
        return 1;
    }

    // Lettura del numero di nodi
    int n;
    inputFile >> n;

    //controllo se è nel range consentito
    if(n<1 || n>100){
        cout<<"Errore: il numero di router deve essere compreso tra 1 e 100."<<endl;
        return 1;
    }

    // Costruzione del grafo
    Graph graph(n);
    for (int i = 0; i < n; ++i) {
        string line;
        inputFile >> line;

        stringstream ss(line);
        string nodeStr;
        getline(ss, nodeStr, '-'); //legge la stringa fino al carattere '-' e memorizza in nodeStr
        int node = stoi(nodeStr); //stoi è un metodo che converte in intero

        if (node<1 || node >n){
            cout<<"Errore, il numero dei router deve essere compreso tra 1 e "<<n<<"."<<endl;
            return 1;
        }

        string neighborsStr;
        getline(ss, neighborsStr); //leggiamo il resto della riga e salviamo in neighborsStr
        stringstream ss_neighbors(neighborsStr);
        
        //lettura dei singoli vicini e aggiunta degli archi
        string neighbor;
        int neighborCount=0;
         while (getline(ss_neighbors, neighbor, ',')) {
            if(neighborCount>=25){
                cout<<"Errore, il router "<<node<<" vede piu' di 25 altri router."<<endl;
                return 1;
            }
            graph.addEdge(node, stoi(neighbor));
            ++neighborCount;
        }
    }

    // Lettura del numero di percorsi
    int m;
    inputFile >> m;

    // Calcolo e stampa dei percorsi minimi
    for (int i = 0; i < m; ++i) {
        int start, end;
        inputFile >> start >> end;
        graph.shortestPath(start, end); 
    }

    inputFile.close();
}


/*PSEUDOCODICE DI RIFERIMENTO PER RICERCA BFS
    Unmark tutti i vertici e inizializza una coda Q
    Mark s and Q.ENQUEUE(s) 
    While Q is not empty: 
        v = Q.DEQUEUE()
        For each unmarked adiacente u di v:
            Mark u
            Q.ENQUEUE(u)
*/