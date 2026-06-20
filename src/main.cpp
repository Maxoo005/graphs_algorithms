#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <cstdlib>
#include <climits>

#include "Parameters.h"
#include "graph/ListGraph.hpp"
#include "graph/MatrixGraph.hpp"
#include "graph/GraphLoader.hpp"
#include "graph/Edge.hpp"
#include "algorithms/mst/Prim.hpp"
#include "algorithms/sp/Dijkstra.hpp"
#include "generator/GraphGenerator.hpp"

static const int INF_VAL = 1000000000;

// Wyswietlanie grafu – lista sasiedztwa
static void printListGraph(ListGraph* g, bool directed) {
    int V = g->getNumVertices();
    std::cout << "  Lista sasiedztwa ("
              << (directed ? "skierowany" : "nieskierowany")
              << "), wierzcholkow: " << V << "\n";
    for (int u = 0; u < V; ++u) {
        std::cout << "    v" << u << ": ";
        int cnt = g->getNeighborCount(u);
        for (int i = 0; i < cnt; ++i) {
            Edge e = g->getNeighbor(u, i);
            std::cout << e.end_vertex << "(w=" << e.weight << ")  ";
        }
        std::cout << "\n";
    }
}

// Wyswietlanie grafu – macierz incydencji
static void printMatrixGraph(MatrixGraph* g) {
    int V = g->getNumVertices();
    int E = g->getNumEdges();
    std::cout << "  Macierz incydencji, wierzcholkow: " << V
              << ", krawedzi: " << E << "\n";
    // naglowek kolumn
    std::cout << "       ";
    for (int j = 0; j < E; ++j) std::cout << " e" << j;
    std::cout << "\n";
    for (int v = 0; v < V; ++v) {
        std::cout << "  v" << v << ":  ";
        for (int j = 0; j < E; ++j) {
            int cell = g->getCell(v, j);
            if (cell >= 0) std::cout << "  " << cell;
            else           std::cout << " "  << cell; // -1 zajmuje 2 znaki
        }
        std::cout << "\n";
    }
    std::cout << "  wagi:  ";
    for (int j = 0; j < E; ++j) std::cout << " " << g->getEdgeWeight(j);
    std::cout << "\n";
}

// Wyswietlanie wyniku MST (Prim)
static void printMstResult(MstResult& r) {
    std::cout << "  Wynik MST (Prim), calkowita waga: " << r.totalWeight << "\n";
    std::cout << "  Krawedzie MST:\n";
    for (int v = 0; v < r.size; ++v) {   // startujemy od 0 – korzeń ma parent==-1
        if (r.parent[v] >= 0) {
            std::cout << "    " << r.parent[v] << " -- " << v
                      << "  (waga " << r.key[v] << ")\n";
        }
    }
}

// Wyswietlanie wyniku SP (Dijkstra)
static void printSpResult(SpResult& r, int source, int endV = -1) {
    std::cout << "  Wynik SP (Dijkstra), zrodlo: " << source;
    if (endV >= 0) std::cout << ", cel: " << endV;
    std::cout << "\n";
    int* path = new int[r.size];
    for (int v = 0; v < r.size; ++v) {
        if (v == source) continue;
        if (endV >= 0 && v != endV) continue; // filtruj wg vertexEnd
        std::cout << "  " << source << " -> " << v << ":  ";
        if (r.dist[v] >= INF_VAL) {
            std::cout << "NIEOSIAGALNY\n";
        } else {
            std::cout << "dlugosc=" << r.dist[v] << "  sciezka: ";
            int len = 0, cur = v;
            while (cur != -1 && len < r.size) {
                path[len++] = cur;
                cur = r.parent[cur];
            }
            for (int i = len - 1; i >= 0; --i) {
                std::cout << path[i];
                if (i > 0) std::cout << " -> ";
            }
            std::cout << "\n";
        }
    }
    delete[] path;
}

// Zapis wiersza do pliku CSV z wynikami badan
static void saveCsvRow(const std::string& filename,
                       const char* problem, const char* algorithm,
                       const char* structure,
                       int V, int E, int density, int iterNum,
                       long long timeUs) {
    // Jesli plik nie istnieje, dopisz naglowek
    std::ifstream check(filename);
    bool needsHeader = !check.is_open();
    check.close();

    std::ofstream f(filename, std::ios::app);
    if (!f.is_open()) {
        std::cerr << "Nie mozna otworzyc pliku wynikow: " << filename << "\n";
        return;
    }
    if (needsHeader) {
        f << "timestamp,problem,algorithm,structure,"
             "vertices,edges,density,iteration,time_us\n";
    }

    std::time_t now = std::time(nullptr);
    char buf[32];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));

    f << buf << "," << problem << "," << algorithm << "," << structure
      << "," << V << "," << E << "," << density << "," << iterNum
      << "," << timeUs << "\n";
}

