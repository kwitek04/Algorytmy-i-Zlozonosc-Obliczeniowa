#include "BranchAndBound.h"

// Konstruktor
BranchAndBound::BranchAndBound(int n) {
    N = n;
    bestPath = new int[N + 1]; 
    bestCost = INT_MAX;
}

// Destruktor
BranchAndBound::~BranchAndBound() {
    delete[] bestPath;
}

// Funkcja do obliczania upper bound za pomocą algorytmu zachłannego NN
void BranchAndBound::calculateInitialUpperBound(int** matrix) {
    bool* visited = new bool[N];
    for (int i = 0; i < N; i++) visited[i] = false;

    int currentCity = 0;
    visited[currentCity] = true;
    bestPath[0] = currentCity;
    
    int totalCost = 0;

    for (int step = 1; step < N; step++) {
        int nextCity = -1;
        int minEdge = INT_MAX;

        // szukamy najbliższego nieodwiedzonego sąsiada
        for (int j = 0; j < N; j++) {
            if (!visited[j] && matrix[currentCity][j] < minEdge) {
                minEdge = matrix[currentCity][j];
                nextCity = j;
            }
        }

        visited[nextCity] = true;
        totalCost += minEdge;
        bestPath[step] = nextCity;
        currentCity = nextCity;
    }

    // trzeba jeszcze dodać miasto początkowe
    totalCost += matrix[currentCity][0];
    bestPath[N] = 0;

    bestCost = totalCost; 
    
    delete[] visited;
}

// Best-First Search  
int BranchAndBound::solveLowestCost(int** matrix) {
    // Obliczanie Upper Bound algorytmem NN
    calculateInitialUpperBound(matrix);

    // inicjalizacja kolejki priorytetowej i węzła startowego
    std::priority_queue<Node*, std::vector<Node*>, CompareNode> pq;
    Node* root = new Node(matrix, N);
    pq.push(root);

    while (!pq.empty()) {
        Node* curr = pq.top();
        pq.pop();

        // usuwamy węzeł jeśli jego koszt jest większy lub równy najlepszemu znalezionemu
        if (curr->cost >= bestCost) {
            delete curr;
            continue;
        }

        // jeśli mamy N-1 wybranych krawędzi, została tylko jedna żeby zamknąć cykl
        if (curr->level == N - 1) {
            bool* hasOut = new bool[N]();
            bool* hasIn = new bool[N]();
            
            // odznaczamy już wykorzystane wyjazdy i wjazdy
            for (int i = 0; i < curr->level; i++) {
                hasOut[curr->edges[i].from] = true;
                hasIn[curr->edges[i].to] = true;
            }
            
            // szukamy brakującej krawędzi do domknięcia trasy
            int u = -1, v = -1;
            for (int i = 0; i < N; i++) {
                if (!hasOut[i]) u = i;
                if (!hasIn[i]) v = i;
            }
            
            // wpisujemy ostatnią krawędź do tablicy
            if (u != -1 && v != -1) {
                curr->edges[curr->level] = {u, v};
                curr->level++;
            }
            delete[] hasOut;
            delete[] hasIn;

            // rozkodowanie pełnej trasy i zliczenie ostatecznego kosztu
            int currentTourCost = 0;
            int* nextNode = new int[N];
            for (int i = 0; i < curr->level; i++) {
                currentTourCost += matrix[curr->edges[i].from][curr->edges[i].to];
                nextNode[curr->edges[i].from] = curr->edges[i].to;
            }

            // jeśli wyliczona trasa jest lepsza, to nadpisujemy wynik
            if (currentTourCost < bestCost) {
                bestCost = currentTourCost;
                int currCity = 0; 
                for (int i = 0; i < N; i++) {
                    bestPath[i] = currCity;
                    currCity = nextNode[currCity];
                }
                bestPath[N] = 0;
            }
            delete[] nextNode;
            delete curr;
            continue;
        }

        // szukamy najlepszej krawędzi na podstawie kar
        Edge bestEdge = curr->findBestEdge();
        if (bestEdge.from == -1) {
            delete curr;
            continue;
        }

        // lewe dziecko czyli akceptacja wybranej krawędzi
        Node* left = new Node(curr);
        left->includeEdge(bestEdge.from, bestEdge.to);
        left->cost += left->reduceMatrix();
        if (left->cost < bestCost) pq.push(left);
        else delete left;

        // prawe dziecko czyli odrzucenie krawędzi
        Node* right = new Node(curr);
        right->excludeEdge(bestEdge.from, bestEdge.to);
        right->cost += right->reduceMatrix(); 
        if (right->cost < bestCost) pq.push(right);
        else delete right;

        delete curr;
    }

    return bestCost;
}

