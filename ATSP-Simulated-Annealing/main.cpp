#include <iostream>
#include <string>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <map>     // do obslugi slownika z optymalnymi wynikami
#include <sstream> // do parsowania pliku tekstowego

#include "AtspGraph.h" 
#include "SimulatedAnnealing.h"

using namespace std;

// Funkcja ladujaca optymalne rozwiazania z pliku tekstowego do struktury mapy
map<string, int> loadOptimalSolutions(const string& filepath) {
    map<string, int> solutions;
    ifstream file(filepath);
    if (!file.is_open()) {
        cout << "Nie udalo sie otworzyc pliku z optymalnymi wynikami (" << filepath << "). Blad wzgledny nie bedzie liczony!\n";
        return solutions;
    }
    
    string line;
    while (getline(file, line)) {
        size_t colonPos = line.find(':');
        if (colonPos != string::npos) {
            string name = line.substr(0, colonPos);
            int cost = stoi(line.substr(colonPos + 1));
            solutions[name] = cost;
        }
    }
    return solutions;
}

// Funkcja pomocnicza do wyciagania samej nazwy ze sciezki
string extractInstanceName(const string& filepath) {
    string name = filepath;
    size_t slashPos = name.find_last_of("/\\");
    if (slashPos != string::npos) name = name.substr(slashPos + 1);
    size_t dotPos = name.find_last_of('.');
    if (dotPos != string::npos) name = name.substr(0, dotPos);
    return name;
}

// Funkcja realizujaca testy automatyczne dla Symulowanego Wyzarzania
void runPerformanceTests(double T, double coolingRate, int epoch, int scheme, int initMethod, int neighborMethod) {
    const int REPETITIONS = 10; 
    
    // Lista instancji testowych
    string testFiles[] = {
        "data/test6.atsp",
        "data/test14.atsp",
        "data/test24.atsp", 
        "data/ftv33.atsp",        
        "data/ftv64.atsp",
        "data/kro124p.atsp",
        "data/ftv170.atsp",
        "data/rbg358.atsp"
    };

    // Wczytanie slownika optimum
    map<string, int> optimalSolutions = loadOptimalSolutions("data/bestSolutions.txt");

    string outFileName = "wyniki_SW.csv";
    ofstream file(outFileName);
    
    if (!file.is_open()) {
        cout << "Nie udalo sie utworzyc pliku " << outFileName << "\n";
        return;
    }

    file << "Plik;N;Sredni Czas [ms];Optimum;Sredni Koszt;Blad Wzgledny [%]\n";
    cout << "\nRozpoczynanie badan automatycznych (10 powtorzen dla kazdego pliku)...\n";

    for (const string& filename : testFiles) {
        AtspGraph graph; 
        if (!graph.loadFromFile(filename)) {
            cout << "Pominiecie pliku: " << filename << "\n";
            continue;
        }

        int** matrix = graph.getMatrix();
        int N = graph.getSize();

        // Odszukanie optymalnego wyniku w slowniku
        string instanceName = extractInstanceName(filename);
        int optCost = optimalSolutions.count(instanceName) ? optimalSolutions[instanceName] : -1;

        cout << "Badanie: " << filename << " (N=" << N << ")...\n";
        
        double totalTime = 0;
        long long totalCost = 0;

        for (int i = 0; i < REPETITIONS; i++) {
            SimulatedAnnealing sw(matrix, N);
            
            auto start = chrono::high_resolution_clock::now();
            sw.solve(T, coolingRate, epoch, scheme, initMethod, neighborMethod);
            auto end = chrono::high_resolution_clock::now();
            
            chrono::duration<double, std::milli> elapsed = end - start;
            totalTime += elapsed.count();
            totalCost += sw.getBestCost();
        }
        
        double avgTime = totalTime / REPETITIONS;
        double avgCost = (double)totalCost / REPETITIONS;
        
        // Obliczanie bledu wzglednego
        double error = -1.0;
        if (optCost != -1) {
            error = ((avgCost - optCost) / (double)optCost) * 100.0;
        }
        
        cout << fixed << setprecision(2) << "Czas: " << avgTime << " ms | Koszt: " << avgCost;
        if (optCost != -1) cout << " | Blad: " << error << " %";
        cout << "\n";
        
        file << filename << ";" << N << ";" << fixed << setprecision(2) << avgTime << ";" 
             << optCost << ";" << avgCost << ";" << error << "\n";
    }

    file.close();
    cout << "\nWyniki zapisano w: " << outFileName << "\n";
}

