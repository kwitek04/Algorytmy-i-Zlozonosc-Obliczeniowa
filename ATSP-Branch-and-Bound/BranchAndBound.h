#pragma once
#include "Node.h"
#include <stack>
#include <queue>
#include <iostream>
#include <climits>

// Komparator dla Kolejki Priorytetowej
struct CompareNode {
    bool operator()(const Node* a, const Node* b) const {
        return a->cost > b->cost;
    }
};

class BranchAndBound {
private:
    int N;                   // liczba miast
    int bestCost;            // upper bound
    int* bestPath;           // tablica przechowująca najlepszą trasę
    
    // Funkcja licząca Upper Bound na wejściowej macierzy
    void calculateInitialUpperBound(int** matrix);

public:
    BranchAndBound(int n);
    ~BranchAndBound();

    int solveLowestCost(int** matrix);
    int solveDFS(int** matrix);   
    int solveBFS(int** matrix);

    void displayResult();
};