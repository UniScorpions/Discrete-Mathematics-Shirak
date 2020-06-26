#include <iostream> 
#include <algorithm>
#include <stack> 
#include <list> 

constexpr auto NIL = -1;

using namespace std;

class Edge {
public:
    int u;
    int v;
    Edge(int u, int v);
};
Edge::Edge(int u, int v) {
    this->u = u;
    this->v = v;
};

class Graph {
public:
    Graph(int V);
    ~Graph();
    int biCount() const;
    void addEdge(int v, int w);
    void BCC();
private:
    int V;
    int E;
    list<int>* adj;
    int biconnectedCount;
    void BCCUtil(int u, int disc[], int low[], list<Edge>* st, int parent[]);
};

Graph::Graph(int V) : 
    V{ V }, 
    E{ 0 }, 
    adj{ new list<int>[V] },
    biconnectedCount{ 0 } {}

Graph::~Graph() {
    delete[] adj;
}

void Graph::addEdge(int v, int w) {
    adj[v].push_back(w);
    adj[w].push_back(v);  
    E++;
}

int Graph::biCount() const {
    return biconnectedCount;
}

void Graph::BCCUtil(int u, int disc[], int low[], list<Edge>* st, int parent[]) {
    static int d = 0; 

    disc[u] = low[u] = ++d;
    int children = 0;

    for (auto i = adj[u].begin(); i != adj[u].end(); ++i) {
        int v = *i; 

        if (disc[v] == -1) {
            children++;
            parent[v] = u;

            st->push_back(Edge{ u, v });
            BCCUtil(v, disc, low, st, parent);

            low[u] = min(low[u], low[v]);

            if ((disc[u] == 1 && children > 1) || (disc[u] > 1 && low[v] >= disc[u])) {
                while (st->back().u != u || st->back().v != v) {
                    cout << st->back().u << "--" << st->back().v << " ";
                    st->pop_back();
                }
                cout << st->back().u << "--" << st->back().v;
                st->pop_back();
                cout << endl;
                biconnectedCount++;
            }
        }

        else if (v != parent[u]) {
            low[u] = min(low[u], disc[v]);
            if (disc[v] < disc[u]) {
                st->push_back(Edge(u, v));
            }
        }
    }
}

void Graph::BCC() {
    int* disc = new int[V];
    int* low = new int[V];
    int* parent = new int[V];
    list<Edge>* st = new list<Edge>[E];

    for (int i = 0; i < V; i++) {
        disc[i] = NIL;
        low[i] = NIL;
        parent[i] = NIL;
    }

    biconnectedCount = 0;

    for (int i = 0; i < V; i++) {
        if (disc[i] == NIL)
            BCCUtil(i, disc, low, st, parent);

        bool j = false;
        while (st->size() > 0) {
            j = true;
            cout << st->back().u << "--" << st->back().v << " ";
            st->pop_back();
        }
        if (j) {
            cout << endl;
            biconnectedCount++;
        }
    }
}

int main() {
    Graph g(12);
    g.addEdge(0, 1);
    g.addEdge(2, 1);
    g.addEdge(3, 1);
    g.addEdge(3, 2);
    g.addEdge(4, 2);
    g.addEdge(4, 3);
    g.addEdge(5, 1);
    g.addEdge(6, 0);
    g.addEdge(6, 5);
    g.addEdge(7, 5);
    g.addEdge(8, 5);
    g.addEdge(8, 7);
    g.addEdge(9, 8);
    g.addEdge(11, 10);
    g.BCC();
    cout << "Above are " << g.biCount() << " biconnected components in graph";
}