#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <nlohmann/json.hpp>
#include <unordered_map>

using namespace std;
using json = nlohmann::json;



struct Node {
    long id;
    float lat, lon;
};

struct Ways {
    long id;
    vector<long> nodes;
    string name;
    bool oneway;
};

struct Edge {
    int to, w;
};


struct CSR {
    vector<Edge> edges;
    vector<size_t> row_ptr;
};

struct JSONData {
    vector<Node> nodes;
    unordered_map<long long, size_t> id_to_index;
    vector<Ways> ways;
};


long double haversine(long double lat1, long double lon1, long double lat2, long double lon2);


vector<int> findShortestPath(const CSR& csr, int src, int dest);


void count_road_edges(vector<int>& edge_count, int u, int v, bool oneway);
void add_road(CSR& csr, vector<size_t>& current_pos, const vector<Node>& nodes, int u, int v, bool oneway);

JSONData loadNodesAndWays(const string& filename);