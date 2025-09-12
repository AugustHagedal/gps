#include <iostream>
#include <vector>
using namespace std;

struct Node {
    double lat, lon;
};

struct Edge {
    int to, w;
};

void dijkstra(const vector<vector<Edge>>& graph, int src, vector<int>& dist,vector<int>&prev);
void add_edge(vector<vector<Edge>>& graph,int src, int dest, int w) {
    graph[src].push_back({dest,w});
}

int main() {
    int n = 6;
    vector<vector<Edge>> Graph(n);
    add_edge(Graph,0,1,4);
    add_edge(Graph,0,2,2);
    add_edge(Graph,1,4,1);
    add_edge(Graph,2,1,1);
    add_edge(Graph,3,4,3);
    add_edge(Graph,3,5,6);
    add_edge(Graph,4,5,3);


    vector<int> dist,prev;
    int src = 0;
    dijkstra(Graph, src, dist, prev);

    cout << dist[5] << endl;
}


void dijkstra(const vector<vector<Edge>>& graph, int src, vector<int>& dist,vector<int>&prev) {
    int n = graph.size();
    const int INF = numeric_limits<int>::max();
    dist.assign(n, INF);
    prev.assign(n, -1);

    using P = pair<int,int>;
    priority_queue<P, vector<P>, greater<P>> pq;

    dist[src] = 0;
    pq.push({0, src});
    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d != dist[u]) continue;
        for (const auto e : graph[u]) {
            int nd = d+e.w;
            int v = e.to;
            if (nd < dist[v]) {
                cout << u << "->" << v << " koster: " << e.w << " samlet: " << nd << endl;
                dist[v] = nd;
                prev[v] = u;
                pq.push({nd, v});
            }
        }
    }
}

