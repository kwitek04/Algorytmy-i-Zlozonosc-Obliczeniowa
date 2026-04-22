#include <iostream>
#include "AtspGraph.h"
#include "BruteForce.h"
#include "NearestNeighbor.h"
#include "RepetitiveNearestNeighbor.h"
#include "RandomSearch.h"
#include <string> // Biblioteka do obsługi ciągów znaków i nazw plików
#include <chrono>   // Biblioteka potrzebna mierzenia czasu
#include <fstream>  // Biblioteka do tworzenia i zapisu pliku CSV
#include <map>      // Biblioteka do stworzenia mapy najlepszych rozwiązań dla konkretnego pliku wejściowego

// Biblioteki potrzebne do generowania liczb losowych
#include <cstdlib> 
#include <ctime>   

// Struktura grupująca ścieżkę do pliku i rozmiar grafu
struct TestInstance {
    std::string filename;
    int n;
};

using namespace std;
using namespace std::chrono;

// Funkcja wyodrębniająca czystą nazwę pliku ze ścieżki (bez folderów i rozszerzenia)
string extractInstanceName(const string& filepath) {
    // szukamy ostatniego ukośnika
    size_t slash = filepath.find_last_of("/\\");
    string filename;

    // jeśli nie znaleziono ukośnika, cała ścieżka to po prostu nazwa pliku
    if (slash == string::npos) {
        filename = filepath;
    } 
    // w przeciwnym razie wycinamy wszystko to, co jest po ostatnim ukośniku
    else {
        filename = filepath.substr(slash + 1);
    }

    // szukamy ostatniej kropki
    size_t dot = filename.find_last_of('.');

    // jeśli nie ma kropki, to zwracamy nazwę w całości
    if (dot == string::npos) {
        return filename;
    } 
    // w przeciwnym razie zwracamy tylko tekst od początku do miejsca kropki
    else {
        return filename.substr(0, dot);
    }
}

// Funkcja wczytująca z pliku optymalne wyniki do mapy w celu późniejszego porównania
map<string, int> loadBestSolutions(const string& filepath) {
    map<string, int> solutions;
    ifstream file(filepath);
    if (!file.is_open()) return solutions;
    
    string name;
    int cost;
    while (file >> name >> cost) {
        if (!name.empty() && name.back() == ':') {
            name.pop_back(); 
        }
        solutions[name] = cost;
    }
    return solutions;
}

