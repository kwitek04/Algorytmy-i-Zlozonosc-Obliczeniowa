#include <iostream>
#include <fstream> // Biblioteka potrzebna do zapisu wyników do pliku
#include <chrono> // Biblioteka potrzebna do pomiaru czasu działania algorytmu
#include <cstdint> // Biblioteka potrzebna do typu danych int64_t, pozwalającego przechowywać 64-bitowe liczby całkowite
using namespace std;
using namespace chrono;

const int liczba_testow = 100; // Stała liczba testów do przeprowadzenia dla konkretnego rozmiaru danych

void generuj_tablice_losowa(int tablica[], int n) { // Funkcja wypełniająca tablicę liczbami losowymi
    srand(time(NULL));
    for (int i = 0; i < n; i++){
        tablica [i] = rand() % 10000; // Tablica uzupełniana losowymi liczbami z zakresu 0-9999
    }
}

bool sprawdz_sortowanie(int tablica[], int rozmiar) { // Funkcja sprawdzająca, czy tablica jest posortowana
    for (int i = 1; i < rozmiar; i++) {
        if (tablica[i-1] > tablica[i]) return false;
    }
    return true;
}

struct Statystyki{ // Struktura do przechowywania statystyk sortowania
    int64_t porownania; // Liczba porównań i przestawień typu int64_t, ponieważ poruszamy sie po bardzo dużych liczbach
    int64_t przestawienia;
    double czas;
};

Statystyki klasyczne_sortowanie(int tablica[], int n) { // Klasyczny algorytm sortowania przez wstawianie liniowe
    Statystyki s = {0, 0, 0}; // Tworzymy nowy obiekt struktury Statystyki i inicjalizujemy go zerami
    auto start = high_resolution_clock::now(); // Zapis czasu rozpoczęcia algorytmu
    
    for (int i = 1; i < n; i++) { // Pętla iterująca przez elementy tablicy, z pominięciem pierwszego elementu, bo zakładamy na początku, że jest on uporządkowany
        int temp = tablica[i];
        int j = i - 1; // j = i - 1, ponieważ szukamy właściwego miejsca zaczynając porównania od elementu poprzedzającego rozważany element
        while (j >= 0) { // Pętla wewnętrzna, która szuka miejsca dla elementu tablica[i], oraz przesuwa elementy na właściwe pozycje
            s.porownania++; 
            if (tablica[j] > temp) {
                tablica[j+1] = tablica[j];
                s.przestawienia++;
                j--;
            } else {
                break;
            }
        }
        tablica[j + 1] = temp; // Wstawiamy rozważany element na właściwą pozycję
    }    
    s.czas = duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1000.0; // Czas działania algorytmu obliczany jako różnica czasu po zakończeniu algorytmu i przed jego rozpoczęciem. Czas jest konwertowany na milisekundy poprzez dzielenie przez 1000
    return s; // Zwracamy obiekt struktury Statystyki z wynikami działania algorytmu
}

Statystyki klasyczne_sortowanie_od_poczatku(int tablica[], int n) {
    Statystyki s = {0, 0, 0}; // Tworzymy nowy obiekt struktury Statystyki i inicjalizujemy go zerami
    auto start = high_resolution_clock::now(); // Zapis czasu rozpoczęcia algorytmu

    for (int i = 1; i < n; i++) { // Pętla iterująca przez elementy tablicy, z pominięciem pierwszego elementu, bo zakładamy na początku, że jest on uporządkowany
        int temp = tablica[i];
        int j = 0; // j = 0, ponieważ szukamy właściwego miejsca zaczynając porównania od początku tablicy
        while (j < i) { // Pętla wewnętrzna, która szuka miejsca dla elementu tablica[i]
            s.porownania++;
            if (tablica[j] > temp) {
                break;
            }
            j++;
        }
        for (int k = i; k > j; k--) { // Przesuwamy elementy w prawo, aby zrobić miejsce dla elementu tablca[i]
            tablica[k] = tablica[k - 1];
            s.przestawienia++;
        }
        tablica[j] = temp; // Wstawiamy rozważany element na właściwą pozycję
    }
    s.czas = duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1000.0; // Czas działania algorytmu obliczany jako różnica czasu po zakończeniu algorytmu i przed jego rozpoczęciem. Czas jest konwertowany na milisekundy poprzez dzielenie przez 1000
    return s; // Zwracamy obiekt struktury Statystyki z wynikami działania algorytmu
}

