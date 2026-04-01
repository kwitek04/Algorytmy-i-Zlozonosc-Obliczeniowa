#ifndef ATSPPROBLEM_H
#define ATSPPROBLEM_H

#include <string>

class AtspProblem {
private:
    int N;                  
    int** distanceMatrix;   
    
    int* bestPath;         
    int minCost;            

    void freeMemory();
    int calculatePathCost(int* path);
    void bruteForceHelper(int* currentPath, int step, bool* visited);

public:
    AtspProblem();
    ~AtspProblem();

    bool loadFromFile(const std::string& filename);
    void displayData() const;
    void generateRandomGraph(int size, const std::string& filename);

    int runBruteForce();
    int runNearestNeighbor(int startCity); 
    int runRepetitiveNearestNeighbor();
    int runRandom(int iterations);
};

#endif