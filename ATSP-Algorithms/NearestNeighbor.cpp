#include "NearestNeighbor.h"
#include <iostream>
#include <limits> // Biblioteka używana tylko do INT_MAX

using namespace std;

// Algorytm zachłanny Nearest Neighbour (NN)
int runNearestNeighbor(int** distanceMatrix, int N, int startCity) {
    if (distanceMatrix == nullptr || startCity < 0 || startCity >= N){
        cout << "Bledne dane startowe\n";
        return -1;
    }

    bool* visited = new bool[N];
    for (int i = 0; i < N; ++i) visited[i] = false;

    int* bestPath = new int[N];

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
    delete[] bestPath;

    return totalCost;
}