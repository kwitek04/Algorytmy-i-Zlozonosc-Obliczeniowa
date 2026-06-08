#include "AtspGraph.h"
#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

// Konstruktor
AtspGraph::AtspGraph() {
    N = 0;
    distanceMatrix = nullptr;
}

// Destruktor
AtspGraph::~AtspGraph() {
    freeMemory();
}

// Funkcja zwalniająca pamięć
void AtspGraph::freeMemory() {
    if (distanceMatrix != nullptr) {
        for (int i = 0; i < N; ++i) {
            delete[] distanceMatrix[i];
        }
        delete[] distanceMatrix;
        distanceMatrix = nullptr;
    }
}

// Funkcja do wczytywania danych z pliku
bool AtspGraph::loadFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Blad: Nie mozna otworzyc pliku " << filename << endl;
        return false;
    }

    //zwalniamy pamięć i inicjalizujemy N które będzie przechowywać liczbę miast
    freeMemory();
    N = 0;
    string word;

    while (file >> word) {
        // znalezione pliki atsp z danymi mają różne formaty raz jest DIMENSION: N, a raz jest DIMENSION : N, więc trzeba dobrze obsłużyć odczyt N
        if (word == "DIMENSION:") {
            file >> N;
        }
        else if (word == "DIMENSION") {
            string next_word;
            file >> next_word;             
            if (next_word == ":") {
                file >> N; 
            } else {
                N = stoi(next_word); 
            }
        } 
        else if (word == "EDGE_WEIGHT_SECTION") {
            break; 
        }
    }

    if (N <= 0) {
        cout << "Blad: Nie udalo sie odczytac wymiarow macierzy" << endl;
        file.close();
        return false;
    }

    // alokacja pamięci dla macierzy odległości
    distanceMatrix = new int*[N];
    for (int i = 0; i < N; ++i) {
        distanceMatrix[i] = new int[N];
    }

    // wczytywanie do macierzy odległości danych z pliku
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            file >> distanceMatrix[i][j];
        }
    }
    
    file.close();
    return true;
}

// Funkcja do wyświetlania macierzy
void AtspGraph::displayData() const {
    if (distanceMatrix == nullptr) return;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            cout << distanceMatrix[i][j] << "\t";
        }
        cout << endl;
    }
}

// Funkcja generująca losowy graf o podanym rozmiarze i zapisująca go do pliku
void AtspGraph::generateRandomGraph(int size, const string& filename) {
    if (size <= 0) {
        cout << "Rozmiar grafu musi byc wiekszy od 0" << endl;
        return;
    }

    freeMemory(); 
    N = size;

    // alokacja pamięci
    distanceMatrix = new int*[N];
    for (int i = 0; i < N; ++i) {
        distanceMatrix[i] = new int[N];
    }

    // wypełnianie macierzy losowymi wartościami
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (i == j) {
                distanceMatrix[i][j] = 0; // odległość do samego siebie ustawiana na 0
            } else {
                distanceMatrix[i][j] = rand() % 100 + 1; // losowa odległość z zakresu od 1 do 100
            }
        }
    }

    // zapisywanioe wygenerowanego grafu do pliku
    ofstream file(filename);
    if (file.is_open()) {
        file << "NAME : " << filename << "\n";
        file << "COMMENT : ATSP wygenerowany dla " << N << " miast\n";
        file << "TYPE : ATSP\n";
        file << "DIMENSION : " << N << "\n";
        file << "EDGE_WEIGHT_TYPE : EXPLICIT\n";
        file << "EDGE_WEIGHT_FORMAT : FULL_MATRIX\n";
        file << "EDGE_WEIGHT_SECTION\n";

        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                file << distanceMatrix[i][j] << " ";
            }
            file << "\n";
        }
        file.close();
        cout << "Wygenerowano losowy graf o rozmiarze " << N << "x" << N << " i zapisano do pliku: " << filename << "\n";
    } else {
        cout << "Blad: Nie udalo sie utworzyc pliku do zapisu!" << endl;
    }
}