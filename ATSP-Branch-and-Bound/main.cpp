#include <iostream>
#include <string>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <ctime>

#include "AtspGraph.h" 
#include "BranchAndBound.h"

using namespace std;

// Funkcja realizujaca testy automatyczne dla wybranego wariantu algorytmu
void runPerformanceTests(int algChoice) {
    const int REPETITIONS = 10; 
    
    // Lista plikow do testow
    string testFiles[] = {
        "data/test6.atsp",
        "data/test10.atsp",
        "data/test14.atsp",
        "data/test18.atsp", 
        "data/test20.atsp",
        "data/test22.atsp",
        "data/test24.atsp", 
        "data/ftv33.atsp",        
        "data/ftv47.atsp",
    };

    string algName = "";
    if (algChoice == 1) algName = "Best-First";
    else if (algChoice == 2) algName = "DFS";
    else if (algChoice == 3) algName = "BFS";

    string outFileName = "wyniki_" + algName + ".csv";
    ofstream file(outFileName);
    
    if (!file.is_open()) {
        cout << "Nie udalo sie utworzyc pliku " << outFileName << "\n";
        return;
    }

    file << "Plik;N;Algorytm;Sredni Czas [ms]\n";
    cout << "\nBadania dla: " << algName << " (10 powtorzen)\n";

    for (const string& filename : testFiles) {
        AtspGraph graph; 
        if (!graph.loadFromFile(filename)) {
            cout << "Pominiecie pliku: " << filename << "\n";
            continue;
        }

        int** matrix = graph.getMatrix();
        int N = graph.getSize();

        // Zabezpieczenie dla bfs przed wyczerpaniem pamieci ram
        if (algChoice == 3 && N >= 25) {
            cout << "Pominiecie " << filename << " (N=" << N << " za duze dla BFS)\n";
            file << filename << ";" << N << ";" << algName << ";-1.00\n";
            continue;
        }

        cout << "Badanie: " << filename << " (N=" << N << ")...\n";
        double totalTime = 0;

        for (int i = 0; i < REPETITIONS; i++) {
            BranchAndBound solver(N);
            auto start = chrono::high_resolution_clock::now();
            
            if (algChoice == 1) solver.solveLowestCost(matrix);
            else if (algChoice == 2) solver.solveDFS(matrix);
            else if (algChoice == 3) solver.solveBFS(matrix);
            
            auto end = chrono::high_resolution_clock::now();
            
            chrono::duration<double, std::milli> elapsed = end - start;
            totalTime += elapsed.count();
        }
        
        double avgTime = totalTime / REPETITIONS;
        cout << fixed << setprecision(2) << "Srednia: " << avgTime << " ms\n";
        file << filename << ";" << N << ";" << algName << ";" << fixed << setprecision(2) << avgTime << "\n";
    }

    file.close();
    cout << "Wyniki zapisano w: " << outFileName << "\n";
}

int main() {
    // Inicjalizacja ziarna losowosci dla generatora grafow
    srand(time(NULL));

    AtspGraph graph; 
    int choice = -1;

    while (choice != 0) {
        cout << "\nMenu:\n";
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
                if (graph.loadFromFile(filename)) cout << "Wczytano N = " << graph.getSize() << "\n";
                break;
            }
            case 2:
                if (graph.getSize() > 0) graph.displayData();
                else cout << "Najpierw wczytaj dane!\n";
                break;
            case 3: {
                if (graph.getSize() == 0) { cout << "Brak danych!\n"; break; }
                int alg;
                cout << "1. Best-First, 2. DFS, 3. BFS: ";
                cin >> alg;
                
                if (alg < 1 || alg > 3) {
                    cout << "Bledny wybor!\n";
                    break;
                }

                int** matrix = graph.getMatrix();
                BranchAndBound solver(graph.getSize());
                auto start = chrono::high_resolution_clock::now();
                
                if (alg == 1) solver.solveLowestCost(matrix);
                else if (alg == 2) solver.solveDFS(matrix);
                else if (alg == 3) solver.solveBFS(matrix);
                
                auto end = chrono::high_resolution_clock::now();
                solver.displayResult();
                
                // Czas wyswietlamy z dokladnoscia do dwoch miejsc po przecinku
                chrono::duration<double, std::milli> elapsed = end - start;
                cout << "Czas: " << fixed << setprecision(2) << elapsed.count() << " ms\n";
                break;
            }
            case 4: {
                int alg;
                cout << "Wybierz algorytm:\n";
                cout << "1. Best-First\n2. DFS\n3. BFS\nWybierz: ";
                cin >> alg;
                if (alg >= 1 && alg <= 3) runPerformanceTests(alg);
                else cout << "Bledny wybor!\n";
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
            case 0: break;
        }
    }
    return 0;
}