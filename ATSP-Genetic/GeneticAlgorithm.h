#pragma once
#include <vector>
#include <climits>
#include <chrono>
#include <algorithm>

// Struktura reprezentująca pojedynczego osobnika w populacji
struct Individual {
    int* path;
    int cost;
};

class GeneticAlgorithm {
private:
    int** matrix;
    int N;
    
    // Parametry algorytmu
    int populationSize;
    double crossoverRate;
    double mutationRate;
    int mutationType; // 1: Swap, 2: Invert, 3: Insert

    Individual* population;
    Individual* newPopulation;
    
    int* bestPath;
    int bestCost;

    int calculateCost(int* path);
    void generateRandomSolution(int* path);
    void getNeighborSwap(int* currentPath, int* neighborPath);
    void getNeighborInvert(int* currentPath, int* neighborPath);
    void getNeighborInsert(int* currentPath, int* neighborPath);
    void initializePopulation();
    Individual tournamentSelection(int tournamentSize);
    void orderCrossover(int* parent1, int* parent2, int* child);
    void mutate(int* path);

public:
    GeneticAlgorithm(int** m, int size);
    ~GeneticAlgorithm();

    // Główna funkcja uruchamiająca algorytm (kryterium stopu to czas w sekundach)
    int* solve(int popSize, double crossRate, double mutRate, int mutMethod, double timeLimitSeconds);
    int getBestCost();
};