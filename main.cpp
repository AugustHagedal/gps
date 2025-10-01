#include <iostream>
#include <vector>
#include <math.h>
#include "main.h"
#include "loader/city_loader.h"

using namespace std;


int main() {
    CSR csr;

    if( !ifstream("../berlin_roads.bin")) {
        cout << "Binary file not found, creating it..." << endl;
        csr = loadToBinCSR("berlin_roads");
        cout << "Binary file created." << endl;
    } 
    else {
        cout << "Loading binary file..." << endl;
        csr = loadFromBinCSR("berlin_roads");
        cout << "Binary file loaded." << endl;
    }


    vector<int> path = findShortestPath(csr, 54613, 157705);
    
  /* if (!path.empty()) {
        cout << "Shortest path: ";
        for (size_t i = 0; i < path.size(); ++i) {
            cout << nodes[path[i]].id;
            if (i < path.size() - 1) cout << " -> ";
        }
        cout << endl;
    } else {
        cout << "No path found!" << endl;
    }*/
    return 0;
}


vector<int> findShortestPath(const CSR& csr, int src, int dest) {
    int n = csr.row_ptr.size() - 1;
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

        for (int i = csr.row_ptr[u]; i < csr.row_ptr[u + 1]; ++i) {
            const auto& e = csr.edges[i];
            int newdist = d + e.w;
            int v = e.to;
            if (newdist < dist[v]) {
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
        cout << "Afstand fra: " << src <<" til: " <<dest<< " er: " << dist[dest]/1000 <<"."<< dist[dest]%1000/100 <<" kilometer" <<endl;
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




void count_road_edges(vector<int>& edge_count, int u, int v, bool oneway) {
    edge_count[u]++;
    if (!oneway) edge_count[v]++;
}

void add_road(CSR& csr, vector<size_t>& current_pos, const vector<Node>& nodes, int u, int v, bool oneway) {
    int w = (int)llround(haversine(nodes[u].lat, nodes[u].lon, nodes[v].lat, nodes[v].lon));
    
    csr.edges[current_pos[u]] = {v, w};
    current_pos[u]++;
    
    if (!oneway) {
        csr.edges[current_pos[v]] = {u, w};
        current_pos[v]++;
    }
}

