#include "Prim.hpp"

static const int INF = 1000000000;

namespace Prim {

    // Zwraca indeks wierzcholka z najmniejszym kluczem sposrod nie dodanych do MST
    static int minKey(const int* key, const bool* inMST, int V) {
        int min = INF, minIdx = -1;
        for (int v = 0; v < V; ++v) {
            if (!inMST[v] && key[v] < min) {
                min    = key[v];
                minIdx = v;
            }
        }
        return minIdx;
    }

    // Lista sasiedztwa 
    MstResult run(ListGraph* g) {
        int V = g->getNumVertices();

        MstResult result;
        result.size   = V;
        result.parent = new int[V];
        result.key    = new int[V];
        bool* inMST   = new bool[V];

        for (int i = 0; i < V; ++i) {
            result.key[i]    = INF;
            result.parent[i] = -1;
            inMST[i]         = false;
        }
        result.key[0] = 0; // startujemy od wierzcholka 0

        for (int iter = 0; iter < V; ++iter) {
            int u = minKey(result.key, inMST, V);
            if (u == -1) break;

            inMST[u] = true;

            // Sprawdz wszystkich sasiadow u
            int count = g->getNeighborCount(u);
            for (int i = 0; i < count; ++i) {
                Edge e = g->getNeighbor(u, i);
                int v = e.end_vertex;
                int w = e.weight;
                if (!inMST[v] && w < result.key[v]) {
                    result.key[v]    = w;
                    result.parent[v] = u;
                }
            }
        }

        // Suma wag MST
        result.totalWeight = 0;
        for (int i = 1; i < V; ++i) {
            if (result.key[i] != INF) result.totalWeight += result.key[i];
        }

        delete[] inMST;
        return result;
    }

    // Macierz incydencji
    // Krawedz nieskierowana: matrix[u][j] == 1 i matrix[v][j] == 1
    MstResult run(MatrixGraph* g) {
        int V = g->getNumVertices();
        int E = g->getNumEdges();

        MstResult result;
        result.size   = V;
        result.parent = new int[V];
        result.key    = new int[V];
        bool* inMST   = new bool[V];

        for (int i = 0; i < V; ++i) {
            result.key[i]    = INF;
            result.parent[i] = -1;
            inMST[i]         = false;
        }
        result.key[0] = 0;

        for (int iter = 0; iter < V; ++iter) {
            int u = minKey(result.key, inMST, V);
            if (u == -1) break;

            inMST[u] = true;

            // krawedzie z U (kolumny gdzie matrix[u][j] == 1)
            for (int j = 0; j < E; ++j) {
                if (g->getCell(u, j) != 1) continue;

                // Znajdz drugi koniec krawedzi
                int v = -1;
                for (int k = 0; k < V; ++k) {
                    if (k != u && g->getCell(k, j) == 1) {
                        v = k;
                        break;
                    }
                }
                if (v == -1) continue;

                int w = g->getEdgeWeight(j);
                if (!inMST[v] && w < result.key[v]) {
                    result.key[v]    = w;
                    result.parent[v] = u;
                }
            }
        }

        // Suma wag MST
        result.totalWeight = 0;
        for (int i = 1; i < V; ++i) {
            if (result.key[i] != INF) result.totalWeight += result.key[i];
        }

        delete[] inMST;
        return result;
    }

    void freeResult(MstResult& r) {
        delete[] r.parent;
        delete[] r.key;
        r.parent      = nullptr;
        r.key         = nullptr;
        r.size        = 0;
        r.totalWeight = 0;
    }

} 