Statystyki klasyczne_sortowanie_odwrocone(int tablica[], int n) { // Odwrócony algorytm sortowania przez wstawianie liniowe
    Statystyki s = {0, 0, 0}; // Tworzymy nowy obiekt struktury Statystyki i inicjalizujemy go zerami
    auto start = high_resolution_clock::now(); // Zapis czasu rozpoczęcia algorytmu
    
    for (int i = n - 2; i >= 0; i--) { // Pętla iterująca przez elementy tablicy, zaczynając od elementu przedostatniego, z pominięciem ostatniego elementu, bo zakładamy na początku, że jest on uporządkowany
        int temp = tablica[i];
        int j = i + 1; // j = i + 1, ponieważ szukamy właściwego miejsca zaczynając porównania od elementu znajdującego się za rozważanym elementem
        while (j < n) { // Pętla wewnętrzna, która szuka miejsca dla elementu tablica[i], oraz przesuwa elementy na właściwe pozycje
            s.porownania++; 
            if (tablica[j] < temp) {
                tablica[j - 1] = tablica[j];
                s.przestawienia++;
                j++;
            } else {
                break;
            }
        }
        tablica[j - 1] = temp; // Wstawiamy rozważany element na właściwą pozycję
    }    
    s.czas = duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1000.0; // Czas działania algorytmu obliczany jako różnica czasu po zakończeniu algorytmu i przed jego rozpoczęciem. Czas jest konwertowany na milisekundy poprzez dzielenie przez 1000. Konwersja ponieważ gdy liczymy czas odrazu w milisekundach, to przy małych wartościach czas jest automatycznie zaokrąglany do 0ms
    return s; // Zwracamy obiekt struktury Statystyki z wynikami działania algorytmu
}

Statystyki klasyczne_sortowanie_od_konca(int tablica[], int n) {
    Statystyki s = {0, 0, 0}; // Tworzymy nowy obiekt struktury Statystyki i inicjalizujemy go zerami
    auto start = high_resolution_clock::now(); // Zapis czasu rozpoczęcia algorytmu

    for (int i = n - 2; i >= 0; i--) { // Pętla iterująca przez elementy tablicy, zaczynając od elementu przedostatniego, z pominięciem ostatniego elementu, bo zakładamy na początku, że jest on uporządkowany
        int temp = tablica[i];
        int j = n - 1; // j = n - 1, ponieważ szukamy właściwego miejsca zaczynając porównania od końca tablicy
        while (j > i) { // Pętla wewnętrzna, która szuka miejsca dla elementu tablica[i]
            s.porownania++;
            if (tablica[j] < temp) {
                break;
            }
            j--;
        }
        for (int k = i; k < j; k++) { // Przesuwamy elementy w lewo, aby zrobić miejsce dla elementu tablca[i]
            tablica[k] = tablica[k + 1];
            s.przestawienia++;
        }
        tablica[j] = temp; // Wstawiamy rozważany element na właściwą pozycję
    }
    s.czas = duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1000.0; // Czas działania algorytmu obliczany jako różnica czasu po zakończeniu algorytmu i przed jego rozpoczęciem. Czas jest konwertowany na milisekundy poprzez dzielenie przez 1000
    return s; // Zwracamy obiekt struktury Statystyki z wynikami działania algorytmu
}

Statystyki sortowanie_przez_wstawianie_binarne(int tablica[], int n) {
    Statystyki s = {0, 0, 0}; // Tworzymy nowy obiekt struktury Statystyki i inicjalizujemy go zerami
    auto start = high_resolution_clock::now(); // Zapis czasu rozpoczęcia algorytmu

    for (int i = 1; i < n; i++) { // Pętla iterująca przez elementy tablicy, z pominięciem pierwszego elementu, bo zakładamy na początku, że jest on uporządkowany
        int temp = tablica[i];
        
        int lewo = 0; // Inicializacja granic przeszukiwania binarnego
        int prawo = i;
        while (lewo < prawo) { // Pętla wewnętrzna, która szuka miejsca dla elementu tablica[i]
            s.porownania++;
            int srodek = (lewo + prawo) / 2; // Obliczanie środka przeszukiwanego zakresu. W każdym kroku ciąg do którego chcemy wstawić rozważany element, jest redukowany o połowę
            if (temp < tablica[srodek]) {
                prawo = srodek;
            } else {
                lewo = srodek + 1;
            }
        }
        for (int j = i; j > lewo; j--) { // Przesuwamy elementy w prawo, aby zrobić miejsce dla elementu tablca[i]
            tablica[j] = tablica[j - 1];
            s.przestawienia++;
        }
        tablica[lewo] = temp; // Wstawiamy rozważany element na właściwą pozycję
    }
    s.czas = duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1000.0; // Czas działania algorytmu obliczany jako różnica czasu po zakończeniu algorytmu i przed jego rozpoczęciem. Czas jest konwertowany na milisekundy poprzez dzielenie przez 1000
    return s; // Zwracamy obiekt struktury Statystyki z wynikami działania algorytmu
}

