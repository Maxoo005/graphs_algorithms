#ifndef LIST_GRAPH_HPP
#define LIST_GRAPH_HPP

#include "Edge.hpp"

class ListGraph{
private:
    Edge** adjacency_lists;  //lista sosiadow
    int* list_sizes; //tab z listą krawedzi dla każdego wierzchołka
    int* list_capacities; //tab z pojemnością listy dla każdego wierzchołka
    int num_vertices; //liczba wierzcholkow


    // rozszerzanie pamięci dla konkretnego wierzchołka
    void growList(int vertex) {
        // Jeśli pojemność 0, startujemy od 2, w przeciwnym wypadku podwajamy
        int new_capacity = (list_capacities[vertex] == 0) ? 2 : list_capacities[vertex] * 2;
        
        // Alokacja nowej, większej tablicy
        Edge* new_list = new Edge[new_capacity];
        
        // Przepisanie starych danych
        for (int i = 0; i < list_sizes[vertex]; ++i) {
            new_list[i] = adjacency_lists[vertex][i];
        }
        
        // Zwolnienie starej pamięci
        delete[] adjacency_lists[vertex];
        
        // Przypisanie nowego wskaźnika i nowej pojemności
        adjacency_lists[vertex] = new_list;
        list_capacities[vertex] = new_capacity;
    }

public:
    // Konstruktor 
    ListGraph(int vertices) {
        num_vertices = vertices;

        //aokacja tablic
        adjacency_lists = new Edge*[num_vertices];
        list_sizes = new int[num_vertices]{0}; //zeruuujemy rozm
        list_capacities = new int[num_vertices]{0}; //zerujemy poj

        for(int i=0; i<num_vertices; ++i){
            adjacency_lists[i] = nullptr; //brak list sasiadow
        }
    }
    
    //destruktor- bum bum  suswamy liste sąsiadow ksxdego wierzchołka 
    ~ListGraph() {
        for(int i=0; i<num_vertices; ++i){
            delete[] adjacency_lists[i];
        }
        delete[] adjacency_lists;
        delete[] list_sizes;
        delete[] list_capacities;
    }

    void addDirectedEdge(int start, int end, int weight) {
        // Jeśli brakuje miejsca w tablicy sąsiadów wierzchołka 'start', powiększ ją
        if (list_sizes[start] >= list_capacities[start]) {
            growList(start);
        }

        // Dodanie nowej krawędzi na koniec listy wierzchołka startowego
        adjacency_lists[start][list_sizes[start]] = Edge{start, end, weight};
        list_sizes[start]++;
    }

    // NIESKIEROWANA 
    void addUndirectedEdge(int u, int v, int weight) {
        addDirectedEdge(u, v, weight);
        addDirectedEdge(v, u, weight);
    }

    int getNumVertices()        const { return num_vertices; }
    int getNeighborCount(int v) const { return list_sizes[v]; }
    Edge getNeighbor(int v, int i) const { return adjacency_lists[v][i]; }
};

#endif