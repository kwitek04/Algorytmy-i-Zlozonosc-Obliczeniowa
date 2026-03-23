#include <iostream>
#include <fstream> // Biblioteka potrzebna do odczytu danych z pliku
#include <vector> // Biblioteka potrzebna do tworzenia wektorów
#include <queue> // Biblioteka potrzebna do implementacji kolejki priorytetowej
#include <climits> // Biblioteka potrzebna do używania stałej INT_MAX reprezentującej nieskończoność
#include <chrono> // Biblioteka potrzebna do pomiaru czasu wykonania algorytmu
#include <cstdlib> // Biblioteka potrzebna do użycia funkcji rand()
#include <ctime> // Biblioteka potrzebna do użycia funkcji time()
using namespace std;
using namespace chrono;

const int inf = INT_MAX; // Stała zmienna reprezentująca nieskończoność (przypisana maksymalna wartość typu int)

// Inicjalizacja struktur danych
vector<vector<pair<int, int>>> List; // Lista sąsiedztwa
vector<vector<int>> Matrix; // Macierz sąsiedztwa
int vertex_count = 0; // Aktualna liczba wierzchołków

// Funkcja do wstępnej inicjalizacji grafów
void init_graph(int v) {
    vertex_count = v;
    
    List.clear(); // Usunięcie ewentualnych starych danych z listy
    List.resize(v); // Zmiana rozmiaru listy sąsiedztwa na v (liczbę wierzchołków)
    
    Matrix.clear(); // Usunięcie ewentualnych starych danych z macierzy
    Matrix.resize(v); // Ustawienie liczby wierszy macierzy
    for (int i = 0; i < v; i++) {
        Matrix[i].resize(v, -1); // Ustawienie liczby kolumn i inicjalizacja wartością -1 (brak krawędzi)
    }
}

// Funkcja wczytująca graf z pliku podanego przez użytkownika
// Plik powinien zawierać w pierwszym wierszu liczbę krawędzi i liczbę wierzchołków rozdzielone spacją, a następnie pary wierzchołków i wagę krawędzi
void load_from_file(string filename) {
    fstream file; 
    file.open(filename); 
    
    if (!file.is_open()) { 
        cout << "Błąd otwierania pliku!\n";
        return;
    }

    int edge_count; // Zmienna do przechowywania liczby krawędzi
    file >> edge_count >> vertex_count; // Wczytanie liczby krawędzi i wierzchołków z pliku
    init_graph(vertex_count);

    // Pętla uzupełniająca listę sąsiedztwa i macierz sąsiedztwa zgodnie z danymi z pliku
    for (int i = 0; i < edge_count; i++) {
        int from, to, weight;
        file >> from >> to >> weight;
        List[from].push_back({to, weight});
        Matrix[from][to] = weight;
    }

    file.close();
    cout << "Graf wczytany (" << vertex_count << " wierzchołków, " << edge_count << " krawędzi).\n";
}

// Funkcja generująca losowy graf o zadanej liczbie wierzchołków i gęstości
void generate_random_graph(int v, int density) {
    init_graph(v);

    int edges = (density * v * (v - 1)) / 100; // Obliczenie liczby krawędzi na podstawie gęstości

    // Generowanie grafu losowego
    srand(time(0));
    int added = 0;
    while (added < edges) {
        int from = rand() % v; // Losowanie wierzchołka początkowego z zakresu od 0 do v-1
        int to = rand() % v; // Losowanie wierzchołka końcowego z zakresu od 0 do v-1
        if (from != to && Matrix[from][to] == -1) {  // Sprawdzenie, czy krawędź już istnieje
            int weight = rand() % 1000; // Losowanie wagi krawędzi z zakresu od 0 do 999
            List[from].push_back({to, weight}); // Dodanie krawędzi do listy sąsiedztwa
            Matrix[from][to] = weight; // Ustawienie wagi krawędzi w macierzy sąsiedztwa
            added++;
        }
    }

    cout << "Graf wygenerowany (" << v << " wierzchołków, " << edges << " krawędzi, gęstość " << density << "%).\n";
}