// Tryb pojedynczego testu
static void runSingleFile() {
    bool directed = (Parameters::problem == Parameters::Problems::sp);

    bool doList   = (Parameters::structure == Parameters::Structures::adjacencyList   ||
                     Parameters::structure == Parameters::Structures::allStructures);
    bool doMatrix = (Parameters::structure == Parameters::Structures::incidenceMatrix ||
                     Parameters::structure == Parameters::Structures::allStructures);

    // start/endVertex dla trybu singleFile
    int startV = (Parameters::vertexStart >= 0) ? Parameters::vertexStart : 0;
    int endV   = Parameters::vertexEnd; 

    // Jawna obsluga opcji 0 = allAlgorithms
    bool doPrim     = (Parameters::algorithm == Parameters::Algorithms::allAlgorithms ||
                       Parameters::algorithm == Parameters::Algorithms::prim);
    bool doDijkstra = (Parameters::algorithm == Parameters::Algorithms::allAlgorithms ||
                       Parameters::algorithm == Parameters::Algorithms::dijkstra);

    // Lista sasiedztwa 
    if (doList) {
        ListGraph* g = GraphLoader::loadListGraph(
            Parameters::inputFile.c_str(), directed);
        if (g != nullptr) {
            std::cout << "\n=== Lista sasiedztwa ===\n";
            printListGraph(g, directed);

            if (Parameters::problem == Parameters::Problems::mst && doPrim) {
                MstResult r = Prim::run(g, startV);
                printMstResult(r);
                Prim::freeResult(r);
            } else if (Parameters::problem == Parameters::Problems::sp && doDijkstra) {
                SpResult r = Dijkstra::run(g, startV);
                printSpResult(r, startV, endV);
                Dijkstra::freeResult(r);
            }
            delete g;
        }
    }

    // S Macierz incydencji 
    if (doMatrix) {
        MatrixGraph* g = GraphLoader::loadMatrixGraph(
            Parameters::inputFile.c_str(), directed);
        if (g != nullptr) {
            std::cout << "\n=== Macierz incydencji ===\n";
            printMatrixGraph(g);

            if (Parameters::problem == Parameters::Problems::mst && doPrim) {
                MstResult r = Prim::run(g, startV);
                printMstResult(r);
                Prim::freeResult(r);
            } else if (Parameters::problem == Parameters::Problems::sp && doDijkstra) {
                SpResult r = Dijkstra::run(g, startV);
                printSpResult(r, startV, endV);
                Dijkstra::freeResult(r);
            }
            delete g;
        }
    }
}

// Pomiar czasu N iteracji – lista sasiedztwa
static void benchmarkList(ListGraph* g, bool isMst, int iters,
                          long long& outMin, long long& outAvg, long long& outMax,
                          const std::string& resultsFile,
                          const char* probStr, const char* algStr,
                          int V, int E, int density) {
    long long sum = 0;
    outMin = LLONG_MAX;
    outMax = 0;
    for (int i = 0; i < iters; ++i) {
        long long us = 0;
        if (isMst) {
            auto t0 = std::chrono::high_resolution_clock::now();
            MstResult r = Prim::run(g);
            auto t1 = std::chrono::high_resolution_clock::now();
            us = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count();
            Prim::freeResult(r);
        } else {
            auto t0 = std::chrono::high_resolution_clock::now();
            SpResult r = Dijkstra::run(g, 0);
            auto t1 = std::chrono::high_resolution_clock::now();
            us = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count();
            Dijkstra::freeResult(r);
        }
        if (us < outMin) outMin = us;
        if (us > outMax) outMax = us;
        sum += us;
        // Zapisz wynik tej iteracji do pliku CSV
        if (!resultsFile.empty()) {
            saveCsvRow(resultsFile, probStr, algStr, "adjacencyList",
                       V, E, density, i + 1, us);
        }
    }
    outAvg = sum / iters;
}

