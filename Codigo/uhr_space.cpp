/** Testing construction space
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
#include "suffix_array.hpp"

int main(int argc, char *argv[])
{
    if (argc == 1) {
        std::cerr << "Add docs location as command-line arguments." << std::endl;
        return 1;
    }

    int64_t i, j, N;

    // Load documents
    std::vector<std::string> docs(argc - 1);
    for (i = 1; i < argc; i++) {
        std::ifstream text_file(argv[i]);
        std::stringstream text_buffer;
        text_buffer << text_file.rdbuf();
        docs[i - 1] = text_buffer.str();
    }

    // File to write Suffix Array space data
    std::ofstream time_data;
    time_data.open("experimental_data/suffix_array/sources/space_mib.csv");
    time_data << "N,space" << std::endl;

    // Begin testing
    for (j = 0; j < docs.size(); j++) {
        suffix_array SA(std::vector<std::string>(docs.begin(), docs.begin() + j + 1));

        N = 0;
        for (i = 0; i <= j; i++)
            N += docs[i].length() + 1;

        time_data << N << "," << SA.size() << std::endl;
    }

    time_data.close();

    // File to write Suffix Array space data
    time_data.open("experimental_data/FM_index/sources/space_mib.csv");
    time_data << "N,space" << std::endl;

    // Begin testing
    for (j = 0; j < docs.size(); j++) {
        FM_index fm_index(std::vector<std::string>(docs.begin(), docs.begin() + j + 1));

        N = 0;
        for (i = 0; i <= j; i++)
            N += docs[i].length() + 1;

        time_data << N << "," << fm_index.size() << std::endl;
    }

    time_data.close();

    return 0;
}
