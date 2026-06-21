#include "GeneticAlgorithm.h"
#include <iostream>

// Konstruktor
GeneticAlgorithm::GeneticAlgorithm(int** m, int size) {
    N = size;
    matrix = m;
    bestPath = new int[N];
    bestCost = INT_MAX;
    population = nullptr;
    newPopulation = nullptr;
}

// Destruktor
GeneticAlgorithm::~GeneticAlgorithm() {
    delete[] bestPath;
}

// Funkcja licząca koszt ścieżki
int GeneticAlgorithm::calculateCost(int* path) {
    int cost = 0;
    for (int i = 0; i < N - 1; i++) {
        cost += matrix[path[i]][path[i + 1]];
    }
    cost += matrix[path[N - 1]][path[0]];
    return cost;
}

// Tworzenie losowego osobnika
void GeneticAlgorithm::generateRandomSolution(int* path) {
    for (int i = 0; i < N; i++) path[i] = i;
    for (int i = N - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        std::swap(path[i], path[j]);
    }
}

// Mutacja: SWAP
void GeneticAlgorithm::getNeighborSwap(int* currentPath, int* neighborPath) {
    for (int i = 0; i < N; i++) neighborPath[i] = currentPath[i];
    int i = rand() % N;
    int j = rand() % N;
    while (i == j) j = rand() % N;
    std::swap(neighborPath[i], neighborPath[j]);
}

// Mutacja: INVERT
void GeneticAlgorithm::getNeighborInvert(int* currentPath, int* neighborPath) {
    for (int i = 0; i < N; i++) neighborPath[i] = currentPath[i];
    int i = rand() % N;
    int j = rand() % N;
    if (i > j) std::swap(i, j);
    while (i < j) {
        std::swap(neighborPath[i], neighborPath[j]);
        i++; j--;
    }
}

// Mutacja: INSERT
void GeneticAlgorithm::getNeighborInsert(int* currentPath, int* neighborPath) {
    for (int k = 0; k < N; k++) neighborPath[k] = currentPath[k];
    int i = rand() % N;
    int j = rand() % N;
    while (i == j) j = rand() % N;
    int cityToMove = neighborPath[i];
    
    if (i < j) {
        for (int k = i; k < j; k++) neighborPath[k] = neighborPath[k + 1];
    } else {
        for (int k = i; k > j; k--) neighborPath[k] = neighborPath[k - 1];
    }
    neighborPath[j] = cityToMove;
}

// Inicjalizacja populacji startowej
void GeneticAlgorithm::initializePopulation() {
    for (int i = 0; i < populationSize; i++) {
        generateRandomSolution(population[i].path);
        population[i].cost = calculateCost(population[i].path);
        
        // Zapisz najlepszego z populacji
        if (population[i].cost < bestCost) {
            bestCost = population[i].cost;
            for (int j = 0; j < N; j++) bestPath[j] = population[i].path[j];
        }
    }
}

// Selekcja turniejowa
Individual GeneticAlgorithm::tournamentSelection(int tournamentSize) {
    int bestIdx = rand() % populationSize;
    int minCost = population[bestIdx].cost;

    for (int i = 1; i < tournamentSize; i++) {
        int randomIdx = rand() % populationSize;
        if (population[randomIdx].cost < minCost) {
            minCost = population[randomIdx].cost;
            bestIdx = randomIdx;
        }
    }
    return population[bestIdx];
}

// Krzyżowanie: Order Crossover (OX)
void GeneticAlgorithm::orderCrossover(int* parent1, int* parent2, int* child) {
    int startPos = rand() % N;
    int endPos = rand() % N;
    if (startPos > endPos) std::swap(startPos, endPos);

    bool* inChild = new bool[N]();
    
    // Skopiuj srodek z Rodzica 1
    for (int i = startPos; i <= endPos; i++) {
        child[i] = parent1[i];
        inChild[parent1[i]] = true;
    }

    // Uzupelnij reszte genami z Rodzica 2, zachowujac ich kolejnosc
    int currPos = (endPos + 1) % N;
    int parent2Pos = (endPos + 1) % N;

    while (currPos != startPos) {
        if (!inChild[parent2[parent2Pos]]) {
            child[currPos] = parent2[parent2Pos];
            currPos = (currPos + 1) % N;
        }
        parent2Pos = (parent2Pos + 1) % N;
    }
    
    delete[] inChild;
}

// Zbiorcza funkcja mutacji
void GeneticAlgorithm::mutate(int* path) {
    int* tempPath = new int[N];
    if (mutationType == 1) getNeighborSwap(path, tempPath);
    else if (mutationType == 2) getNeighborInvert(path, tempPath);
    else getNeighborInsert(path, tempPath);
    
    for (int i = 0; i < N; i++) path[i] = tempPath[i];
    delete[] tempPath;
}

// Glowna petla algorytmu
int* GeneticAlgorithm::solve(int popSize, double crossRate, double mutRate, int mutMethod, double timeLimitSeconds) {
    populationSize = popSize;
    crossoverRate = crossRate;
    mutationRate = mutRate;
    mutationType = mutMethod;

    // Alokacja pamieci dla dwoch populacji
    population = new Individual[populationSize];
    newPopulation = new Individual[populationSize];
    for (int i = 0; i < populationSize; i++) {
        population[i].path = new int[N];
        newPopulation[i].path = new int[N];
    }

    initializePopulation();
    auto startTime = std::chrono::high_resolution_clock::now();
    int tournamentSize = 5;
    while (true) {
        // Kryterium stopu oparte na czasie
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = currentTime - startTime;
        if (elapsed.count() >= timeLimitSeconds) break;

        // Zachowujemy najlepszego osobnika z poprzedniego pokolenia
        int currentBestIdx = 0;
        for (int i = 1; i < populationSize; i++) {
            if (population[i].cost < population[currentBestIdx].cost) currentBestIdx = i;
        }
        for (int j = 0; j < N; j++) newPopulation[0].path[j] = population[currentBestIdx].path[j];
        newPopulation[0].cost = population[currentBestIdx].cost;

        // Generowanie reszty nowej populacji
        for (int i = 1; i < populationSize; i++) {
            Individual parent1 = tournamentSelection(tournamentSize);
            
            // Prawdopodobienstwo krzyzowania
            double r = (double)rand() / RAND_MAX;
            if (r < crossoverRate) {
                Individual parent2 = tournamentSelection(tournamentSize);
                orderCrossover(parent1.path, parent2.path, newPopulation[i].path);
            } else {
                // Jesli nie ma krzyzowania, dziecko jest kopia rodzica
                for (int j = 0; j < N; j++) newPopulation[i].path[j] = parent1.path[j];
            }

            // Prawdopodobienstwo mutacji
            double rm = (double)rand() / RAND_MAX;
            if (rm < mutationRate) {
                mutate(newPopulation[i].path);
            }

            // Ocena nowego osobnika
            newPopulation[i].cost = calculateCost(newPopulation[i].path);

            // Aktualizacja globalnego optimum
            if (newPopulation[i].cost < bestCost) {
                bestCost = newPopulation[i].cost;
                for (int j = 0; j < N; j++) bestPath[j] = newPopulation[i].path[j];
            }
        }

        // Zastapienie starej populacji nowa
        std::swap(population, newPopulation);
    }

    // Czyszczenie pamieci wewnetrznej
    for (int i = 0; i < populationSize; i++) {
        delete[] population[i].path;
        delete[] newPopulation[i].path;
    }
    delete[] population;
    delete[] newPopulation;

    return bestPath;
}

int GeneticAlgorithm::getBestCost() {
    return bestCost;
}