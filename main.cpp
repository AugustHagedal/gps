#include <iostream>
#include <vector>
#include <math.h>
#include "main.h"
#include "loader/city_loader.h"


using namespace std;


int main() {

    vector <Node> Nodes = loadNodes("../data/aalborg_roads.json");
    int n = Nodes.size();
    vector<vector<Edge>> Graph(n);

    cout << std::fixed << std::setprecision(10); 
    for(int i = 0; i < 100; i++){
        cout <<"Number: " << i+1 << " Node id: " << Nodes[i].id << " Node lat: " << Nodes[i].lat << endl;
    }

}


vector<int> findShortestPath(const vector<vector<Edge>>& graph, int src, int dest) {
    int n = graph.size();
    const int INF = numeric_limits<int>::max();
    vector<int> dist(n, INF);
    vector<int> prev(n, -1);


    using P = pair<int,int>;
    priority_queue<P, vector<P>, greater<P>> pq;

    dist[src] = 0;
    pq.push({0, src});
    
    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        
        if (u == dest) break;
        
        if (d != dist[u]) continue;
        
        for (const auto e : graph[u]) {
            int newdist = d + e.w;
            int v = e.to;
            if (newdist < dist[v]) {
                cout << u << "->" << v << " afstand: " << e.w << "m" << " samlet afstand: " << newdist << "m"<< endl;
                dist[v] = newdist;
                prev[v] = u;
                pq.push({newdist, v});
            }
        }
    }
    
    vector<int> path;
    if (dist[dest] == INF) return path; 
    
    for (int at = dest; at != -1; at = prev[at]) {
        path.push_back(at);
    }
    reverse(path.begin(), path.end());
    return path;
}



long double haversine(long double lat1, long double lon1, long double lat2, long double lon2) {
    const long double R = 6378000.0L;
    auto rad = [](long double deg){return deg * M_PI / 180.0L;};
    long double dlat = rad(lat2-lat1);
    long double dlon = rad(lon2-lon1);
    long double distance = sin(dlat/2.0L) * sin(dlat/2.0L)
    +cos(rad(lat1))*cos(rad(lat2))*sin(dlon/2.0L)*sin(dlon/2.0L);
    return 2.0L * R * asin(sqrt(distance));
}

void add_road(vector<vector<Edge>>& G, const vector<Node>& nodes,int u, int v, bool oneway){
    int w = (int)llround(haversine(nodes[u].lat,nodes[u].lon,
    nodes[v].lat,nodes[v].lon));

    G[u].push_back({v,w});
    if (!oneway) G[v].push_back({u,w});
}

void add_edge(vector<vector<Edge>>& graph,int src, int dest, int w) {
    graph[src].push_back({dest,w});
}
