#include "AtspProblem.h"
#include <iostream>
#include <fstream>
#include <limits> // Biblioteka używana tylko do INT_MAX

using namespace std;

// Konstruktor
AtspProblem::AtspProblem() {
    N = 0;
    distanceMatrix = nullptr;
    bestPath = nullptr;
    minCost = INT_MAX;
}

// Destruktor
AtspProblem::~AtspProblem() {
    freeMemory();
}

// Funkcja zwalniająca pamięć
void AtspProblem::freeMemory() {
    if (distanceMatrix != nullptr) {
        for (int i = 0; i < N; ++i) {
            delete[] distanceMatrix[i];
        }
        delete[] distanceMatrix;
        distanceMatrix = nullptr;
    }
    if (bestPath != nullptr) {
        delete[] bestPath;
        bestPath = nullptr;
    }
}

// Funkcja do wczytywania danych z pliku
bool AtspProblem::loadFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Blad: Nie mozna otworzyc pliku " << filename << endl;
        return false;
    }

    //zwalniamy pamięć i inicjalizujemy N które będzie przechowywać liczbę miast
    freeMemory();
    N = 0;
    string word;

    while (file >> word) {
        // znalezione pliki atsp z danymi mają różne formaty raz jest DIMENSION: N, a raz jest DIMENSION : N, więc trzeba dobrze obsłużyć odczyt N
        if (word == "DIMENSION:") {
            file >> N;
        }
        else if (word == "DIMENSION") {
            string next_word;
            file >> next_word;             
            if (next_word == ":") {
                file >> N; 
            } else {
                N = stoi(next_word); 
            }
        } 
        else if (word == "EDGE_WEIGHT_SECTION") {
            break; 
        }
    }

    if (N <= 0) {
        cout << "Blad: Nie udalo sie odczytac wymiarow macierzy" << endl;
        file.close();
        return false;
    }

    // alokacja pamięci dla macierzy odległości i najlepszej ścieżki
    distanceMatrix = new int*[N];
    for (int i = 0; i < N; ++i) {
        distanceMatrix[i] = new int[N];
    }
    bestPath = new int[N];

    // wczytywanie do macierzy odległości danych z pliku
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            file >> distanceMatrix[i][j];
        }
    }
    
    file.close();
    return true;
}

// Funkcja do wyświetlania macierzy
void AtspProblem::displayData() const {
    if (distanceMatrix == nullptr) return;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            cout << distanceMatrix[i][j] << "\t";
        }
        cout << endl;
    }
}

// Funkcja do obliczania kosztu zadanej ścieżki
int AtspProblem::calculatePathCost(int* path) {
    int cost = 0;
    for (int i = 0; i < N - 1; ++i) {
        cost += distanceMatrix[path[i]][path[i + 1]];
    }
    // trzeba jeszcze dodać odległość między ostatnim miastem, a miastem początkowym
    cost += distanceMatrix[path[N - 1]][path[0]]; 
    return cost;
}

// Funkcja obliczająca wartości wszsytkich możliwych ścieżek. Zapamiętuje wartość najkrótszej ścieżki w minCost, i kolejność odwiedzanych miast w bestPath
void AtspProblem::bruteForceHelper(int* currentPath, int step, bool* visited) {
    // gdy cała ścieżka została zbudowana to liczymy jej wartość i zapamiętujemy jeśli jest najlepsza
    if (step == N) {
        int currentCost = calculatePathCost(currentPath);
        if (currentCost < minCost) {
            minCost = currentCost;
            for (int i = 0; i < N; i++) bestPath[i] = currentPath[i];
        }
        return;
    }

    // w każdym kroku funkcja uruchamia samą siebie po dodaniu wszystkich możliwych przejść do jeszcze nieodwiedzonych miast
    for (int i = 1; i < N; ++i) {
        if (!visited[i]) {
            visited[i] = true;
            currentPath[step] = i;
            bruteForceHelper(currentPath, step + 1, visited);
            visited[i] = false;
        }
    }
}

