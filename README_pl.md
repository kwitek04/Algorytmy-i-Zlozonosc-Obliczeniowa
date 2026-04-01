<div align="right">
  <a href="README_pl.md">
    <img src="https://img.shields.io/badge/Polski-red.svg" alt="PL" />
  </a>
  <a href="README.md">
    <img src="https://img.shields.io/badge/English-blue.svg" alt="EN" />
  </a>
</div>

# Algorytmy i Złożoność Obliczeniowa

Zbiór programów i testów wydajnościowych zrealizowanych w ramach przedmiotów **Algorytmy i Złożoność Obliczeniowa**, oraz **Projektowanie Efektywnych Algorytmów**. Projekty te skupiają się na implementacji różnych algorytmów oraz badaniu ich czasu wykonania w zależności od rozmiaru danych wejściowych i zastosowanych struktur danych.

## Zawartość repozytorium

### 1. Asymetryczny Problem Komiwojażera (ATSP)
Aplikacja w języku C++ rozwiązująca problem ATSP za pomocą algorytmów dokładnych i przybliżonych.
* **Zaimplementowane algorytmy:** Przegląd zupełny (Brute-Force), zachłanne (Nearest Neighbor, Repetitive Nearest Neighbor) oraz algorytm losowy (RAND).
* **Cechy projektu:** Dynamiczna alokacja pamięci, pomiar czasu wykonywania, oraz obliczanie błędu względnego. Program potrafi generować losowe instancje grafów oraz wczytywać/zapisywać dane w standardowym formacie **TSPLIB**.

### 2. Algorytm Dijkstry
Program testujący wpływ reprezentacji grafu w pamięci komputera na czas wykonywania algorytmu Dijkstry z użyciem kolejki priorytetowej. 
* **Cechy projektu:** Porównywana jest wydajność reprezentacji w postaci listy sąsiedztwa oraz macierzy sąsiedztwa. Program posiada generator grafów losowych o zadanej gęstości oraz liczbie wierzchołków, opcję wczytywania grafów z pliku oraz moduł do przeprowadzania testów wydajnościowych.

### 3. Badanie algorytmów sortowania
Program testujący wydajność algorytmów sortowania przez wstawianie (Insertion Sort) w różnych wariantach. 
* **Cechy projektu:** Program generuje losowe zestawy danych o rosnących rozmiarach od 100 do 100 000 elementów, weryfikuje poprawność sortowania i eksportuje wyniki (czas wykonania, liczba porównań, liczba przestawień) do pliku CSV w celu dalszej analizy.