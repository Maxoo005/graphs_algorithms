#ifndef GRAPH_STRUCTURES_HPP
#define GRAPH_STRUCTURES_HPP

struct Edge{
    int target_vertex; //pocz
    int weight;  //waga
};

struct MatrixGraph{
    int** adjacency_matrix;  //macierz sasiedztwa
    int* edge_weights; //wagi krawedzi
    int num_vertices; //liczba wierzcholkow
    int num_edges; //liczba krawedzi
};

struct ListGraph{
    Edge** adjacency_lists;  //lista sosiadow dynamiczna tablic
    int* list_sizes; //tab z listą krawedzi dla każdego wierzchołka
    int* list_capacities; //tab z pojemnością listy dla każdego wierzchołka
    int num_vertices; //liczba wierzcholkow
};

//zwalnianie pamieci
void freeMatrixGraph(MatrixGraph& g);
void freeListGraph(ListGraph& g);

#endif