#include <iostream>
#include "main.h"
#include "loader/city_loader.h"


CSR loadToBinCSR(const string& filename) {
    JSONData data = loadNodesAndWays("../data/" + filename + ".json");
    unordered_map<long long, size_t> id_to_index = data.id_to_index;
    int n = data.nodes.size();

    vector<int> edge_count(n, 0);

    
    int prev_index = -1;
    for(const Ways& way: data.ways){  
        if(way.nodes.size() < 2) continue;
        
        prev_index = -1;
        for(long long wn: way.nodes){
            auto it = id_to_index.find(wn);
            if (it != id_to_index.end()) { 
                if(prev_index != -1 && prev_index != it->second){
                    count_road_edges(edge_count, prev_index, it->second, way.oneway); 
                }
                prev_index = it->second;
            }
        }
    }


    CSR csr;
    csr.row_ptr.resize(n + 1);
    csr.row_ptr[0] = 0;
    for(int i = 0; i < n; i++) {
        csr.row_ptr[i + 1] = csr.row_ptr[i] + edge_count[i];
    }
    csr.edges.resize(csr.row_ptr[n]);
    
    vector<size_t> current_pos = csr.row_ptr;
    


    prev_index = -1;
    for(const Ways& way: data.ways){  
        if(way.nodes.size() < 2) continue;
        
        prev_index = -1;
        for(long long wn: way.nodes){
            auto it = id_to_index.find(wn);
            if (it != id_to_index.end()) { 
                if(prev_index != -1 && prev_index != it->second){
                    add_road(csr, current_pos, data.nodes, prev_index, it->second, way.oneway); 
                }
                prev_index = it->second;
            }
        }
    }
    ofstream fout("../" + filename + ".bin", ios::out | ios::binary);
    if(fout){

        size_t row_size = csr.row_ptr.size();
        size_t edge_size = csr.edges.size();
        size_t combined_size = row_size + edge_size;
        fout.write(reinterpret_cast<char*>(&row_size), sizeof(row_size));
        fout.write(reinterpret_cast<char*>(&edge_size), sizeof(edge_size));

        fout.write(reinterpret_cast<char*>(csr.row_ptr.data()), row_size * sizeof(size_t));
        fout.write(reinterpret_cast<char*>(csr.edges.data()), edge_size * sizeof(Edge));
        fout.close();
    }
    return csr;
}


CSR loadFromBinCSR(const string& filename) {
    CSR csr;
    ifstream fin("../" + filename + ".bin", ios::in | ios::binary);
    if(fin){
        size_t row_ptr_size = 0;
        size_t edge_size = 0;
        fin.read(reinterpret_cast<char*>(&row_ptr_size), sizeof(row_ptr_size));
        fin.read(reinterpret_cast<char*>(&edge_size), sizeof(edge_size));
        csr.row_ptr.resize(row_ptr_size);
        csr.edges.resize(edge_size);
        fin.read(reinterpret_cast<char*>(csr.row_ptr.data()), csr.row_ptr.size() * sizeof(size_t));
        fin.read(reinterpret_cast<char*>(csr.edges.data()), csr.edges.size() * sizeof(Edge));
        fin.close();
    }
    return csr;
}