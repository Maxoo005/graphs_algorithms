#ifndef PRIM_HPP
#define PRIM_HPP

#include "../../graph/ListGraph.hpp"
#include "../../graph/MatrixGraph.hpp"

// Wynik PRIM
struct MstResult {
    int* parent;      // parent[v] = rodzic v w MST (-1 = korzen/nieosiagalny)
    int* key;         // key[v] = waga krawedzi laczacej v z rodzicem
    int  size;        // liczba wierzcholkow
    int  totalWeight; // suma wag MST
};

namespace Prim {

    // Prim na liscie sasiedztwa (graf nieskierowany)
    MstResult run(ListGraph* g);

    // Prim na macierzy incydencji (graf nieskierowany)
    MstResult run(MatrixGraph* g);

    // Zwolnienie pamieci wyniku
    void freeResult(MstResult& r);

} 

#endif
