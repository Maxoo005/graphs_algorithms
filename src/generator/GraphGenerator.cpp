#include "generator/GraphGenerator.hpp"
#include "graph/Edge.hpp"
#include <cstdlib> // rand

namespace GraphGenerator {

// Pomocnicza: tasuje tablice krawedzi metodą Fishera-Yatesa
static void shuffleEdges(Edge* arr, int n) {
    for (int i = n - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        Edge tmp = arr[i];
        arr[i]   = arr[j];
        arr[j]   = tmp;
    }
}

// Graf nieskierowany, spojny
void generateUndirected(int V, int density,
                        ListGraph*& outList, MatrixGraph*& outMatrix) {
    // Maksymalna liczba krawedzi w grafie pelnym nieskierowanym
    int maxE = V * (V - 1) / 2;

    // Docelowa liczba krawedzi wg gestosci
    int targetE = maxE * density / 100;

    // Macierz uzytych krawedzi 
    bool** used = new bool*[V];
    for (int i = 0; i < V; ++i) {
        used[i] = new bool[V]();
    }

    // Bufor na wszystkie krawedzie
    Edge* edges = new Edge[maxE];
    int edgeCount = 0;

    //Struktura spajajaca: lancuch 0-1-2-...-(V-1) 
    for (int i = 0; i < V - 1; ++i) {
        edges[edgeCount++] = {i, i + 1, 0};
        used[i][i + 1]     = true;
        used[i + 1][i]     = true;
    }

    // Zamkniecie cyklu: (V-1) -> 0 tylko jesli V > 2
    if (V > 2 && !used[V - 1][0]) {
        edges[edgeCount++] = {V - 1, 0, 0};
        used[V - 1][0]     = true;
        used[0][V - 1]     = true;
    }

    int spanCount = edgeCount; // ile krawedzi 

    // Upewnij sie ze targetE >= spanCount
    if (targetE < spanCount) targetE = spanCount;
    if (targetE > maxE)      targetE = maxE;

    // Zakres wag: rand(1, targetE * 4/5), minimum 1
    int maxWeight = targetE * 4 / 5;
    if (maxWeight < 1) maxWeight = 1;

    // Pula pozostalych mozliwych krawedzi 
    int poolMax = maxE - spanCount;
    Edge* pool  = new Edge[poolMax];
    int poolSize = 0;

    for (int u = 0; u < V; ++u) {
        for (int v = u + 1; v < V; ++v) {
            if (!used[u][v]) {
                pool[poolSize++] = {u, v, 0};
            }
        }
    }

    // Tasowanie puli
    shuffleEdges(pool, poolSize);

    // Dociagnij do targetE losowymi krawędziami z puli
    int need = targetE - spanCount;
    for (int i = 0; i < need && i < poolSize; ++i) {
        edges[edgeCount++] = pool[i];
    }

    // Zwolnij pomocnicze struktury
    delete[] pool;
    for (int i = 0; i < V; ++i) delete[] used[i];
    delete[] used;

    // Przypisz wagi 
    maxWeight = edgeCount * 4 / 5;
    if (maxWeight < 1) maxWeight = 1;
    for (int i = 0; i < edgeCount; ++i) {
        edges[i].weight = rand() % maxWeight + 1;
    }

    // Zbuduj obie reprezentacje
    outList   = new ListGraph(V);
    outMatrix = new MatrixGraph(V, edgeCount);
    for (int i = 0; i < edgeCount; ++i) {
        outList->addUndirectedEdge(edges[i].start_vertex, edges[i].end_vertex, edges[i].weight);
        outMatrix->addUndirectedEdge(edges[i].start_vertex, edges[i].end_vertex, edges[i].weight);
    }

    delete[] edges;
}

// Graf skierowany, spojny spadajaca
void generateDirected(int V, int density,
                      ListGraph*& outList, MatrixGraph*& outMatrix) {
    // Maksymalna liczba krawedzi w grafie pelnym skierowanym 
    int maxE = V * (V - 1);

    // Docelowa liczba krawedzi wg gestosci
    int targetE = maxE * density / 100;

    // Macierz uzytych krawedzi skierowanych
    bool** used = new bool*[V];
    for (int i = 0; i < V; ++i) {
        used[i] = new bool[V]();
    }

    Edge* edges = new Edge[maxE];
    int edgeCount = 0;

    // Struktura spajajaca: cykl 0->1->2->...->( V-1)->0 
    for (int i = 0; i < V; ++i) {
        int u = i;
        int v = (i + 1) % V;
        edges[edgeCount++] = {u, v, 0};
        used[u][v]         = true;
    }

    int spanCount = edgeCount; // = V

    if (targetE < spanCount) targetE = spanCount;
    if (targetE > maxE)      targetE = maxE;

    // Pula pozostalych mozliwych krawedzi skierowanych 
    int poolMax = maxE - spanCount;
    Edge* pool  = new Edge[poolMax];
    int poolSize = 0;

    for (int u = 0; u < V; ++u) {
        for (int v = 0; v < V; ++v) {
            if (u != v && !used[u][v]) {
                pool[poolSize++] = {u, v, 0};
            }
        }
    }

    shuffleEdges(pool, poolSize);

    int need = targetE - spanCount;
    for (int i = 0; i < need && i < poolSize; ++i) {
        edges[edgeCount++] = pool[i];
    }

    delete[] pool;
    for (int i = 0; i < V; ++i) delete[] used[i];
    delete[] used;

    // Przypisz wagi
    int maxWeight = edgeCount * 4 / 5;
    if (maxWeight < 1) maxWeight = 1;
    for (int i = 0; i < edgeCount; ++i) {
        edges[i].weight = rand() % maxWeight + 1;
    }

    // Zbuduj obie reprezentacje
    outList   = new ListGraph(V);
    outMatrix = new MatrixGraph(V, edgeCount);
    for (int i = 0; i < edgeCount; ++i) {
        outList->addDirectedEdge(edges[i].start_vertex, edges[i].end_vertex, edges[i].weight);
        outMatrix->addDirectedEdge(edges[i].start_vertex, edges[i].end_vertex, edges[i].weight);
    }

    delete[] edges;
}

} 
