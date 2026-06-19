#include "GraphLoader.hpp"
#include <fstream>
#include <iostream>

namespace GraphLoader {

    // GRAF lista sąsiadów
    ListGraph* loadListGraph(const char* filename, bool directed) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "GraphLoader Nie mozna otworzyc pliku: " << filename << "\n";
            return nullptr;
        }

        int V = 0, E = 0;
        file >> V >> E;

        ListGraph* g = new ListGraph(V);

        // Wczytaj E krawedzi
        int loaded = 0;
        for (int i = 0; i < E; ++i) {
            int u, v, w;
            file >> u >> v >> w;
            if (directed) {
                g->addDirectedEdge(u, v, w);
            } else {
                g->addUndirectedEdge(u, v, w);
            }
            ++loaded;
        }

        // JEST GIT?
        if (loaded != E) {
            std::cerr << "GraphLoader Weryfikacja: oczekiwano " << E
                      << " krawedzi, wczytano " << loaded << "\n";
        }

        return g;
    }

    //GRaf macierze
    MatrixGraph* loadMatrixGraph(const char* filename, bool directed) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "[GraphLoader] Nie mozna otworzyc pliku: " << filename << "\n";
            return nullptr;
        }

        int V = 0, E = 0;
        file >> V >> E;

        MatrixGraph* g = new MatrixGraph(V, E);

        // Wczytaj E krawedzi
        int loaded = 0;
        for (int i = 0; i < E; ++i) {
            int u, v, w;
            file >> u >> v >> w;
            if (directed) {
                g->addDirectedEdge(u, v, w);
            } else {
                g->addUndirectedEdge(u, v, w);
            }
            ++loaded;
        }

        // czy gites?
        if (g->getEdgeCount() != E) {
            std::cerr << "GraphLoader Weryfikacja: tyle " << E
                      << " krawedzi, wczytano " << g->getEdgeCount() << "\n";
        }

        return g;
    }

} 

