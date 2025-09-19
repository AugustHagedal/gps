#include <iostream>
#include <vector>
#include <math.h>
#include "main.h"


int main() {
    vector <Node> Nodes = {
        {57.0463891, 9.9135551},
        {57.0467164,9.9141827},
        {57.0461667,9.9148274},
        {57.0470625, 9.9145806},
        {57.0478946,9.9155201},
        {57.0475035, 9.9167926},
        {57.0466836, 9.9160344},
        {57.0461981, 9.9188502},
        {57.0467068, 9.9147912},
        {57.0460351, 9.9156376},
        {57.0458007, 9.9152305},
    };
    int n = Nodes.size();
    vector<vector<Edge>> Graph(n);

    add_road(Graph,Nodes,0,1, true);
    add_road(Graph,Nodes,1,2, true);
    add_road(Graph,Nodes,2,10, false);
    add_road(Graph,Nodes,1,3, true);
    add_road(Graph,Nodes,3,4, true);
    add_road(Graph,Nodes,4,5, true);
    add_road(Graph,Nodes,5,6, true);
    add_road(Graph,Nodes,6,7, true);
    add_road(Graph,Nodes,6,8, true);
    add_road(Graph,Nodes,8,9, true);
    add_road(Graph,Nodes,9,10, true);

    vector<int> dist,prev;
    int src = 0;
    dijkstra(Graph, src, dist, prev);
    cout << dist[10] << endl;

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
                cout << u << "->" << v << " afstand: " << e.w << "m" << " samlet afstand: " << nd << "m"<< endl;
                dist[v] = nd;
                prev[v] = u;
                pq.push({nd, v});
            }
        }
    }
}


double haversine(double lat1, double lon1, double lat2, double lon2) {
    const double R = 6378000.0;
    auto rad = [](double deg){return deg * M_PI / 180;};
    double dlat = rad(lat2-lat1);
    double dlon = rad(lon2-lon1);
    double distance = sin(dlat/2) * sin(dlat/2)
    +cos(rad(lat1))*cos(rad(lat2))*sin(dlon/2)*sin(dlon/2);
    return 2 * R * asin(sqrt(distance));
}

void add_road(vector<vector<Edge>>& G, const vector<Node>& nodes,int u, int v, bool oneway){
    int w = (int)llround(haversine(nodes[u].lat,nodes[u].lon,
    nodes[v].lat,nodes[v].lon));

    G[u].push_back({v,w});
    if (!oneway) G[v].push_back({u,w});
}