// Funkcja służąca do wyświetlania grafu w formie listy sąsiedztwa i macierzy sąsiedztwa
void print_graph() {
    if (vertex_count == 0) { // Sprawdzenie czy graf nie jest pusty
        cout << "Graf jest pusty!\n";
        return;
    }
    
    // Wyświetlanie listy sąsiedztwa
    cout << "Lista sąsiedztwa:\n";
    for (int i = 0; i < vertex_count; i++) {
        cout << i << ": ";
        for (auto edge : List[i]) {
            cout << "(" << edge.first << "," << edge.second << ") ";
        }
        cout << "\n";
    }
    
    // Wyświetlanie macierzy sąsiedztwa
    cout << "Reprezentacja macierzowa:\n";
    for (int i = 0; i < vertex_count; i++) {
        for (int j = 0; j < vertex_count; j++) {
            cout.width(4); // Ustawienie szerokości kolumny na 4 znaki, aby macierz była czytelna
            cout << Matrix[i][j];
        }
        cout << "\n";
    }
}

// Funkcja wykonująca algorytm Dijkstry dla reprezentacji listowej 
void dijkstra_list(int start, vector<int>& dist, double& time) {
    auto start_time = high_resolution_clock::now(); // Start pomiaru czasu

    // Inicjalizacja wektora odległości
    dist.assign(vertex_count, inf); // Ustawienie wszystkich odległości na nieskończoność
    dist[start] = 0; // Odległość do wierzchołka startowego ustawiamy na 0
    
    // Inicjalizacja kolejki priorytetowej przechowującej pary (odległość, wierzchołek)
    priority_queue<pair<int, int>> pq;
    pq.push({0, start});

    while (!pq.empty()) {
        int current_vertex = pq.top().second; // Pobranie wierzchołka z kolejki
        int current_dist = -pq.top().first; // Pobranie aktualnej odległości (ujemna ze względu na specyfikę kolejki)
        pq.pop(); // Usunięcie pary z kolejki

        if (current_dist > dist[current_vertex]) continue; // Sprawdzenie, czy nie znaleziono już krótszej ścieżki

        // Pętla po sąsiadach aktualnego wierzchołka
        for (size_t j = 0; j < List[current_vertex].size(); j++) {
            int v = List[current_vertex][j].first; // Wierzchołek sąsiadujący z aktualnym wierzchołkiem
            int w = List[current_vertex][j].second; // Waga krawędzi do sąsiada
            
            // Sprawdzenie, czy znaleziono krótszą ścieżkę do sąsiada
            if (dist[current_vertex] + w < dist[v]) {
                dist[v] = dist[current_vertex] + w; // Aktualizacja odległości do wierzchołka v
                pq.push({-dist[v], v}); // Dodanie nowej pary do kolejki
            }
        }
    }

    auto end_time = high_resolution_clock::now(); // Koniec pomiaru czasu
    time = duration<double, milli>(end_time - start_time).count(); // Obliczenie czasu wykonania algorytmu w milisekundach
}

// Funkcja wykonująca algorytm Dijkstry dla reprezentacji macierzowej
void dijkstra_matrix(int start, vector<int>& dist, double& time) {
    auto start_time = high_resolution_clock::now(); // Start pomiaru czasu

    // Inicjalizacja wektora odległości
    dist.assign(vertex_count, inf); // Ustawienie wszystkich odległości na nieskończoność
    dist[start] = 0; // Odległość do wierzchołka startowego ustawiamy na 0

    // Inicjalizacja kolejki priorytetowej przechowującej pary (odległość, wierzchołek)
    priority_queue<pair<int, int>> pq;
    pq.push({0, start});

    while (!pq.empty()) {
        int current_vertex = pq.top().second; // Pobranie wierzchołka z kolejki
        int current_dist = -pq.top().first; // Pobranie aktualnej odległości
        pq.pop(); // Usunięcie pary z kolejki

        if (current_dist > dist[current_vertex]) continue; // Sprawdzenie, czy nie znaleziono już krótszej ścieżki

        // Pętla po sąsiadach aktualnego wierzchołka w macierzy
        for (int v = 0; v < vertex_count; v++) {
            int w = Matrix[current_vertex][v]; // Waga krawędzi do sąsiada
            if (w != -1) { // Sprawdzenie, czy istnieje krawędź
                if (dist[current_vertex] + w < dist[v]) {
                    dist[v] = dist[current_vertex] + w; // Aktualizacja odległości do wierzchołka v
                    pq.push({-dist[v], v}); // Dodanie nowej pary do kolejki
                }
            }
        }
    }

    auto end_time = high_resolution_clock::now(); // Koniec pomiaru czasu
    time = duration<double, milli>(end_time - start_time).count(); // Obliczenie czasu wykonania algorytmu w milisekundach
}

