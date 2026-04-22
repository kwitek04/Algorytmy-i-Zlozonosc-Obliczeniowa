#include "BruteForce.h"
#include <iostream>
#include <limits> // Biblioteka używana tylko do INT_MAX

using namespace std;

// Funkcja do obliczania kosztu zadanej ścieżki
static int calculatePathCost(int** distanceMatrix, int N, int* path) {
    int cost = 0;
    for (int i = 0; i < N - 1; ++i) {
        cost += distanceMatrix[path[i]][path[i + 1]];
    }
    // trzeba jeszcze dodać odległość między ostatnim miastem, a miastem początkowym
    cost += distanceMatrix[path[N - 1]][path[0]]; 
    return cost;
}

// Funkcja obliczająca wartości wszsytkich możliwych ścieżek. Zapamiętuje wartość najkrótszej ścieżki w minCost, i kolejność odwiedzanych miast w bestPath
static void bruteForceHelper(int** distanceMatrix, int N, int* currentPath, int step, bool* visited, int& minCost, int* bestPath) {
    // gdy cała ścieżka została zbudowana to liczymy jej wartość i zapamiętujemy jeśli jest najlepsza
    if (step == N) {
        int currentCost = calculatePathCost(distanceMatrix, N, currentPath);
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
            bruteForceHelper(distanceMatrix, N, currentPath, step + 1, visited, minCost, bestPath);
            visited[i] = false;
        }
    }
}

// Funkcja inicjująca algorytm brute force
int runBruteForce(int** distanceMatrix, int N) {
    if (distanceMatrix == nullptr){
        cout << "Macierz jest pusta\n";
        return -1;
    }
    
    int minCost = INT_MAX;
    
    int* currentPath = new int[N];
    int* bestPath = new int[N];
    bool* visited = new bool[N];
    for (int i = 0; i < N; ++i) visited[i] = false;

    // zawsze zaczynamy od miasta nr 0
    currentPath[0] = 0;
    visited[0] = true;

    // uruchamiamy funkcję która oblicza wartości wszystkich możliwych ścieżek i zwraca najlepszą
    bruteForceHelper(distanceMatrix, N, currentPath, 1, visited, minCost, bestPath);

    cout << "\nAlgorytm Brute Force - najlepszy koszt: " << minCost << "\nTrasa: ";
    for (int i = 0; i < N; ++i) cout << bestPath[i] << " -> ";
    cout << bestPath[0] << endl;

    delete[] currentPath;
    delete[] bestPath;
    delete[] visited;

    return minCost;
}