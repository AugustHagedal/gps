#include "city_loader.h"
#include <iomanip>
#include <fstream>
#include <iostream>


using namespace std;





namespace nlohmann {
    template <>
    struct adl_serializer<Node> {
        static void to_json(json& j, const Node& config) {
            j = json{
                {"type", config.type},
                {"id", config.id},
                {"lat", config.lat},
                {"lon", config.lon}
            };
        }

        static void from_json(const json& j, Node& config) {
            try {
                // Initialize with defaults
                config.type = "";
                config.id = 0;
                config.lat = 0.0L;
                config.lon = 0.0L;
                
                // Only parse fields that exist
                if (j.contains("type")) j.at("type").get_to(config.type);
                if (j.contains("id")) j.at("id").get_to(config.id);
                if (j.contains("lat")) j.at("lat").get_to(config.lat);
                if (j.contains("lon")) j.at("lon").get_to(config.lon);
                
            } catch (json::exception& e) {
                std::cerr << "Error parsing JSON: " << e.what() << std::endl;
                // Set safe defaults on error
                config.type = "unknown";
                config.id = -1;
                config.lat = 0.0L;
                config.lon = 0.0L;
            }
        }
    };
}


vector<Node> loadNodes(const string& filename) {
    vector<Node> nodes;
    
    try {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Could not open file: " << filename << endl;
            return nodes;
        }
        
        json j;
        file >> j;
        
        if (j.contains("elements") && j["elements"].is_array()) {
            for (json element : j["elements"]) {
                Node config = element.get<Node>();

                

                if (config.type == "node" && config.lat != 0.0 && config.lon != 0.0) {
                    nodes.push_back(config);
                }
            
            }
        }
        
        
    } catch (json::exception& e) {
        cerr << "JSON parsing error: " << e.what() << endl;
    } catch (exception& e) {
        cerr << "Error loading file: " << e.what() << endl;
    }
    
    return nodes;
}