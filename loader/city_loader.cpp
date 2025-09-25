#include "../main.h"
#include <iomanip>
#include <fstream>
#include <iostream>


using namespace std;



namespace nlohmann {
    template <>
    struct adl_serializer<Ways> {
        static void to_json(json& j, const Ways& way) {
            j = json{
                {"id", way.id},
                {"nodes", way.nodes},
                {"name", way.name},
                {"oneway", way.oneway}
            };
        }

        static void from_json(const json& j, Ways& way) {
            string boolway;
            try {
                j.at("id").get_to(way.id);
                j.at("nodes").get_to(way.nodes);
                if (j.contains("tags") && j["tags"].is_object()) {
                    if (j["tags"].contains("name")) {
                        j["tags"]["name"].get_to(way.name);
                    }
                    if (j["tags"].contains("oneway")){
                        j["tags"]["oneway"].get_to(boolway);
                        if(boolway=="yes"){
                            way.oneway = true;
                        }
                        else way.oneway = false;
                    }
                }
            } catch (json::exception& e) {
                std::cerr << "Error parsing Ways JSON: " << e.what() << std::endl;
                way.id = -1;
                way.nodes.clear();
                way.name = "Unknown";
            }
        }
    };
}

namespace nlohmann {
    template <>
    struct adl_serializer<Node> {
        static void to_json(json& j, const Node& config) {
            j = json{
                {"id", config.id},
                {"lat", config.lat},
                {"lon", config.lon}
            };
        }

        static void from_json(const json& j, Node& config) {
            try {
                config.id = 0;
                config.lat = 0.0f;
                config.lon = 0.0f;
                
                if (j.contains("id")) j.at("id").get_to(config.id);
                if (j.contains("lat")) j.at("lat").get_to(config.lat);
                if (j.contains("lon")) j.at("lon").get_to(config.lon);
                
            } catch (json::exception& e) {
                std::cerr << "Error parsing JSON: " << e.what() << std::endl;
                config.id = -1;
                config.lat = 0.0f;
                config.lon = 0.0f;
            }
        }
    };
}



vector<Ways> loadWays(const string& filename) {
    vector<Ways> ways;

    try {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Could not open file: " << filename << endl;
            return ways;
        }

        json j;
        file >> j;

        if (j.contains("elements") && j["elements"].is_array()) {
            for (const auto& element : j["elements"]) {
                if (element.contains("type") && element["type"] == "way") {
                    Ways way = element.get<Ways>();
                    ways.push_back(way);
                }
            }
        }

        cout << "Loaded " << ways.size() << " ways from " << filename << endl;

    } catch (json::exception& e) {
        cerr << "JSON parsing error: " << e.what() << endl;
    } catch (exception& e) {
        cerr << "Error loading file: " << e.what() << endl;
    }

    return ways;
}

pair<vector<Node>, unordered_map<long long, size_t>> loadNodes(const string& filename) {
    vector<Node> nodes;
    unordered_map<long long, size_t> id_to_index;
    size_t idx = 0;

    
    try {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Could not open file: " << filename << endl;
            return {nodes, id_to_index};
        }
        
        json j;
        file >> j;
        
        if (j.contains("elements") && j["elements"].is_array()) {
            for (json element : j["elements"]) {
                Node config = element.get<Node>();

                

                if (config.lat != 0.0 && config.lon != 0.0) {
                    nodes.push_back(config);
                    id_to_index[config.id] = idx;
                    idx++;  
                }
            
            }
        }
        cout << "Loaded " << nodes.size() << " nodes from " << filename << endl;

        
    } catch (json::exception& e) {
        cerr << "JSON parsing error: " << e.what() << endl;
    } catch (exception& e) {
        cerr << "Error loading file: " << e.what() << endl;
    }
    
    return {nodes, id_to_index};
}