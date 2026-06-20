#ifndef DIJKSTRA_HPP
#define DIJKSTRA_HPP

#include "graph/ListGraph.hpp"
#include "graph/MatrixGraph.hpp"

// Wynik algorytmu Dijkstry
struct SpResult {
    int* dist;   // dist[v] = najkrotsza odleglosc od zrodla
    int* parent; // parent[v] = poprzednik na najkrotszej sciezce 
    int  size;   // liczba wierzcholkow
};

namespace Dijkstra {

    // Dijkstra na liscie sasiedztwa (graf skierowany)
    SpResult run(ListGraph* g, int source);

    // Dijkstra na macierzy incydencji (graf skierowany)
    SpResult run(MatrixGraph* g, int source);

    // Zwolnienie pamieci wyniku
    void freeResult(SpResult& r);

} // namespace Dijkstra

#endif
