#include "city_loader.h"
#include <fstream>
#include <iostream>


using namespace std;

struct ConfigStruct {
    std::string type;
    int id;
    double lat;
    double lon;
};



namespace nlohmann {
    template <>
    struct adl_serializer<ConfigStruct> {
        static void to_json(json& j, const ConfigStruct& config) {
            j = json{
                {"type", config.type},
                {"id", config.id},
                {"lat", config.lat},
                {"long", config.lon}
            };
        }

        static void from_json(const json& j, ConfigStruct& config) {
            try{
            j.at("type").get_to(config.type);
            j.at("id").get_to(config.id);
            j.at("lat").get_to(config.lat);
            j.at("lon").get_to(config.lon);
            } catch (json::exception& e) {
                std::cerr << "Error parsing JSON: " << e.what() << std::endl;
            }
        }
    };
}