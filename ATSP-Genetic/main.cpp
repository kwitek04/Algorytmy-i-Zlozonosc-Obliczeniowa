#include <iostream>
#include <string>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <map>
#include <sstream>

#include "AtspGraph.h"
#include "GeneticAlgorithm.h"

using namespace std;

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

string extractInstanceName(const string& filepath) {
    string name = filepath;
    size_t slashPos = name.find_last_of("/\\");
    if (slashPos != string::npos) name = name.substr(slashPos + 1);
    size_t dotPos = name.find_last_of('.');
    if (dotPos != string::npos) name = name.substr(0, dotPos);
    return name;
}

void runPerformanceTests(int popSize, double crossRate, double mutRate, int mutMethod, double timeLimitSeconds) {
    const int REPETITIONS = 10;

    string testFiles[] = {
        "data/test6.atsp",
        "data/test14.atsp",
        "data/test24.atsp",
        "data/ftv33.atsp",
        "data/ftv64.atsp",
        "data/kro124p.atsp",
        "data/ftv170.atsp",
        "data/rbg358.atsp",
        "data/rbg403.atsp",
        "data/rbg443.atsp"
    };

    map<string, int> optimalSolutions = loadOptimalSolutions("data/bestSolutions.txt");

    string outFileName = "wyniki_GA.csv";
    ofstream file(outFileName, ios::app);

    if (!file.is_open()) {
        cout << "Nie udalo sie utworzyc pliku " << outFileName << "\n";
        return;
    }

    file << "\n--- Test: Pop=" << popSize << ", Cross=" << crossRate << ", Mut=" << mutRate
         << ", Metoda=" << mutMethod << ", Czas=" << timeLimitSeconds << "s ---\n";
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

        string instanceName = extractInstanceName(filename);
        int optCost = optimalSolutions.count(instanceName) ? optimalSolutions[instanceName] : -1;

        cout << "Badanie: " << filename << " (N=" << N << ")...\n";

        double totalTime = 0;
        long long totalCost = 0;

        for (int i = 0; i < REPETITIONS; i++) {
            GeneticAlgorithm ga(matrix, N);

            auto start = chrono::high_resolution_clock::now();
            ga.solve(popSize, crossRate, mutRate, mutMethod, timeLimitSeconds);
            auto end = chrono::high_resolution_clock::now();

            chrono::duration<double, milli> elapsed = end - start;
            totalTime += elapsed.count();
            totalCost += ga.getBestCost();
        }

        double avgTime = totalTime / REPETITIONS;
        double avgCost = (double)totalCost / REPETITIONS;

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
    cout << "\nWyniki dopisano do: " << outFileName << "\n";
}

int main() {
    srand(time(NULL));
    AtspGraph graph;
    int choice = -1;

    while (choice != 0) {
        cout << "\nWybierz opcje:\n";
        cout << "1. Wczytaj dane z pliku\n";
        cout << "2. Wyswietl macierz\n";
        cout << "3. Uruchom algorytm (parametry reczne)\n";
        cout << "4. Uruchom algorytm (parametry domyslne)\n";
        cout << "5. Badania automatyczne\n";
        cout << "6. Wygeneruj losowy graf\n";
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

                int popSize, mutMethod;
                double crossRate, mutRate, timeLimitSeconds;

                cout << "\nKonfiguracja Algorytmu Genetycznego\n";
                cout << "Rozmiar populacji (np. 100): "; cin >> popSize;
                cout << "Wspolczynnik krzyzowania (np. 0.8): "; cin >> crossRate;
                cout << "Wspolczynnik mutacji (np. 0.05): "; cin >> mutRate;
                cout << "Metoda mutacji (1 - Swap, 2 - Invert, 3 - Insert): "; cin >> mutMethod;
                cout << "Kryterium stopu [sekundy] (np. 3.0): "; cin >> timeLimitSeconds;

                int** matrix = graph.getMatrix();
                int N = graph.getSize();
                GeneticAlgorithm ga(matrix, N);

                auto start = chrono::high_resolution_clock::now();
                int* resultPath = ga.solve(popSize, crossRate, mutRate, mutMethod, timeLimitSeconds);
                auto end = chrono::high_resolution_clock::now();
                int finalCost = ga.getBestCost();

                cout << "\nWyniki:\n";
                cout << "Najlepszy koszt: " << finalCost << "\n";
                cout << "Najlepsza sciezka: ";
                for (int i = 0; i < N; i++) {
                    cout << resultPath[i] << " -> ";
                }
                cout << resultPath[0] << "\n";

                chrono::duration<double, milli> elapsed = end - start;
                // cout << "Czas obliczen: " << fixed << setprecision(2) << elapsed.count() << " ms\n";
                break;
            }
            case 4: {
                if (graph.getSize() == 0) {
                    cout << "Brak danych!\n";
                    break;
                }

                int popSize = 100;
                double crossRate = 0.8;
                double mutRate = 0.05;
                int mutMethod = 3; // Insert
                double timeLimitSeconds = 3.0;

                cout << "Parametry domyslne: Pop=" << popSize << ", Pc=" << crossRate
                     << ", Pm=" << mutRate << ", mutacja Insert, czas=" << timeLimitSeconds << "s\n";

                int** matrix = graph.getMatrix();
                int N = graph.getSize();
                GeneticAlgorithm ga(matrix, N);

                auto start = chrono::high_resolution_clock::now();
                int* resultPath = ga.solve(popSize, crossRate, mutRate, mutMethod, timeLimitSeconds);
                auto end = chrono::high_resolution_clock::now();
                int finalCost = ga.getBestCost();

                cout << "\nWyniki:\n";
                cout << "Najlepszy koszt: " << finalCost << "\n";
                cout << "Najlepsza sciezka: ";
                for (int i = 0; i < N; i++) {
                    cout << resultPath[i] << " -> ";
                }
                cout << resultPath[0] << "\n";

                chrono::duration<double, milli> elapsed = end - start;
                // cout << "Czas obliczen: " << fixed << setprecision(2) << elapsed.count() << " ms\n";
                break;
            }
            case 5: {
                int popSize, mutMethod;
                double crossRate, mutRate, timeLimitSeconds;

                cout << "\nParametry do zautomatyzowanych badan:\n";
                cout << "Rozmiar populacji: "; cin >> popSize;
                cout << "Wspolczynnik krzyzowania: "; cin >> crossRate;
                cout << "Wspolczynnik mutacji: "; cin >> mutRate;
                cout << "Metoda mutacji (1 - Swap, 2 - Invert, 3 - Insert): "; cin >> mutMethod;
                cout << "Czas na jedna instancje [sekundy]: "; cin >> timeLimitSeconds;

                runPerformanceTests(popSize, crossRate, mutRate, mutMethod, timeLimitSeconds);
                break;
            }
            case 6: {
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
