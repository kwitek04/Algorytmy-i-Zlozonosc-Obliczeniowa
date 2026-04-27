#include "Node.h"

// Konstruktor dla korzenia
Node::Node(int** initialMatrix, int n) {
    N = n;
    level = 0;
    cost = 0;
    
    // dynamiczna alokacja pamięci dla macierzy i tablic cykli
    matrix = new int*[N];
    startNode = new int[N];
    endNode = new int[N];
    edges = new PathEdge[N];
    
    for(int i = 0; i < N; i++) {
        matrix[i] = new int[N];
        for(int j = 0; j < N; j++) {
            // zabezpieczenie przed jazdą z miasta do samego siebie
            if (i == j) matrix[i][j] = INT_MAX; 
            else matrix[i][j] = initialMatrix[i][j];
        }
        // początkowo każde miasto jest początkiem i końcem własnej ścieżki
        startNode[i] = i;
        endNode[i] = i;
    }
    
    // pierwsza redukcja macierzy
    cost = reduceMatrix();
}

// Konstruktor dla dziecka
Node::Node(Node* parent) {
    N = parent->N;
    level = parent->level;
    cost = parent->cost;
    
    edges = new PathEdge[N];
    for (int i = 0; i < level; i++) {
        edges[i] = parent->edges[i];
    }
    
    // dynamiczna alokacja pamięci
    matrix = new int*[N];
    startNode = new int[N];
    endNode = new int[N];
    
    // kopiowanie macierzy i stanu cykli od rodzica
    for(int i = 0; i < N; i++) {
        matrix[i] = new int[N];
        for(int j = 0; j < N; j++){
            matrix[i][j] = parent->matrix[i][j];
        } 
        startNode[i] = parent->startNode[i];
        endNode[i] = parent->endNode[i];
    }
}

// Destruktor
Node::~Node() {
    for(int i = 0; i < N; i++) delete[] matrix[i];
    delete[] matrix;
    delete[] startNode;
    delete[] endNode;
    delete[] edges;
}

// Funkcja redukująca macierz
int Node::reduceMatrix() {
    int reducedCost = 0;
    
    // redukcja wierszy
    for (int i = 0; i < N; ++i) {
        int minVal = INT_MAX;
        // szukamy minimum w wierszu
        for (int j = 0; j < N; ++j) {
            if (matrix[i][j] < minVal) minVal = matrix[i][j];
        }
        // odejmujemy minimum od całego wiersza
        if (minVal != INT_MAX && minVal > 0) {
            reducedCost += minVal;
            for (int j = 0; j < N; ++j) {
                if (matrix[i][j] != INT_MAX) matrix[i][j] -= minVal;
            }
        }
    }
    
    // redukcja kolumn
    for (int j = 0; j < N; ++j) {
        int minVal = INT_MAX;
        // szukamy minimum w kolumnie
        for (int i = 0; i < N; ++i) {
            if (matrix[i][j] < minVal) minVal = matrix[i][j];
        }
        // odejmujemy minimum od całej kolumny
        if (minVal != INT_MAX && minVal > 0) {
            reducedCost += minVal;
            for (int i = 0; i < N; ++i) {
                if (matrix[i][j] != INT_MAX) matrix[i][j] -= minVal;
            }
        }
    }
    
    return reducedCost;
}

// Funkcja szukająca zera z największą karą za jego ominięcie
Edge Node::findBestEdge() {
    int maxPenalty = -1;
    Edge bestEdge = {-1, -1, -1};
    
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (matrix[i][j] == 0) {
                int rowMin = INT_MAX, colMin = INT_MAX;
                
                // szukamy najtańszej alternatywy w wierszu pomijając to zero
                for (int k = 0; k < N; ++k) {
                    if (k != j && matrix[i][k] < rowMin) rowMin = matrix[i][k];
                }
                
                // szukamy najtańszej alternatywy w kolumnie pomijając to zero
                for (int k = 0; k < N; ++k) {
                    if (k != i && matrix[k][j] < colMin) colMin = matrix[k][j];
                }
                
                int penalty = 0;
                if (rowMin != INT_MAX) penalty += rowMin;
                if (colMin != INT_MAX) penalty += colMin;

                // zapisujemy krawędź która ma największą karę
                if (penalty > maxPenalty) {
                    maxPenalty = penalty;
                    bestEdge = {i, j, penalty};
                }
            }
        }
    }
    
    return bestEdge;
}

// Funkcja dla lewego dziecka czyli akceptacja krawędzi
void Node::includeEdge(int u, int v) {
    edges[level] = {u, v};
    level++;
    
    // odczytujemy początek i koniec obecnego fragmentu trasy
    int start = startNode[u];
    int end = endNode[v];
    
    int oldStartV = startNode[v];
    int oldEndU = endNode[u];

    // zabezpieczenie przed stworzeniem przedwczesnego cyklu
    matrix[end][start] = INT_MAX; 

    // aktualizacja tablic początków i końców po połączeniu ścieżki
    for(int i = 0; i < N; i++) {
        if (startNode[i] == oldStartV) startNode[i] = start;
        if (endNode[i] == oldEndU) endNode[i] = end;
    }
    
    // wykreślamy wykorzystany wiersz i kolumnę wpisując nieskończoność
    for (int i = 0; i < N; ++i) {
        matrix[u][i] = INT_MAX;
        matrix[i][v] = INT_MAX;
    }
}

// Funkcja dla prawego dziecka czyli odrzucenie krawędzi
void Node::excludeEdge(int u, int v) {
    // wpisujemy nieskończoność żeby całkowicie zabronić jazdy tą drogą
    matrix[u][v] = INT_MAX;
}