int main() {
    srand(time(NULL));
    AtspGraph graph; 
    int choice = -1;

    while (choice != 0) {
        cout << "\nWybierz opcje:\n";
        cout << "1. Wczytaj dane z pliku\n";
        cout << "2. Wyswietl macierz\n";
        cout << "3. Uruchom algorytm\n";
        cout << "4. Badania automatyczne\n";
        cout << "5. Wygeneruj losowy graf\n";
        cout << "0. Wyjdz z programu\n";
        cout << "Wybierz opcje: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                string filename;
                cout << "Podaj nazwe pliku: ";
                cin >> filename;
                if (graph.loadFromFile(filename)) {
                    cout << "Wczytano N = " << graph.getSize() << "\n";
                } 
                break;
            }
            case 2:
                if (graph.getSize() > 0) {
                    graph.displayData();
                } 
                else cout << "Najpierw wczytaj dane!\n";
                break;

            case 3: {
                if (graph.getSize() == 0) { 
                    cout << "Brak danych!\n"; 
                    break; 
                }
                
                double T, coolingRate;
                int epoch, scheme, initMethod, neighborMethod;

                cout << "\nKonfiguracja Symulowanego Wyzarzania\n";
                cout << "Temperatura poczatkowa (np. 1000): "; cin >> T;
                cout << "Wspolczynnik chlodzenia (np. 0.99): "; cin >> coolingRate;
                cout << "Dlugosc epoki (np. 100): "; cin >> epoch;
                cout << "Schemat schladzania (1 - Geometryczny, 2 - Liniowy): "; cin >> scheme;
                cout << "Rozwiazanie poczatkowe (1 - Losowe, 2 - NN): "; cin >> initMethod;
                cout << "Generowanie sasiedztwa (1 - Swap, 2 - Invert, 3 - Insert): "; cin >> neighborMethod;
                
                int** matrix = graph.getMatrix();
                int N = graph.getSize();
                SimulatedAnnealing sw(matrix, N);
                
                auto start = chrono::high_resolution_clock::now();
                int* resultPath = sw.solve(T, coolingRate, epoch, scheme, initMethod, neighborMethod);
                auto end = chrono::high_resolution_clock::now();
                int finalCost = sw.getBestCost();
                
                cout << "\nWyniki:" << "\n";
                cout << "Najlepszy koszt: " << finalCost << "\n";
                cout << "Najlepsza sciezka: ";
                for (int i = 0; i < N; i++) {
                    cout << resultPath[i] << " -> ";
                }
                cout << resultPath[0] << "\n";
                
                chrono::duration<double, std::milli> elapsed = end - start;
                cout << "Czas obliczen: " << fixed << setprecision(2) << elapsed.count() << " ms\n";
                break;
            }
            case 4: {
                double T, coolingRate;
                int epoch, scheme, initMethod, neighborMethod;

                cout << "\nParametry do zautomatyzowanych badan:\n";
                cout << "Temperatura poczatkowa: "; cin >> T;
                cout << "Wspolczynnik chlodzenia: "; cin >> coolingRate;
                cout << "Dlugosc epoki: "; cin >> epoch;
                cout << "Schemat schladzania (1 - Geometryczny, 2 - Liniowy): "; cin >> scheme;
                cout << "Rozwiazanie poczatkowe (1 - Losowe, 2 - NN): "; cin >> initMethod;
                cout << "Generowanie sasiedztwa (1 - Swap, 2 - Invert, 3 - Insert): "; cin >> neighborMethod;
                runPerformanceTests(T, coolingRate, epoch, scheme, initMethod, neighborMethod);
                break;
            }
            case 5: {
                int size;
                string filename;
                cout << "Podaj ilosc miast: ";
                cin >> size;
                cout << "Podaj nazwe pliku do zapisu: ";
                cin >> filename;
                graph.generateRandomGraph(size, filename);
                break;
            }
            case 0: cout << "Koniec programu.\n"; break;
            default: cout << "Nieznana opcja!\n"; break;
        }
    }
    return 0;
}