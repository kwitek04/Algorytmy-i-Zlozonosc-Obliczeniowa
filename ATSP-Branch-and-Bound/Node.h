#pragma once
#include <climits>

// Struktura przechowująca sprawdzaną krawędź i karę za jej ominięcie
struct Edge {
    int from;
    int to;
    int penalty;
};

// Struktura przechowująca krawędź już zaakceptowaną do trasy
struct PathEdge {
    int from;
    int to;
};

class Node {
public:
    int** matrix;      // wskaźnik na zredukowaną macierz kosztów dla węzła
    int N;             // liczba miast
    int cost;          // dolne ograniczenie (Lower Bound)
    int level;         // liczba dotychczas zebranych krawędzi

    int* startNode;    // tablica do śledzenia początków fragmentów trasy
    int* endNode;      // tablica do śledzenia końców fragmentów trasy
    
    PathEdge* edges;   // tablica zebranych krawędzi

    Node(int** initialMatrix, int n);
    Node(Node* parent);
    ~Node();

    int reduceMatrix();
    Edge findBestEdge();
    void includeEdge(int u, int v);
    void excludeEdge(int u, int v);
};