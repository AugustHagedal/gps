#include "../main.h"
#include <iomanip>
#include <fstream>
#include <iostream>
#include <limits>


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

JSONData loadNodesAndWays(const string& filename) {
    JSONData data;
    
    try {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Could not open file: " << filename << endl;
            return data;
        }

        json j;
        file >> j;

        // Use YOUR existing logic for nodes
        if (j.contains("elements") && j["elements"].is_array()) {
            for (const auto& element : j["elements"]) {
                if (element.contains("type") && element["type"] == "node") {
                    Node node;
                    node.id = element["id"];
                    node.lat = element["lat"];
                    node.lon = element["lon"];
                    data.id_to_index[node.id] = data.nodes.size();
                    data.nodes.push_back(node);
                }
            }
        }
        
        cout << "Loaded " << data.nodes.size() << " nodes from " << filename << endl;

        // Use YOUR existing logic for ways  
        if (j.contains("elements") && j["elements"].is_array()) {
            for (const auto& element : j["elements"]) {
                if (element.contains("type") && element["type"] == "way") {
                    Ways way = element.get<Ways>(); // Uses YOUR deserialization logic
                    data.ways.push_back(way);
                }
            }
        }
        cout << "Loaded " << data.ways.size() << " ways from " << filename << endl;
        file.close();
    } catch (json::parse_error& e) {
        cerr << "JSON parsing error: " << e.what() << endl;
    } catch (exception& e) {
        cerr << "Error loading file: " << e.what() << endl;
    }
    
    return data;
}