/*
 * This example shows how to use pgm::DynamicPGMIndex, a std::map-like container supporting inserts and deletes.
 * Compile with:
 *   g++ updates.cpp -std=c++17 -I../include -o updates
 * Run with:
 *   ./updates
 */

#include <vector>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <random>
#include <fstream>
#include "pgm/pgm_index_dynamic.hpp"

int main() {
    // Generate some random key-value pairs to bulk-load the Dynamic PGM-index
    // std::vector<std::pair<uint32_t, uint32_t>> data(1000000);
    // std::generate(data.begin(), data.end(), [] { return std::make_pair(std::rand(), std::rand()); });
    // std::sort(data.begin(), data.end());

    // read from file
    unsigned long long N = 1000000;
    std::vector<std::pair<uint32_t, uint32_t>> keys;
    std::vector<std::pair<uint32_t, uint32_t>> inserts;
    std::ifstream inputFile("/Users/subarnachatterjee/Dropbox/Cosine_Learned_Component/code_fun/data.txt");
    std::ifstream insertFile("/Users/subarnachatterjee/Dropbox/Cosine_Learned_Component/code_fun/insert.txt");
    // test file open   
    if (inputFile) {        
        int key;

        // read the elements in the file into a vector  
        while ( inputFile >> key ) {
            keys.push_back(std::make_pair(key, key));
        }
    }
    else
    {
        printf("ERROR OPENING FILE\n");
    }
    // close the data file
    std::sort(keys.begin(), keys.end());

    // Construct and bulk-load the Dynamic PGM-index
    pgm::DynamicPGMIndex<uint32_t, uint32_t> dynamic_pgm(keys.begin(), keys.end());

    // get insert data
    if (insertFile) {        
        int key;
        // read the elements in the file into a vector  
        while ( insertFile >> key ) {
            inserts.push_back(std::make_pair(key, key));
        }
    }
    else
    {
        printf("ERROR OPENING FILE\n");
    }

    // Insert into the PGM-index
    int sum_IOs = 0;
    int insert_count = 0;
    for(int i=0; i<inserts.size(); i++)
    {
        dynamic_pgm.insert_or_assign(inserts[i].first, inserts[i].second);

        sum_IOs += IOs;
        insert_count++;
        // std::cout << "IO: " << IOs << std::endl;
    }
    sum_IOs = (double)sum_IOs/(double)insert_count;
    
    std::cout << "average IO per insert: " << sum_IOs << std::endl;
    std::cout << "inserted " << insert_count << " keys." << std::endl;

    return 0;
}