// Funkcja inicjująca algorytm brute force
int AtspProblem::runBruteForce() {
    if (distanceMatrix == nullptr){
        cout << "Macierz jest pusta\n";
        return -1;
    }
    
    minCost = INT_MAX;
    
    int* currentPath = new int[N];
    bool* visited = new bool[N];
    for (int i = 0; i < N; ++i) visited[i] = false;

    // zawsze zaczynamy od miasta nr 0
    currentPath[0] = 0;
    visited[0] = true;

    // uruchamiamy funkcję która oblicza wartości wszystkich możliwych ścieżek i zwraca najlepszą
    bruteForceHelper(currentPath, 1, visited);

    cout << "\nAlgorytm Brute Force - najlepszy koszt: " << minCost << "\nTrasa: ";
    for (int i = 0; i < N; ++i) cout << bestPath[i] << " -> ";
    cout << bestPath[0] << endl;

    delete[] currentPath;
    delete[] visited;

    return minCost;
}

// Algorytm zachłanny Nearest Neighbour (NN)
int AtspProblem::runNearestNeighbor(int startCity) {
    if (distanceMatrix == nullptr || startCity < 0 || startCity >= N){
    cout << "Bledne dane startowe\n";
    return -1;
    }

    bool* visited = new bool[N];
    for (int i = 0; i < N; ++i) visited[i] = false;

    // miasto startowe ustawiamy jako obecne miasto i ustawiamy je jako odwiedzone
    int currentCity = startCity;
    visited[currentCity] = true;
    
    int totalCost = 0;
    
    bestPath[0] = currentCity;

    // w każdym korku musimy znaleźć najbliższe misato od obecnego
    for (int step = 1; step < N; step++) {
        int nearestCity = -1;
        int minDistance = INT_MAX;

        // szukamy nieodwiedzonego miasta z najmniejszym kosztem
        for (int i = 0; i < N; ++i) {
            if (!visited[i] && distanceMatrix[currentCity][i] < minDistance) {
                minDistance = distanceMatrix[currentCity][i];
                nearestCity = i;
            }
        }

        visited[nearestCity] = true;
        totalCost += minDistance;
        currentCity = nearestCity;
        bestPath[step] = currentCity;
    }

    // trzeba jeszcze dodać odległość między ostatnim miastem, a miastem początkowym
    totalCost += distanceMatrix[currentCity][startCity];

    cout << "\nAlgorytm Nearest Neighbour - start z miasta: " << startCity << endl;
    cout << "najlepszy koszt: " << totalCost << "\nTrasa: ";
    for (int i = 0; i < N; ++i) cout << bestPath[i] << " -> ";
    cout << bestPath[0] << endl;

    delete[] visited;

    return totalCost;
}

// Algorytm Losowy (RAND)
int AtspProblem::runRandom(int iterations) {
    if (distanceMatrix == nullptr) {
        cout << "Macierz jest pusta\n";
        return -1;
    }

    minCost = INT_MAX;
    int* currentPath = new int[N];

    // ustawiamy początkowo ścieżkę rosnąco według indeksu miasta
    for (int i = 0; i < N; ++i) {
        currentPath[i] = i;
    }

    // wykonujemy zadaną liczbę losowań
    for (int k = 0; k < iterations; k++) {
        // przechodzimy przez wszytskie miasta poza miastem 0 które pozostaje stale przypisane na początek
        for (int i = N - 1; i > 1; i--) {
            // losujemy indeks na który ustawimy aktualnie rozważane miasto
            int j = 1 + rand() % i;
            int temp = currentPath[i];
            currentPath[i] = currentPath[j];
            currentPath[j] = temp;
        }

        // funkcja oblicza wartość wylosowanej trasy
        int currentCost = calculatePathCost(currentPath);

        // jeśli wylosowana trasa jest lepsza to ją zapisujemy
        if (currentCost < minCost) {
            minCost = currentCost;
            for (int i = 0; i < N; ++i) {
                bestPath[i] = currentPath[i];
            }
        }
    }

    cout << "\nAlgorytm Losowy (" << iterations << " iteracji) - najlepszy koszt: " << minCost << "\nTrasa: ";
    for (int i = 0; i < N; ++i) cout << bestPath[i] << " -> ";
    cout << bestPath[0] << endl;

    delete[] currentPath;

    return minCost;
}

