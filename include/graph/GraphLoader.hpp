#ifndef GRAPH_LOADER_HPP
#define GRAPH_LOADER_HPP

#include "ListGraph.hpp"
#include "MatrixGraph.hpp"

//1linia v e
//2linia start koniec waga

namespace GraphLoader {

    ListGraph*   loadListGraph  (const char* filename, bool directed);
    MatrixGraph* loadMatrixGraph(const char* filename, bool directed);

}

#endif