// Pomiar czasu N iteracji – macierz incydencji
static void benchmarkMatrix(MatrixGraph* g, bool isMst, int iters,
                            long long& outMin, long long& outAvg, long long& outMax,
                            const std::string& resultsFile,
                            const char* probStr, const char* algStr,
                            int V, int E, int density) {
    long long sum = 0;
    outMin = LLONG_MAX;
    outMax = 0;
    for (int i = 0; i < iters; ++i) {
        long long us = 0;
        if (isMst) {
            auto t0 = std::chrono::high_resolution_clock::now();
            MstResult r = Prim::run(g);
            auto t1 = std::chrono::high_resolution_clock::now();
            us = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count();
            Prim::freeResult(r);
        } else {
            auto t0 = std::chrono::high_resolution_clock::now();
            SpResult r = Dijkstra::run(g, 0);
            auto t1 = std::chrono::high_resolution_clock::now();
            us = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count();
            Dijkstra::freeResult(r);
        }
        if (us < outMin) outMin = us;
        if (us > outMax) outMax = us;
        sum += us;
        // Zapisz wynik tej iteracji do pliku CSV
        if (!resultsFile.empty()) {
            saveCsvRow(resultsFile, probStr, algStr, "incidenceMatrix",
                       V, E, density, i + 1, us);
        }
    }
    outAvg = sum / iters;
}


// Tryb badan
static void runBenchmark() {
    int V       = Parameters::vertexCount;
    int density = Parameters::density;
    int iters   = Parameters::iterations;
    if (iters <= 0) iters = 1;

    bool isMst = (Parameters::problem == Parameters::Problems::mst);

    // Jawna obsluga opcji 0 = allAlgorithms 
    bool runPrim     = isMst  && (Parameters::algorithm == Parameters::Algorithms::allAlgorithms ||
                                  Parameters::algorithm == Parameters::Algorithms::prim);
    bool runDijkstra = !isMst && (Parameters::algorithm == Parameters::Algorithms::allAlgorithms ||
                                  Parameters::algorithm == Parameters::Algorithms::dijkstra);

    if (!runPrim && !runDijkstra) {
        std::cerr << "Nieobslugiwany algorytm dla wybranego problemu.\n";
        return;
    }

    // Generacja grafu 
    ListGraph*   listG = nullptr;
    MatrixGraph* matG  = nullptr;
    if (isMst) {
        GraphGenerator::generateUndirected(V, density, listG, matG);
    } else {
        GraphGenerator::generateDirected(V, density, listG, matG);
    }

    int E = matG->getEdgeCount();
    const char* probStr = isMst      ? "mst"      : "sp";
    const char* algStr  = runPrim    ? "prim"     : "dijkstra";

    bool doList   = (Parameters::structure == Parameters::Structures::adjacencyList   ||
                     Parameters::structure == Parameters::Structures::allStructures);
    bool doMatrix = (Parameters::structure == Parameters::Structures::incidenceMatrix ||
                     Parameters::structure == Parameters::Structures::allStructures);

    if (doList && listG != nullptr) {
        long long minT, avgT, maxT;
        benchmarkList(listG, isMst, iters, minT, avgT, maxT,
                      Parameters::resultsFile, probStr, algStr, V, E, density);
        std::cout << "[Lista]   " << probStr << "/" << algStr
                  << " V=" << V << " E=" << E << " gestosc=" << density << "%"
                  << " iter=" << iters
                  << " | min=" << minT << " avg=" << avgT
                  << " max=" << maxT << " [us]\n";
    }

    if (doMatrix && matG != nullptr) {
        long long minT, avgT, maxT;
        benchmarkMatrix(matG, isMst, iters, minT, avgT, maxT,
                        Parameters::resultsFile, probStr, algStr, V, E, density);
        std::cout << "[Macierz] " << probStr << "/" << algStr
                  << " V=" << V << " E=" << E << " gestosc=" << density << "%"
                  << " iter=" << iters
                  << " | min=" << minT << " avg=" << avgT
                  << " max=" << maxT << " [us]\n";
    }

    delete listG;
    delete matG;
}

// main
int main(int argc, char** argv) {
    if (Parameters::readParameters(argc - 1, argv + 1) != 0) {
        return 1;
    }

    // Inicjalizacja generatora liczb losowych
    srand(static_cast<unsigned int>(std::time(nullptr)));

    switch (Parameters::runMode) {
        case Parameters::RunModes::help:
            Parameters::help();
            break;

        case Parameters::RunModes::singleFile:
            runSingleFile();
            break;

        case Parameters::RunModes::benchmark:
            runBenchmark();
            break;

        default:
            Parameters::help();
            break;
    }

    return 0;
}