// DFS
int BranchAndBound::solveDFS(int** matrix) {
    // Obliczanie Upper Bound algorytmem NN
    calculateInitialUpperBound(matrix);

    std::stack<Node*> s;
    Node* root = new Node(matrix, N);
    s.push(root);

    while (!s.empty()) {
        Node* curr = s.top();
        s.pop();

        // usuwamy węzeł jeśli koszt jest zbyt duży
        if (curr->cost >= bestCost) {
            delete curr;
            continue;
        }

        // zamykanie trasy z ostatnią krawędzią
        if (curr->level == N - 1) {
            bool* hasOut = new bool[N]();
            bool* hasIn = new bool[N]();
            for (int i = 0; i < curr->level; i++) {
                hasOut[curr->edges[i].from] = true;
                hasIn[curr->edges[i].to] = true;
            }
            
            int u = -1, v = -1;
            for (int i = 0; i < N; i++) {
                if (!hasOut[i]) u = i;
                if (!hasIn[i]) v = i;
            }
            
            if (u != -1 && v != -1) {
                curr->edges[curr->level] = {u, v};
                curr->level++;
            }
            delete[] hasOut; 
            delete[] hasIn;

            // rozkodowanie kosztu i kolejności
            int currentTourCost = 0;
            int* nextNode = new int[N];
            for (int i = 0; i < curr->level; i++) {
                currentTourCost += matrix[curr->edges[i].from][curr->edges[i].to];
                nextNode[curr->edges[i].from] = curr->edges[i].to;
            }

            // aktualizacja najlepszej ścieżki
            if (currentTourCost < bestCost) {
                bestCost = currentTourCost;
                int currCity = 0;
                for (int i = 0; i < N; i++) {
                    bestPath[i] = currCity;
                    currCity = nextNode[currCity];
                }
                bestPath[N] = 0;
            }
            delete[] nextNode;
            delete curr;
            continue;
        }

        Edge bestEdge = curr->findBestEdge();
        if (bestEdge.from == -1) {
            delete curr; 
            continue;
        }

        // lewe dziecko (akceptacja)
        Node* left = new Node(curr);
        left->includeEdge(bestEdge.from, bestEdge.to);
        left->cost += left->reduceMatrix();

        // prawe dziecko (odrzucenie)
        Node* right = new Node(curr);
        right->excludeEdge(bestEdge.from, bestEdge.to);
        right->cost += right->reduceMatrix(); 

        // na stos wrzucamy najpierw prawe dziecko a potem lewe żeby zeszło jako pierwsze
        if (right->cost < bestCost) s.push(right);
        else delete right;

        if (left->cost < bestCost) s.push(left);
        else delete left;

        delete curr;
    }
    return bestCost;
}

// BFS
int BranchAndBound::solveBFS(int** matrix) {
    // Obliczanie Upper Bound algorytmem NN
    calculateInitialUpperBound(matrix);

    std::queue<Node*> q;
    Node* root = new Node(matrix, N);
    q.push(root);

    while (!q.empty()) {
        Node* curr = q.front();
        q.pop();

        // odcinamy nieopłacalne węzły
        if (curr->cost >= bestCost) {
            delete curr;
            continue;
        }

        // zamykanie trasy z ostatnią krawędzią
        if (curr->level == N - 1) {
            bool* hasOut = new bool[N]();
            bool* hasIn = new bool[N]();
            for (int i = 0; i < curr->level; i++) {
                hasOut[curr->edges[i].from] = true;
                hasIn[curr->edges[i].to] = true;
            }
            
            int u = -1, v = -1;
            for (int i = 0; i < N; i++) {
                if (!hasOut[i]) u = i;
                if (!hasIn[i]) v = i;
            }
            
            if (u != -1 && v != -1) {
                curr->edges[curr->level] = {u, v};
                curr->level++;
            }
            delete[] hasOut; 
            delete[] hasIn;

            // rozkodowanie układu trasy
            int currentTourCost = 0;
            int* nextNode = new int[N];
            for (int i = 0; i < curr->level; i++) {
                currentTourCost += matrix[curr->edges[i].from][curr->edges[i].to];
                nextNode[curr->edges[i].from] = curr->edges[i].to;
            }

            // zapisywanie najlepszego wyniku
            if (currentTourCost < bestCost) {
                bestCost = currentTourCost;
                int currCity = 0;
                for (int i = 0; i < N; i++) {
                    bestPath[i] = currCity;
                    currCity = nextNode[currCity];
                }
                bestPath[N] = 0;
            }
            delete[] nextNode;
            delete curr;
            continue;
        }

        Edge bestEdge = curr->findBestEdge();
        if (bestEdge.from == -1) {
            delete curr; 
            continue;
        }

        // generowanie lewego i prawego dziecka
        Node* left = new Node(curr);
        left->includeEdge(bestEdge.from, bestEdge.to);
        left->cost += left->reduceMatrix();

        Node* right = new Node(curr);
        right->excludeEdge(bestEdge.from, bestEdge.to);
        right->cost += right->reduceMatrix(); 

        // w kolejce obojętne jest które wrzucimy pierwsze bo węzły będą wyciągane poziomami
        if (left->cost < bestCost) q.push(left);
        else delete left;

        if (right->cost < bestCost) q.push(right);
        else delete right;

        delete curr;
    }
    return bestCost;
}

// Funkcja do wyświetlania wyniku
void BranchAndBound::displayResult() {
    std::cout << "Najlepszy koszt (Branch & Bound): " << bestCost << "\n";
    std::cout << "Najlepsza sciezka: ";
    for (int i = 0; i <= N; i++) {
        std::cout << bestPath[i];
        if (i < N) std::cout << " -> ";
    }
    std::cout << "\n";
}