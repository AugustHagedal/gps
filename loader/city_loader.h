#pragma once
#include "../main.h"
#include <nlohmann/json.hpp>
#include <string>
#include <vector>
#include <unordered_map>

using json = nlohmann::json;

JSONData loadNodesAndWays(const string& filename);
