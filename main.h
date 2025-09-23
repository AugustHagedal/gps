#pragma once
#include <string>
#include <vector>

using namespace std;

struct Node {
    string type;
    long id;
    long double lat;
    long double lon;
};

struct Edge {
    int to, w;
};

long double haversine(long double lat1, long double lon1, long double lat2, long double lon2);
vector<int> findShortestPath(const vector<vector<Edge>>& graph, int src, int dest);

void add_edge(vector<vector<Edge>>& graph,int src, int dest, int w);

void add_road(vector<vector<Edge>>& G, const vector<Node>& nodes,int u, int v,bool oneway);