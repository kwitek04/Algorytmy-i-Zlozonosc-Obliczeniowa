#include "RandomSearch.h"
#include <iostream>
#include <limits> // Biblioteka używana tylko do INT_MAX
#include <cstdlib>

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

// Algorytm Losowy (RAND)
int runRandom(int** distanceMatrix, int N, int iterations) {
    if (distanceMatrix == nullptr) {
        cout << "Macierz jest pusta\n";
        return -1;
    }

    int minCost = INT_MAX;
    int* currentPath = new int[N];
    int* bestPath = new int[N];

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
        int currentCost = calculatePathCost(distanceMatrix, N, currentPath);

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
    delete[] bestPath;

    return minCost;
}