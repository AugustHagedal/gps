using namespace std;

struct Node {
    double lat, lon;
};

struct Edge {
    int to, w;
};

double haversine(double lat1, double lon1, double lat2, double lon2);
void dijkstra(const vector<vector<Edge>>& graph, int src, vector<int>& dist,vector<int>&prev);
void add_edge(vector<vector<Edge>>& graph,int src, int dest, int w) {
    graph[src].push_back({dest,w});
}

void add_road(vector<vector<Edge>>& G, const vector<Node>& nodes,int u, int v,bool oneway);