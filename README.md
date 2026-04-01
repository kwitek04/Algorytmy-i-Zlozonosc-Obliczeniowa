<div align="right">
  <a href="README_pl.md">
    <img src="https://img.shields.io/badge/Polski-red.svg" alt="PL" />
  </a>
  <a href="README.md">
    <img src="https://img.shields.io/badge/English-blue.svg" alt="EN" />
  </a>
</div>

# Algorithms and Computational Complexity

A collection of programs and performance tests implemented as part of the **Algorithms and Computational Complexity** and **Design of Efficient Algorithms** courses. These projects focus on the implementation of various algorithms and the study of their execution time depending on the input data size and the applied data structures.

## Repository Contents

### 1. Asymmetric Traveling Salesman Problem (ATSP)
A C++ application solving the ATSP using exact and approximation algorithms.
* **Implemented algorithms:** Exhaustive search (Brute-Force), greedy (Nearest Neighbor, Repetitive Nearest Neighbor), and a random algorithm (RAND).
* **Project features:** Dynamic memory allocation, execution time measurement, and relative error calculation. The program can generate random graph instances and read/write data in the standard **TSPLIB** format.

### 2. Dijkstra's Algorithm
A program testing the impact of graph representation in computer memory on the execution time of Dijkstra's algorithm using a priority queue. 
* **Project features:** It compares the performance of adjacency list and adjacency matrix representations. The program includes a random graph generator with a specified density and number of vertices, an option to load graphs from a file, and a module for conducting performance tests.

### 3. Sorting Algorithms Benchmarking
A program testing the performance of insertion sort (Insertion Sort) algorithms in various variants. 
* **Project features:** The program generates random datasets with increasing sizes from 100 to 100,000 elements, verifies the correctness of sorting, and exports the results (execution time, number of comparisons, number of swaps) to a CSV file for further analysis.