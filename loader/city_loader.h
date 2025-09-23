#pragma once
#include "../main.h"
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

using json = nlohmann::json;



vector<Node> loadNodes(const string& filename);
