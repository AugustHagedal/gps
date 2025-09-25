#include <iostream>
#include <vector>
#include <math.h>
#include "main.h"
#include "loader/city_loader.h"

using namespace std;


int main() {

    auto result = loadNodes("../data/berlin_roads.json");
    vector<Node> nodes = result.first;
    unordered_map<long long, size_t> id_to_index = result.second;
    vector <Ways> ways = loadWays("../data/berlin_roads.json");

    int n = nodes.size();
    vector<vector<Edge>> Graph(n);
    
    int prev_index = -1;


    for(Ways way: ways){
        n = way.nodes.size();
        prev_index = -1;
        for(long long wn: way.nodes){
            auto it = id_to_index.find(wn);
            if (it != id_to_index.end()) { 
                if(prev_index != -1 && prev_index != it->second){
                    add_road(Graph, nodes, prev_index, it->second, way.oneway); 
                    //cout << "Road added from: " << prev_index << " to: " <<it->second <<endl;
                }
                    prev_index = it->second;
                    if(it->first==4612182490||it->first==11396807446){
                    cout << way.name << "   Node: "<< it->first << " at: " << it->second <<endl;
                    }
                
            }
        }
    }
    vector<int> path = findShortestPath(Graph,138284,51934);
    
   /*if (!path.empty()) {
        cout << "Shortest path: ";
        for (size_t i = 0; i < path.size(); ++i) {
            cout << nodes[path[i]].id;
            if (i < path.size() - 1) cout << " -> ";
        }
        cout << endl;
    } else {
        cout << "No path found!" << endl;
    }*/


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
              //  cout << u << "->" << v << " afstand: " << e.w << "m" << " samlet afstand: " << newdist << "m"<< endl;
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

    if(dist[dest]>1000){
        int kmval = dist[dest]/1000;
        double meterval = (dist[dest]-1000*kmval)/100;
        cout << "Afstand fra: " << src <<" til: " <<dest<< " er: " << kmval <<"."<<meterval<<" kilometer" <<endl;
    }
    else 
        cout << "Afstand fra: " << src <<" til: " <<dest<< " er: " <<dist[dest] <<" meter" <<endl;
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
