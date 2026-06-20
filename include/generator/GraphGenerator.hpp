#ifndef GRAPH_GENERATOR_HPP
#define GRAPH_GENERATOR_HPP

#include "graph/ListGraph.hpp"
#include "graph/MatrixGraph.hpp"

namespace GraphGenerator {

    // Nieskierowany, spojny graf 
    // Struktura spajajaca: lancuch 0-1-2-...(V-1) oraz krawedz (V-1)-0 (jesli V > 2).
    // Nastepnie losowe krawedzie do osiagniecia zadanej gestosci.
    // gestosc w % (np. 25, 50, 75, 99)
    void generateUndirected(int V, int density,
                            ListGraph*& outList, MatrixGraph*& outMatrix);

    // Skierowany, spojny graf 
    // Struktura spajajaca: cykl 0->1->2->...->( V-1)->0 (silnie spojny).
    // Nastepnie losowe krawedzie skierowane do zadanej gestosci.
    void generateDirected(int V, int density,
                          ListGraph*& outList, MatrixGraph*& outMatrix);

} 

#endif
