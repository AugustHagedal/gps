#pragma once
#include "../main.h"
#include <nlohmann/json.hpp>
#include <string>
#include <vector>
#include <unordered_map>

using json = nlohmann::json;

pair<vector<Node>, unordered_map<long long, size_t>> loadNodes(const string& filename);
vector<Ways> loadWays(const string& filename);
