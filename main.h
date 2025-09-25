#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <nlohmann/json.hpp>
#include <unordered_map>

using namespace std;
using json = nlohmann::json;



struct Node {
    std::string type;
    long long id;
    long double lat;
    double lon;
};

struct Ways {
    std::string type;
    long long id;
    vector<long long> nodes;
    string name;
    bool oneway;
};

struct Edge {
    int to, w;
};





long double haversine(long double lat1, long double lon1, long double lat2, long double lon2);

vector<int> findShortestPath(const vector<vector<Edge>>& graph, int src, int dest);

void add_edge(vector<vector<Edge>>& graph,int src, int dest, int w);

void add_road(vector<vector<Edge>>& G, const vector<Node>& nodes,int u, int v,bool oneway);