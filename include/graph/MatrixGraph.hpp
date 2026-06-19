#ifndef MATRIXGRAPH_HPP
#define MATRIXGRAPH_HPP

class MatrixGraph{
private:
    int** matrix;  //sasiedztwa
    int* edge_weights; //wagi 
    int num_vertices; //liczba wierzcholkow
    int num_edges; //liczba krawedzi
    int current_edge_idx; //pom licznik krawedzi

public:
    // Konstruktor pamiec dokładnie pod graf
    MatrixGraph(int vertices, int edges) {
        num_vertices = vertices;
        num_edges = edges;
        current_edge_idx = 0;

        //tablica wskaznikow na wieszcholki
        matrix = new int*[num_vertices];
        for(int i=0; i<num_vertices; ++i){
            matrix[i] = new int[num_edges]{0}; //wyzerowanie brak polaczenia
        }

        edge_weights = new int[num_edges]{0}; //alokacja i wyzerowanie wag
    }

    //destruktor-  pamiec us kazdy wiersz(wierzcholek)
    ~MatrixGraph() {
        for(int i=0; i<num_vertices; ++i){
            delete[] matrix[i];
        }
        delete[] matrix; //us tab wskaznik
        delete[] edge_weights; //us tab wag
    }

    //+ krawedz skierowana (-1 start, +1 koniec)
    void addDirectedEdge(int start, int end, int weight) {
        if (current_edge_idx >= num_edges) return;
        matrix[start][current_edge_idx] = -1;
        matrix[end][current_edge_idx] = 1;
        edge_weights[current_edge_idx] = weight;
        current_edge_idx++;
    }

    //+ krawedz nieskierowana (+1 oba konce)
    void addUndirectedEdge(int u, int v, int weight) {
        if (current_edge_idx >= num_edges) return;
        matrix[u][current_edge_idx] = 1;
        matrix[v][current_edge_idx] = 1;
        edge_weights[current_edge_idx] = weight;
        current_edge_idx++;
    }

    int getNumVertices() const { return num_vertices; }
    int getNumEdges()    const { return num_edges; }
    int getEdgeCount()   const { return current_edge_idx; }
    int getCell(int v, int e)  const { return matrix[v][e]; }
    int getEdgeWeight(int e)   const { return edge_weights[e]; }
};

#endif 