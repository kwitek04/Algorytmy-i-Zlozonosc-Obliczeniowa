#pragma once
#include <string>

class AtspGraph {
private:
    int N;                  
    int** distanceMatrix;   

    void freeMemory();

public:
    AtspGraph();
    ~AtspGraph();

    bool loadFromFile(const std::string& filename);
    void displayData() const;
    void generateRandomGraph(int size, const std::string& filename);

    int** getMatrix() const { return distanceMatrix; }
    int getSize() const { return N; }
};