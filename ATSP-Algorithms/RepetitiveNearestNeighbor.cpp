#include "RepetitiveNearestNeighbor.h"
#include <iostream>
#include <limits> // Biblioteka używana tylko do INT_MAX

using namespace std;

// Algorytm Repetitive Nearest Neighbour (RNN)
int runRepetitiveNearestNeighbor(int** distanceMatrix, int N) {
    if (distanceMatrix == nullptr) {
        cout << "Macierz jest pusta\n";
        return -1;
    }

    int minCost = INT_MAX;
    
    int* currentPath = new int[N];
    int* bestPath = new int[N];
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
    delete[] bestPath;

    return minCost;
}