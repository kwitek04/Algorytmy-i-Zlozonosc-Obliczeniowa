# Algorytmy i Złożoność Obliczeniowa

Zbiór programów i testów wydajnościowych zrealizowanych w ramach przedmiotu **Algorytmy i Złożoność Obliczeniowa**. Projekty te skupiają się na implementacji różnych algorytmów, oraz badaniu ich czasu wykonania w zależności od rozmiaru danych wejściowych oraz zastosowanych struktur danych.

## Zawartość repozytorium

### 1. Badanie algorytmów sortowania (`sortowanie.cpp`)
Program testujący wydajność algorytmu sortowania przez wstawianie (Insertion sort) w różnych wariantach. Program generuje losowe zestawy danych o rosnących rozmiarach od 100 do 100 000 elementów, weryfikuje poprawność sortowania i eksportuje wyniki (czas wykonania, liczba porównań, liczba przestawień) do pliku CSV w celu dalszej analizy.

### 2. Algorytm Dijkstry (`dijkstra.cpp`)
Program testujący wpływ reprezentacji grafu w pamięci komputera na czas wykonywania algorytmu Dijkstry z użyciem kolejki priorytetowej. Porównywana jest reprezentacja w postaci listy sąsiedztwa, oraz macierzy sąsiedztwa. Program posiada generator grafów losowych o zadanej gęstości oraz liczbie wierzchołków, oraz opcję wczytywania grafów z pliku. Program umożliwia też uruchomienie testów wydaj