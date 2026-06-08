#pragma once
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <climits>

class SimulatedAnnealing {
private:
    int N;
    int** matrix;
    
    int* bestPath;
    int bestCost;

    int calculateCost(int* path);
    
    // Metody generowania rozwiązania początkowego
    void generateRandomSolution(int* path);
    void generateGreedySolution(int* path);
    
    // Metody generowania sąsiedztwa
    void getNeighborSwap(int* currentPath, int* neighborPath);
    void getNeighborInvert(int* currentPath, int* neighborPath);
    void getNeighborInsert(int* currentPath, int* neighborPath);

public:
    SimulatedAnnealing(int** m, int size);
    ~SimulatedAnnealing();

    // Główna funkcja algorytmu
    // initialTemp: temperatura początkowa
    // coolingRate: współczynnik chłodzenia
    // epochLength: ile iteracji w jednej epoce
    // coolingScheme: 1 - Geometryczny (T *= alpha), 2 - Arytmetyczny (T -= alpha)
    // initMethod: 1 - Losowy, 2 - Zachłanny
    // neighborMethod: 1 - Swap, 2 - Invert
    int* solve(double initialTemp, double coolingRate, int epochLength, int coolingScheme, int initMethod, int neighborMethod);
    int getBestCost();
    
    void displayResult();
};