// Funkcja testująca poprawność działania algorytmu Dijkstry
void run_dijkstra() {
    // Sprawdzenie, czy graf jest pusty
    if (vertex_count == 0) {
        cout << "Graf jest pusty!\n";
        return;
    }

    // Wczytanie wierzchołków startowego i końcowego od użytkownika
    int start, end;
    cout << "Podaj wierzcholek startowy i koncowy (0-" << vertex_count-1 << "): ";
    cin >> start >> end;

    // Sprawdzenie poprawności wprowadzonych wierzchołków
    if (start < 0 || start >= vertex_count || end < 0 || end >= vertex_count) {
        cout << "Nieprawidlowe wierzcholki!\n";
        return;
    }

    // Inicjalizacja wektorów do przechowywania odległości, inicjalizacja zmiennych do pomiaru czasu
    vector<int> dist_list, dist_matrix;
    double time_list, time_matrix;

    // Uruchomienie algorytmu Dijkstry dla obu reprezentacji grafu
    dijkstra_list(start, dist_list, time_list);
    dijkstra_matrix(start, dist_matrix, time_matrix);

    cout << "Wyniki:\n";
    cout << "Lista sąsiedztwa: " << dist_list[end] << " (czas: " << time_list << " ms)\n";
    cout << "Macierz sąsiedztwa: " << dist_matrix[end] << " (czas: " << time_matrix << " ms)\n";

    // Sprawdzenie zgodności wyników
    if (dist_list[end] == dist_matrix[end]) {
        cout << "Wyniki zgodne\n";
    } else {
        cout << "Błąd, rózne wyniki\n";
    }
}

// Funkcja do przeprowadzania testów algorytmu Dijkstry dla różnych gęstości i liczby wierzchołków
void run_test() {
    const int repetitions = 20; // Stała liczba powtórzeń testów dla uśrednienia wyników    
    for (int n = 10; n <= 1000; n *= 10) {
        for (int g = 25; g < 100; g += 25) {
            double total_matrix_time = 0;
            double total_list_time = 0;
            int m = (1.0 * g / 100 * n * (n - 1)); // Obliczanie liczby krawędzi ze wzoru
            
            cout << "Test dla n=" << n << ", g=" << g << "%, m=" << m << ":\n";
            
            for (int rep = 0; rep < repetitions; rep++) {
                generate_random_graph(n, g);
                
                // Losowe wierzchołki startowy i końcowy
                int start = rand() % n;
                int end = rand() % n;
                // Sprawdzenie czy wierchołek pącątkowy i końcowy nie są takie same
                while (end == start) end = rand() % n;
                
                // Test dla macierzy
                vector<int> dist_matrix;
                double time_matrix;
                dijkstra_matrix(start, dist_matrix, time_matrix);
                total_matrix_time += time_matrix;
                
                // Test dla listy
                vector<int> dist_list;
                double time_list;
                dijkstra_list(start, dist_list, time_list);
                total_list_time += time_list;
            }
            
            // Obliczanie średniego czasu wykonania algorytmu dla obu reprezentacji
            double avg_matrix_time = total_matrix_time / repetitions;
            double avg_list_time = total_list_time / repetitions;
            
            cout << "Sredni czas - Macierz: " << avg_matrix_time << " ms\n";
            cout << "Sredni czas - Lista: " << avg_list_time << " ms\n";
        }
    }
}

// Funkcja do wyświetlania menu
void showMenu() {
    cout << "\nMenu:\n";
    cout << "1. Wczytaj graf z pliku\n";
    cout << "2. Generuj losowy graf\n";
    cout << "3. Wyswietl graf\n";
    cout << "4. Wykonaj algorytm Dijkstry\n";
    cout << "5. Przeprowadz testy wydajnosciowe\n";  
    cout << "6. Wyjscie\n";  
    cout << "Wybor: ";
}

int main() {
    while (true) {
        showMenu();
        int choice;
        cin >> choice;
        
        // Funkcja switch do obsługi wyboru użytkownika
        switch (choice) {
            case 1: {
                string filename;
                cout << "Podaj nazwe pliku: ";
                cin >> filename;
                load_from_file(filename);
                break;
            }
            case 2: {
                int v, density;
                cout << "Podaj liczbe wierzcholkow: ";
                cin >> v;
                cout << "Podaj gestosc grafu (1-100%): ";
                cin >> density;
                generate_random_graph(v, density);
                break;
            }
            case 3:
                print_graph();
                break;
            case 4:
                run_dijkstra();
                break;
            case 5:
                run_test();
                break;
            case 6:
                return 0;
            default:
                cout << "Nieprawidlowy wybor!\n";
        }
    }
}