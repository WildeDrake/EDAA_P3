/** Testing effect of number of occurrences
 *
 * Things to set up:
 * 0. Number of runs: in RUNS,
 * 1. LOWER, UPPER, STEP bounds for test type,
 * 2. Time unit: in elapsed_time,
 * 3. Output file location: in time_data.open,
 * 4. What to write on time_data,
 * 5. The experiments: in outer for loop. */

#include <chrono>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

#include "FM_index.hpp"

#define RUNS 64

int main(int argc, char *argv[])
{
    if (argc == 1) {
        std::cerr << "Add docs location as command-line arguments." << std::endl;
        return 1;
    }

    // Set up clock variables
    int64_t i, j;
    double time[RUNS];
    double mean_time, time_stdev, dev;
    auto begin_time = std::chrono::high_resolution_clock::now();
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_time = end_time - begin_time;

    // Load documents
    std::vector<std::string> docs(argc - 1);
    for (i = 1; i < argc; i++) {
        std::ifstream text_file(argv[i]);
        std::stringstream text_buffer;
        text_buffer << text_file.rdbuf();
        docs[i - 1] = text_buffer.str();
    }

    // Load patterns
    // Selected before and r's where precomputed
    std::vector<std::string> dblp = {"articles", "domain", "hall", "presentation", "arxiv", "corr", "cs", "year", "article", "author"};
    std::vector<uint64_t> dblp_r = {23, 291, 679, 1390, 4556, 14359, 51663, 252991, 254360, 529792};
    std::vector<std::string> proteins = {"BB", "U", "VVADA", "AAAA", "CC", "X", "DD", "AA", "T", "G", "A"};
    std::vector<uint64_t> proteins_r = {3, 99, 131, 10231, 18557, 30901, 132207, 457547, 3008372, 3614402, 4381677};
    std::vector<std::string> sources = {"xz", "aba", "while", "include", "for", "struct", "int", "i"};
    std::vector<uint64_t> sources_r = {5, 165, 6092, 24886, 48288, 91290, 133632, 1806428};

    // Create FM_index
    FM_index fm_index(docs);

    // File to write FM-index construction doc split time data
    std::ofstream time_data;

    time_data.open("experimental_data/FM_index/sources/locate_r_s.csv");
    time_data << "r,t_mean,t_stdev" << std::endl;

    // Begin testing of FM-index search
    for (j = 0; j < dblp.size(); j++) {
        mean_time = 0;
        time_stdev = 0;

        // Run to compute elapsed time
        for (i = 0; i < RUNS; i++) {
    	    begin_time = std::chrono::high_resolution_clock::now();
            // Function to test goes here
            fm_index.locate(sources[j]);
            end_time = std::chrono::high_resolution_clock::now();

            elapsed_time = end_time - begin_time;
            time[i] = elapsed_time.count();

            mean_time += time[i];
        }

        // Compute statistics
        mean_time /= RUNS;

        for (i = 0; i < RUNS; i++) {
            dev = time[i] - mean_time;
            time_stdev += dev * dev;
        }

        time_stdev /= RUNS - 1; // Subtract 1 to get unbiased estimator
        time_stdev = sqrt(time_stdev);

        time_data << sources_r[j] << "," << mean_time << "," << time_stdev << std::endl;
    }

    time_data.close();

    return 0;
}
