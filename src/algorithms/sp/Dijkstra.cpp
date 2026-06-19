#include "Dijkstra.hpp"

// "Nieskonczonosc" - nie INT_MAX, zeby uniknac overflow przy dist[u] + w
static const int INF = 1000000000;

namespace Dijkstra {

    // Zwraca indeks wierzcholka z najmniejszym dist sposrod nieodwiedzonych
    static int minDist(const int* dist, const bool* visited, int V) {
        int min = INF, minIdx = -1;
        for (int v = 0; v < V; ++v) {
            if (!visited[v] && dist[v] <= min) {
                min    = dist[v];
                minIdx = v;
            }
        }
        return minIdx;
    }

    // Lista sasiedztwa 
    SpResult run(ListGraph* g, int source) {
        int V = g->getNumVertices();

        SpResult result;
        result.size   = V;
        result.dist   = new int[V];
        result.parent = new int[V];
        bool* visited = new bool[V];

        for (int i = 0; i < V; ++i) {
            result.dist[i]   = INF;
            result.parent[i] = -1;
            visited[i]       = false;
        }
        result.dist[source] = 0;

        for (int iter = 0; iter < V; ++iter) {
            int u = minDist(result.dist, visited, V);
            if (u == -1) break; // pozostale wierzcholki sa nieosiagalne

            visited[u] = true;

            // Relaksuj krawedzie wychodzace z u
            int count = g->getNeighborCount(u);
            for (int i = 0; i < count; ++i) {
                Edge e = g->getNeighbor(u, i);
                int v = e.end_vertex;
                int w = e.weight;
                if (!visited[v] && result.dist[u] + w < result.dist[v]) {
                    result.dist[v]   = result.dist[u] + w;
                    result.parent[v] = u;
                }
            }
        }

        delete[] visited;
        return result;
    }

    // Macierz incydencji 
    // Krawedz skierowana: matrix[start][j] == -1, matrix[end][j] == 1
    SpResult run(MatrixGraph* g, int source) {
        int V = g->getNumVertices();
        int E = g->getNumEdges();

        SpResult result;
        result.size   = V;
        result.dist   = new int[V];
        result.parent = new int[V];
        bool* visited = new bool[V];

        for (int i = 0; i < V; ++i) {
            result.dist[i]   = INF;
            result.parent[i] = -1;
            visited[i]       = false;
        }
        result.dist[source] = 0;

        for (int iter = 0; iter < V; ++iter) {
            int u = minDist(result.dist, visited, V);
            if (u == -1) break;

            visited[u] = true;

            // Szukaj krawedzi wychodzacych z u (kolumny gdzie matrix[u][j] == -1)
            for (int j = 0; j < E; ++j) {
                if (g->getCell(u, j) != -1) continue;

                // Znajdz wierzcholek koncowy (matrix[v][j] == 1)
                int v = -1;
                for (int k = 0; k < V; ++k) {
                    if (g->getCell(k, j) == 1) {
                        v = k;
                        break;
                    }
                }
                if (v == -1) continue;

                int w = g->getEdgeWeight(j);
                if (!visited[v] && result.dist[u] + w < result.dist[v]) {
                    result.dist[v]   = result.dist[u] + w;
                    result.parent[v] = u;
                }
            }
        }

        delete[] visited;
        return result;
    }

    void freeResult(SpResult& r) {
        delete[] r.dist;
        delete[] r.parent;
        r.dist   = nullptr;
        r.parent = nullptr;
        r.size   = 0;
    }

} // namespace Dijkstra
