/** Testing construction time
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

#include "suffix_array.hpp"

#define RUNS 64

int main(int argc, char *argv[])
{
    if (argc == 1) {
        std::cerr << "Add docs location as command-line arguments." << std::endl;
        return 1;
    }

    // Set up clock variables
    int64_t i, j, N;
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

    // File to write Suffix Array construction doc split time data
    std::ofstream time_data;

    time_data.open("experimental_data/suffix_array/proteins/construct_d_s.csv");
    time_data << "N,t_mean,t_stdev" << std::endl;

    // Begin testing of Suffix Array search
    for (j = 0; j < docs.size(); j++) {
        mean_time = 0;
        time_stdev = 0;

        // Run to compute elapsed time
        for (i = 0; i < RUNS; i++) {
            begin_time = std::chrono::high_resolution_clock::now();
            // Function to test goes here
            suffix_array SA(std::vector<std::string>(docs.begin(), docs.begin() + j + 1));
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

        N = 0;
        for (i = 0; i <= j; i++)
            N += docs[i].length() + 1;

        time_data << N << "," << mean_time << "," << time_stdev << std::endl;
    }

    time_data.close();

    // File to write Suffix Array construction no doc split time data
    time_data.open("experimental_data/suffix_array/proteins/construct_nd_s.csv");
    time_data << "N,t_mean,t_stdev" << std::endl;

    // Begin testing of Suffix Array search
    char ETX = 3;
    std::string t = docs[0];
    for (j = 0; j < docs.size(); j++) {
        mean_time = 0;
        time_stdev = 0;

        if (j != 0)
            t += ETX + docs[j];

        // Run to compute elapsed time
        for (i = 0; i < RUNS; i++) {
            begin_time = std::chrono::high_resolution_clock::now();
            // Function to test goes here
            suffix_array SA({t});
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

        time_data << t.length() + 1 << "," << mean_time << "," << time_stdev << std::endl;
    }

    time_data.close();

    return 0;
}