void testuj_wszystkie_algorytmy(int rozmiar, ofstream& plik_wynikowy) { // Funkcja testująca wszystkie algorytmy sortowania
    // Przygotowanie zestawu testowego
    int** zestawy_testowe = new int*[liczba_testow]; // Tworzymy tablicę wskaźników
    for (int i = 0; i < liczba_testow; i++) {
        zestawy_testowe[i] = new int[rozmiar]; // Alokacja pamięci dla każdego zestawu testowego
        generuj_tablice_losowa(zestawy_testowe[i], rozmiar); // Wypełnienie tablicy losowymi danymi
    }

    struct Algorytm { // Struktura do przechowywania wskaźnika do funkcji sortującej oraz nazwe algorytmu
        Statystyki (*funkcja)(int[], int); // Wskaźnik do funkcji sortującej
        const string nazwa; // Nazwa algorytmu
    };
    
    Algorytm algorytmy[] = { //Lista algorytmów do przetestowania
        {klasyczne_sortowanie, "Sortowanie klasyczne"},
        {klasyczne_sortowanie_odwrocone, "Odwrocone sortowanie klasyczne"},
        {klasyczne_sortowanie_od_poczatku, "Sortowanie klasyczne od początku"},
        {klasyczne_sortowanie_od_konca, "Sortowanie klasyczne od końca"},
        {sortowanie_przez_wstawianie_binarne, "Sortowanie przez wstawianie binarne"}
    };

    // Nagłówek pliku CSV, wszytskie dane zapisywane są w jedynm pliku więc wystarczy tylko jeden nagłówek
    if (rozmiar == 100) {
        plik_wynikowy << "Rozmiar;Algorytm;Srednie porownania;Srednie przestawienia;Sredni czas [ms];Poprawnosc\n";
    }

    for (auto& alg : algorytmy) { // Iteracja przez wszystkie algorytmy
        Statystyki suma = {0, 0, 0}; // Inicjalizacja sumy statystyk dla danego algorytmu
        int poprawne = 0; // Licznik poprawnych sortowań, potrzebny do sprawdzenia, czy algorytm dziala poprawnie
        
        for (int i = 0; i < liczba_testow; i++) { // Iteracja przez wszystkie zestawy testowe
            int* kopia = new int[rozmiar];
            for (int j = 0; j < rozmiar; j++) { // Tworzenie kopii oryginalnej tablicy
                kopia[j] = zestawy_testowe[i][j];
            }
            // Testowanie i sumowanie wyników
            Statystyki wyniki = alg.funkcja(kopia, rozmiar);
            suma.porownania += wyniki.porownania;
            suma.przestawienia += wyniki.przestawienia;
            suma.czas += wyniki.czas;
            
            if (sprawdz_sortowanie(kopia, rozmiar)) { //Sprawdzenie, czy tablica została poprawnie posortowana
                poprawne++;
            }
            delete[] kopia;
        }
        
        // Wyświetlanie wyników w konsoli
        cout << "\n" << alg.nazwa << "\n";
        cout << "Średnia liczba porównań: " << suma.porownania/liczba_testow << "\n";
        cout << "Średnia liczba przestawień: " << suma.przestawienia/liczba_testow << "\n";
        cout << "Średni czas [ms]: " << suma.czas/liczba_testow << "\n";
        cout << "Poprawność sortowań: " << poprawne << "/" << liczba_testow << "\n";
        
        // Zapis wyników do pliku CSV
        plik_wynikowy << rozmiar << ";"
                     << alg.nazwa << ";"
                     << suma.porownania/liczba_testow << ";"
                     << suma.przestawienia/liczba_testow << ";"
                     << suma.czas/liczba_testow << ";"
                     << poprawne << "/" << liczba_testow << "\n";
    }
    // Zwolnienie pamięci
    for (int i = 0; i < liczba_testow; i++) {
        delete[] zestawy_testowe[i];
    }
    delete[] zestawy_testowe;
}

int main() {
    ofstream plik_wynikowy("wyniki_sortowania.csv"); // Otwarcie pliku do zapisu
    if (!plik_wynikowy.is_open()) { // Sprawdzenie, czy plik został poprawnie otwarty, jeśli nie to wyświetlamy błąd
        cerr << "Nie można otworzyć pliku do zapisu wyników!\n"; 
        return 1;
    }

    int rozmiary[] = {100, 1000, 5000, 10000, 20000, 50000, 100000}; // Tablica rozmiarów do przetestowania, rozmiary można dowolnie zmieniać
    
    for (int rozmiar : rozmiary) {
        cout << "\nRozmiar: " << rozmiar << "\n";
        testuj_wszystkie_algorytmy(rozmiar, plik_wynikowy);
    }
    
    plik_wynikowy.close();
    cout << "\nWyniki zapisane do pliku wyniki_sortowania.csv\n";
    
    return 0;
}