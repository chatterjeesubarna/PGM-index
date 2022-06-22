/*
 * This example shows how to index and query a vector of random integers with the PGM-index.
 * Compile with:
 *   g++ simple.cpp -std=c++17 -I../include -o simple
 * Run with:
 *   ./simple
 */

#include <vector>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <random>
#include <iostream>
#include <fstream>
#include "pgm/pgm_index.hpp"

unsigned long long N = 1000000;
std::vector<int> keys;
std::vector<unsigned int> gaps;

void generateKeys(bool write_to_file, bool print) {
    keys.reserve(N);
    int sum = 0, i = 0;
    while (keys.size() < N) {
        keys.push_back(sum);
        sum = sum + gaps[i];
        i++;
        // printf("%d\n", sum);
    }
    if (print) {
        for(int i = 0;i<keys.size();i++)
        {   
            std::cout << keys[i] << std::endl;
        }   
    }
}

void generateGapsNormal(double mean, double standard_deviation, bool write_to_file, bool print) {
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937_64 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::normal_distribution<> distribution(mean,standard_deviation);

    gaps.reserve(N);
    while (gaps.size() < N) {
        unsigned int gap = distribution(gen);
        // if (print) {
        //     std::cout << gap << std::endl;
        // }
        gaps.push_back(gap);
        if (gap < 0)
        {
            printf("FLAG\n");
            getchar();
        }

        if(write_to_file)
        {
            // fprintf (fp_bulk, "%d\n", (int)key);
        }
    }
    if (print) {
        for(int i = 0;i<gaps.size();i++)
        {   
            std::cout << gaps[i] << std::endl;
        }   
    }
}

void generateGapsUniform(unsigned int a, unsigned int b, bool write_to_file, bool print) {
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937_64 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    double mean = (double)(a + b)/2;
    double standard_deviation = pow( (a-b), 2)/12;
    std::uniform_int_distribution<> distribution(a, b);

    gaps.reserve(N);
    while (gaps.size() < N) {
        unsigned int gap = distribution(gen);
        // if (print) {
        //     std::cout << gap << std::endl;
        // }
        gaps.push_back(gap);
        if (gap < 0)
        {
            printf("FLAG\n");
            getchar();
        }

        if(write_to_file)
        {
            // fprintf (fp_bulk, "%d\n", (int)key);
        }
    }
    if (print) {
        for(int i = 0;i<gaps.size();i++)
        {   
            std::cout << gaps[i] << std::endl;
        }   
    }
}





int main(int argc, char* argv[]) {
    // Generate some random data
    // std::generate(keys.begin(), keys.end(), std::rand);
    // keys.push_back(42);
    double standard_deviation;
    double mean;
    unsigned int a, b;
    const int epsilon = 32; // space-time trade-off parameter
    if(argc <= 1)
    {
        standard_deviation = 5.0;
        mean = 3.0;
    }
    else if(strcmp(argv[1], "0") == 0)  // UNIFORM
    {
        N = atol(argv[2]);
        a = atoi(argv[3]);
        b = atoi(argv[4]);
        printf("distribution: %s data_entries: %llu a: %d b: %d epsilon: %d \n", argv[1], N, a, b, epsilon);
        // generateGapsUniform(a, b, false, false);
    }
    else if(strcmp(argv[1], "1") == 0)  // NORMAL
    {
        N = atol(argv[2]);
        mean = atof(argv[3]);
        standard_deviation = atof(argv[4]);
        printf("distribution: %s data_entries: %llu mean: %.3f standard_deviation: %.3f epsilon: %d \n", argv[1], N, mean, standard_deviation, epsilon);
        // generateGapsNormal(mean, standard_deviation, false, false);
    } 

    // read from file
    std::ifstream inputFile("/Users/subarnachatterjee/Dropbox/Cosine_Learned_Component/code_fun/data.txt");
    // test file open   
    if (inputFile) {        
        int key;

        // read the elements in the file into a vector  
        while ( inputFile >> key ) {
            keys.push_back(key);
        }
    }
    else
    {
        printf("ERROR OPENING FILE\n");
    }
    // close the file
    std::sort(keys.begin(), keys.end());
    

    // Construct the PGM-index
    pgm::PGMIndex<int, epsilon> index(keys);
    printf("\n************************************************************ ACTUAL STRUCTURE (PGM) ************************************************************\n");
    std::cout << "#segments for " << keys.size() << " keys: " << index.segments_count() << std::endl;
    std::cout << "Height: " << index.height() << std::endl;
    index.size_in_bytes();


    // Query the PGM-index
    double sum = 0.0;
    for(int i=0; i<keys.size(); i++)
    {
        auto q = keys[i];
        auto range = index.search(q);
        sum += pgm::count_level;
    }
    sum = (double)sum/(double)N;
    
    std::cout << "average IO per read: " << sum << std::endl;

    // auto q = keys[2000];
    // auto range = index.search(q);
    // auto lo = keys.begin() + range.lo;
    // auto hi = keys.begin() + range.hi;
    // std::cout << "index of " << q << ": " << *std::lower_bound(lo, hi, q) << " and " << range.pos;
    return 0;
}