// Algorytm Repetitive Nearest Neighbour (RNN)
int AtspProblem::runRepetitiveNearestNeighbor() {
    if (distanceMatrix == nullptr) {
        cout << "Macierz jest pusta\n";
        return -1;
    }

    minCost = INT_MAX;
    
    int* currentPath = new int[N];
    bool* visited = new bool[N];

    // sprawdzamy każde z miast jako potencjalne miasto początkowe
    for (int startCity = 0; startCity < N; ++startCity) {
        
        // resetujemy tablicę odwiedzonych miast dla każdego nowego startu
        for (int i = 0; i < N; ++i) visited[i] = false;

        int currentCity = startCity;
        visited[currentCity] = true;
        
        int currentCost = 0;
        currentPath[0] = currentCity;

        // w każdym kroku szukamy najbliższego miasta od obecnego
        for (int step = 1; step < N; step++) {
            int nearestCity = -1;
            int minDistance = INT_MAX;

            for (int i = 0; i < N; ++i) {
                if (!visited[i] && distanceMatrix[currentCity][i] < minDistance) {
                    minDistance = distanceMatrix[currentCity][i];
                    nearestCity = i;
                }
            }

            visited[nearestCity] = true;
            currentCost += minDistance;
            currentCity = nearestCity;
            currentPath[step] = currentCity;
        }

        // trzeba jeszcze dodać odległość między ostatnim miastem, a miastem początkowym
        currentCost += distanceMatrix[currentCity][startCity];

        // jeśli znaleziona trasa dla obecnego miasta startowego jest lepsza to ją zapisujemy
        if (currentCost < minCost) {
            minCost = currentCost;
            for (int i = 0; i < N; ++i) {
                bestPath[i] = currentPath[i];
            }
        }
    }

    cout << "\nAlgorytm Repetitive Nearest Neighbour (RNN) - najlepszy koszt: " << minCost << "\nTrasa: ";
    for (int i = 0; i < N; ++i) cout << bestPath[i] << " -> ";
    cout << bestPath[0] << endl;

    delete[] visited;
    delete[] currentPath;

    return minCost;
}

// Funkcja generująca losowy graf o podanym rozmiarze i zapisująca go do pliku
void AtspProblem::generateRandomGraph(int size, const string& filename) {
    if (size <= 0) {
        cout << "Rozmiar grafu musi byc wiekszy od 0" << endl;
        return;
    }

    freeMemory(); 
    N = size;

    // alokacja pamięci
    distanceMatrix = new int*[N];
    for (int i = 0; i < N; ++i) {
        distanceMatrix[i] = new int[N];
    }
    bestPath = new int[N];

    // wypełnianie macierzy losowymi wartościami
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (i == j) {
                distanceMatrix[i][j] = 0; // odległość do samego siebie ustawiana na 0
            } else {
                distanceMatrix[i][j] = rand() % 100 + 1; // losowa odległość z zakresu od 1 do 100
            }
        }
    }

    // zapisywanioe wygenerowanego grafu do pliku
    ofstream file(filename);
    if (file.is_open()) {
        file << "NAME : " << filename << "\n";
        file << "COMMENT : ATSP wygenerowany dla " << N << " miast\n";
        file << "TYPE : ATSP\n";
        file << "DIMENSION : " << N << "\n";
        file << "EDGE_WEIGHT_TYPE : EXPLICIT\n";
        file << "EDGE_WEIGHT_FORMAT : FULL_MATRIX\n";
        file << "EDGE_WEIGHT_SECTION\n";

        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                file << distanceMatrix[i][j] << " ";
            }
            file << "\n";
        }
        file.close();
        cout << "Wygenerowano losowy graf o rozmiarze " << N << "x" << N << " i zapisano do pliku: " << filename << "\n";
    } else {
        cout << "Blad: Nie udalo sie utworzyc pliku do zapisu!" << endl;
    }
}