int main() {
    // Inicjalizacja generatora liczb pseudolosowych
    srand(time(NULL)); 

    // Główny obiekt przechowujący macierz i wyniki
    AtspGraph graph;
    int choice = -1;

    // Wczytanie optymalnych wyników
    map<string, int> bestSolutions = loadBestSolutions("data/bestSolutions.txt");
    
    while (choice != 0) {
        cout << "1. Wczytaj dane z pliku" << endl;
        cout << "2. Wyswietl macierz" << endl;
        cout << "3. Uruchom algorytm" << endl;
        cout << "4. Wygeneruj losowy graf" << endl;
        cout << "5. Testy wydajnosciowe" << endl;
        cout << "0. Wyjdz z programu" << endl;
        cout << "Wybierz opcje: ";
        cin >> choice;

        // switch realizujący wybór użytkownika z menu
        switch (choice) {
            case 1: {
                string filename;
                cout << "Podaj nazwe pliku: ";
                cin >> filename;
                if (graph.loadFromFile(filename)) {
                    cout << "Plik wczytany pomyslnie!" << endl;
                }
                break;
            }
            case 2:
                graph.displayData();
                break;
            case 3: {
                int algChoice;
                cout << "\nWybierz algorytm: " << endl;
                cout << "1. Przeglad zupelny (Brute force)" << endl;
                cout << "2. Nearest Neighbour (NN)" << endl;
                cout << "3. Repetitive Nearest Neighbour (RNN)" << endl;
                cout << "4. Losowy (RAND)" << endl;
                cout << "5. Uruchom wszystkie algorytmy" << endl;
                cout << "Wybierz algorytm: ";
                cin >> algChoice;

                if (algChoice == 1) {
                    runBruteForce(graph.getMatrix(), graph.getSize());
                } 
                else if (algChoice == 2) {
                    int startCity;
                    cout << "Podaj miasto startowe: "; 
                    cin >> startCity;
                    runNearestNeighbor(graph.getMatrix(), graph.getSize(), startCity);
                } 
                else if (algChoice == 3) {
                    runRepetitiveNearestNeighbor(graph.getMatrix(), graph.getSize());
                } 
                else if (algChoice == 4) {
                    int iterations;
                    cout << "Podaj liczbe losowan: "; // algorytmowi loswemu trzeba podać liczbę iteracji
                    cin >> iterations;
                    runRandom(graph.getMatrix(), graph.getSize(), iterations);
                }
                else if (algChoice == 5) {
                    int iterations;
                    cout << "Podaj liczbe losowan dla algorytmu loswego: "; // algorytmowi loswemu trzeba podać liczbę iteracji
                    cin >> iterations;
                    
                    cout << "\nWyniki: " << endl;
                    runBruteForce(graph.getMatrix(), graph.getSize());
                    runNearestNeighbor(graph.getMatrix(), graph.getSize(), 0); // algorytm NN uruchamiamy domyślnie z miastem startopwym 0
                    runRepetitiveNearestNeighbor(graph.getMatrix(), graph.getSize());
                    runRandom(graph.getMatrix(), graph.getSize(), iterations);
                }
                else {
                    cout << "Nieprawidlowy wybor algorytmu!" << endl;
                }
                break;
            }
            case 4: {
                int size;
                string filename;
                cout << "Podaj liczbe miast dla nowego grafu: ";
                cin >> size;
                cout << "Podaj nazwe pliku do zapisu: ";
                cin >> filename;
                graph.generateRandomGraph(size, filename);
                break;
            }
            case 5: {
                cout << "\nTesty:" << endl;
                
                // testy dla algorytmu brute force
                TestInstance bf_tests[] = {
                    {"data/test2.atsp", 2}, {"data/test4.atsp", 4}, {"data/test6.atsp", 6}, 
                    {"data/test8.atsp", 8}, {"data/test10.atsp", 10}, {"data/test12.atsp", 12}, {"data/test14.atsp", 14}
                };
                int bf_size = 7;

                // testy dla pozostałych algorytmów
                TestInstance approx_tests[] = {
                    {"data/test6.atsp", 6}, {"data/test10.atsp", 10}, {"data/test14.atsp", 14},
                    {"data/ftv33.atsp", 33}, {"data/ftv64.atsp", 64}, 
                    {"data/kro124p.atsp", 124}, {"data/ftv170.atsp", 170}, {"data/rbg358.atsp", 358}
                };
                int approx_size = 8;

                int algTest;
                cout << "Wybierz algorytm do testowania:" << endl;
                cout << "1. Brute Force" << endl;
                cout << "2. Nearest Neighbour" << endl;
                cout << "3. Repetitive NN" << endl;
                cout << "4. Losowy RAND" << endl;
                cout << "Wybierz (1-4): ";
                cin >> algTest;

                int randIterations = 10000;
                if (algTest == 4) {
                    cout << "Podaj liczbe iteracji dla algorytmu RAND: ";
                    cin >> randIterations;
                }

                // otwieramy plik csv w trypie do zapisu, z flaga append żeby nie kasować jego obecnej zawartości
                ofstream csv("wyniki.csv", ios::app);
                if (!csv.is_open()) {
                    cout << "Blad! Nie mozna utworzyc pliku CSV." << endl;
                    break;
                }
                
                // Przesuwamy kursosr na koniec a nastepnie sprawdzamy czy jesteśmy mimo to w pozycji początkowej. 
                // Jeśli jesteśmy to znaczy że to nowo utowrzony plik więc dopisujemy nazwy kolumn
                csv.seekp(0, ios::end);
                if (csv.tellp() == 0) {
                    csv << "Algorytm;N;Iteracje(algorytm RAND);Sredni_Czas_ms;Sredni_Koszt;Optymalny_Koszt;Blad_Wzgledny_%\n";
                }

                // Wskaźnik na wybraną tablicę z danymi testowymi
                TestInstance* selected_tests;
                int selected_size;
                string algName;

                // Przypisanie odpowiedniego zestawu testowego w zależności od algorytmu
                if (algTest == 1) {
                    selected_tests = bf_tests;
                    selected_size = bf_size;
                    algName = "BF";
                } else {
                    selected_tests = approx_tests;
                    selected_size = approx_size;
                    
                    if (algTest == 2) algName = "NN";
                    else if (algTest == 3) algName = "RNN";
                    else if (algTest == 4) algName = "RAND";
                }

                cout << "\nRozpoczynam pomiary...\n";

                // Pętla testująca                
                for (int i = 0; i < selected_size; ++i) {
                    TestInstance test = selected_tests[i];

                    // zabezpieczenie na wypadek usunięcia pliku z folderu data
                    if (!graph.loadFromFile(test.filename)) {
                        cout << "Pominieto test dla N=" << test.n << " (Brak pliku: " << test.filename << ")\n";
                        continue;
                    }

                    // domyślna liczba powtórzeń z których są następnie uśredniane wyniki to 100
                    int repetitions = 100; 
                    if (algTest == 1){
                        if(test.n >= 14){
                            repetitions = 5; // dla brute force gdy n>=14 ustawiamy liczbe powtórzeń na 5, ze wzgledu na długi czas trwania jednego powtórzenia
                        }
                    }

                    // Odczytujemy z mapy optymalne rozwiązanie dla aktualnego pliku
                    string instanceName = extractInstanceName(test.filename);
                    int optimumCost = -1;
                    if (bestSolutions.find(instanceName) != bestSolutions.end()) {
                        optimumCost = bestSolutions[instanceName];
                    }

                    cout << "Badam " << algName << " dla N=" << test.n << " (" << repetitions << " prob)... ";
                    
                    double totalTime = 0.0;
                    long long totalCostSum = 0;

                    for (int r = 0; r < repetitions; r++) {
                        auto start = high_resolution_clock::now(); // pozycja startowa pomiaru czasu
                        int currentCost = 0;
                        
                        if (algTest == 1) currentCost = runBruteForce(graph.getMatrix(), graph.getSize());
                        else if (algTest == 2) currentCost = runNearestNeighbor(graph.getMatrix(), graph.getSize(), 0);
                        else if (algTest == 3) currentCost = runRepetitiveNearestNeighbor(graph.getMatrix(), graph.getSize());
                        else if (algTest == 4) currentCost = runRandom(graph.getMatrix(), graph.getSize(), randIterations); 
                        
                        auto end = high_resolution_clock::now(); // pozycja końcowa pomiaru czasu
                        totalTime += duration<double, milli>(end - start).count(); // dodajemy do łącznego czasu różnicę końca i startu
                        totalCostSum += currentCost; // sumujemy koszt ścieżki w każdym powtórzeniu
                    }

                    // Uśredniamy wyniki licząc średnią arytmetyczną 
                    double avgTime = totalTime / repetitions;
                    double avgCost = (double)totalCostSum / repetitions;

                    string errorStr = "Brak danych";

                    // Obliczamy błąd względny
                    if (optimumCost > 0) {
                        double relError = ((avgCost - optimumCost) / optimumCost) * 100.0;
                        errorStr = to_string(relError);
                        errorStr = errorStr.substr(0, errorStr.find('.') + 3); // zostawiamy 2 miejsca po przecinku, resztę ucinamy
                    } else if (algTest == 1) {
                        errorStr = "0.00"; // dla brute force błą względny zawsze wynosi 0
                        optimumCost = avgCost; // brute force podaje zawsze optimum
                    }

                    cout << "Sredni czas: " << avgTime << " ms | Koszt: " << avgCost;
                    if (optimumCost > 0) cout << " (Opt: " << optimumCost << " | Blad: " << errorStr << "%)";
                    cout << endl;

                    // Przygotowywanie danych do wppisania w kolumnach iteracji i optymalnej ścieżki w pliku csv
                    int iterToWrite = (algTest == 4) ? randIterations : 0; // iteracje są zapisywane tylko dla RAND, dla pozostałych algorytmów wpisujemy w tej kolumnie 0
                    string optCostStr = (optimumCost > 0) ? to_string(optimumCost) : "Nieznany"; // wpisujemy wartość optymalnej ścieżki jeśli udało się ją odczytać

                    // Zapis do pliku csv
                    csv << algName << ";" << test.n << ";" << iterToWrite << ";" << avgTime << ";" << avgCost << ";" << optCostStr << ";" << errorStr << "\n";
                }

                csv.close();
                cout << "\nTesty zakonczone pomyslnie! Wyniki zapisano do 'wyniki.csv'." << endl;
                break;
            }
            case 0:
                cout << "Zamykanie programu..." << endl;
                break;
            default:
                cout << "Nieznana opcja! Sprobuj ponownie." << endl;
                break;
        }
    }
    return 0;
}