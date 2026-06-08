#include "SimulatedAnnealing.h"

// Konstruktor
SimulatedAnnealing::SimulatedAnnealing(int** m, int size) {
    N = size;
    matrix = m;
    bestPath = new int[N];
    bestCost = INT_MAX;
}

// Destruktor
SimulatedAnnealing::~SimulatedAnnealing() {
    delete[] bestPath;
}

// Liczenie kosztu konkretnej ułożonej ścieżki
int SimulatedAnnealing::calculateCost(int* path) {
    int cost = 0;
    for (int i = 0; i < N - 1; i++) {
        cost += matrix[path[i]][path[i + 1]];
    }
    // dodanie powrotu do miasta startowego
    cost += matrix[path[N - 1]][path[0]];
    return cost;
}

// Generowanie losowej trasy na start
void SimulatedAnnealing::generateRandomSolution(int* path) {
    for (int i = 0; i < N; i++) path[i] = i;
    
    // Tasowanie tablicy
    for (int i = N - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        std::swap(path[i], path[j]);
    }
}

// Generowanie trasy z użyciem algorytmu najbliższego sąsiada (NN)
void SimulatedAnnealing::generateGreedySolution(int* path) {
    bool* visited = new bool[N]();
    int currentCity = 0;
    path[0] = currentCity;
    visited[currentCity] = true;

    for (int step = 1; step < N; step++) {
        int nextCity = -1;
        int minEdge = INT_MAX;

        for (int j = 0; j < N; j++) {
            if (!visited[j] && matrix[currentCity][j] < minEdge) {
                minEdge = matrix[currentCity][j];
                nextCity = j;
            }
        }
        visited[nextCity] = true;
        path[step] = nextCity;
        currentCity = nextCity;
    }
    delete[] visited;
}

// Generowanie sąsiada: Metoda SWAP
void SimulatedAnnealing::getNeighborSwap(int* currentPath, int* neighborPath) {
    for (int i = 0; i < N; i++) neighborPath[i] = currentPath[i];
    
    int i = rand() % N;
    int j = rand() % N;
    while (i == j) {
        j = rand() % N;
    } 
    
    std::swap(neighborPath[i], neighborPath[j]);
}

// Generowanie sąsiada: Metoda INVERT
void SimulatedAnnealing::getNeighborInvert(int* currentPath, int* neighborPath) {
    for (int i = 0; i < N; i++) {
        neighborPath[i] = currentPath[i];  
    } 
    
    int i = rand() % N;
    int j = rand() % N;
    if (i > j) {
        std::swap(i, j);
    }

    while (i < j) {
        std::swap(neighborPath[i], neighborPath[j]);
        i++;
        j--;
    }
}

// Generowanie sąsiada: Metoda INSERT
void SimulatedAnnealing::getNeighborInsert(int* currentPath, int* neighborPath) {
    for (int k = 0; k < N; k++) {
        neighborPath[k] = currentPath[k];
    }
    
    int i = rand() % N;
    int j = rand() % N;
    while (i == j) {
        j = rand() % N;
    }

    int cityToMove = neighborPath[i];

    // przesuwanie reszty miast
    if (i < j) {
        for (int k = i; k < j; k++) {
            neighborPath[k] = neighborPath[k + 1];
        }
    } else {
        for (int k = i; k > j; k--) {
            neighborPath[k] = neighborPath[k - 1];
        }
    }

    neighborPath[j] = cityToMove;
}

// Główny algorytm symulowanego wyżarzania
int* SimulatedAnnealing::solve(double initialTemp, double coolingRate, int epochLength, int coolingScheme, int initMethod, int neighborMethod) {
    int* currentPath = new int[N];
    int* neighborPath = new int[N];

    // inicjalizacja trasy początkowej
    if (initMethod == 1) {
        generateRandomSolution(currentPath);
    } else {
        generateGreedySolution(currentPath);
    }

    int currentCost = calculateCost(currentPath);
    bestCost = currentCost;
    for (int i = 0; i < N; i++) {
        bestPath[i] = currentPath[i];
    }

    double T = initialTemp;
    double absoluteZero = 0.0001; // próg temperatury przerywający algorytm

    // główna pętla wyżarzania
    while (T > absoluteZero) {
        for (int i = 0; i < epochLength; i++) {
            
            if (neighborMethod == 1) {
                getNeighborSwap(currentPath, neighborPath);
            } else if (neighborMethod == 2) {
                getNeighborInvert(currentPath, neighborPath);
            } else {
                getNeighborInsert(currentPath, neighborPath);
            }

            int neighborCost = calculateCost(neighborPath);
            int delta = neighborCost - currentCost;

            // Jeśli nowa trasa jest lepsza, akceptujemy ją
            if (delta < 0) {
                currentCost = neighborCost;
                for (int k = 0; k < N; k++){
                    currentPath[k] = neighborPath[k];
                } 

                if (currentCost < bestCost) {
                    bestCost = currentCost;
                    for (int k = 0; k < N; k++) {
                        bestPath[k] = currentPath[k];
                    }
                }
            } 
            // Jeśli nowa trasa jest gorsza, akceptujemy z prawdopodobieństwem Boltzmanna
            else {
                double r = (double)rand() / RAND_MAX; // losowa liczba od 0.0 do 1.0
                if (exp(-delta / T) > r) { // liczba eurlera do potęgi -delta/T
                    currentCost = neighborCost;
                    for (int k = 0; k < N; k++) {
                        currentPath[k] = neighborPath[k];
                    }
                }
            }
        }

        // schładzanie temperatury
        if (coolingScheme == 1) {
            T = T * coolingRate;
        } else {
            T = T - coolingRate;
        }
    }

    delete[] currentPath;
    delete[] neighborPath;
    
return bestPath;
}

// Funkcja zwracająca koszt
int SimulatedAnnealing::getBestCost() {
    return bestCost;
}

// Funkcja do wyświetlania wyniku
void SimulatedAnnealing::displayResult() {
    std::cout << "Najlepszy koszt: " << bestCost << "\n";
    std::cout << "Najlepsza sciezka: ";
    for (int i = 0; i < N; i++) {
        std::cout << bestPath[i] << " -> ";
    }
    std::cout << bestPath[0] << "\n";
}