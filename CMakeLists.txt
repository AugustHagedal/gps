cmake_minimum_required(VERSION 3.31)
project(gps)

set(CMAKE_CXX_STANDARD 20)

# Find nlohmann/json
find_package(nlohmann_json 3.2.0 REQUIRED)

add_executable(gps 
main.cpp
loader/city_loader.cpp)

# Link nlohmann/json
target_link_libraries(gps nlohmann_json::nlohmann_json)
