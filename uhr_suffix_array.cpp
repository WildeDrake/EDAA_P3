#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "suffix_array.cpp"

#define RUNS 200

#define TEXT "banana.txt" // name of text file

int main(void)
{
    // Set up clock variables
    int64_t n, i, j, k;
    double preprocess_time[RUNS];
    double search_time[RUNS];
    double preprocess_mean_time, preprocess_time_stdev;
    double search_mean_time, search_time_stdev;
    auto begin_time = std::chrono::high_resolution_clock::now();
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed_time = end_time - begin_time;

    // Files to write time data
    std::ofstream preprocess;
    std::ofstream search;
    preprocess.open("experimental_data/suffix_array/preprocess_ms.csv");
    search.open("experimental_data/suffix_array/search_ms.csv");

    preprocess << "m,t_mean,t_stdev" << std::endl;
    search << "m,t_mean,t_stdev" << std::endl;

    // Set up text and patterns
    std::string_view s, t;

    std::ifstream text_file(TEXT);
    std::stringstream text_buffer;
    text_buffer << text_file.rdbuf();
    std::string text = text_buffer.str();
    n = text.length();
    t = text.substr(0, n);

    std::vector<std::string> pattern(8);

    i = 0;
    for (k = 0; k < 7; k++) {
        j = 1 << (2 * k + 3);
        pattern[k] = text.substr(i, j);
        i = j;
    }

    // Begin testing

    for (j = 0; j < pattern.size(); j++) {
        preprocess_mean_time = 0;
        preprocess_time_stdev = 0;
        search_mean_time = 0;
        search_time_stdev = 0;

        s = pattern[j];

        // Run to compute elapsed time
        for (i = 0; i < RUNS; i++) {
            // Time preprocessing
            begin_time = std::chrono::high_resolution_clock::now();
            suffix_array SA(text);
            end_time = std::chrono::high_resolution_clock::now();

            elapsed_time = end_time - begin_time;
            preprocess_time[i] = elapsed_time.count();

            preprocess_mean_time += preprocess_time[i];

            // Time search
            begin_time = std::chrono::high_resolution_clock::now();
            SA.count(s);
            end_time = std::chrono::high_resolution_clock::now();

            elapsed_time = end_time - begin_time;
            search_time[i] = elapsed_time.count();

            search_mean_time += search_time[i];
        }

        // Compute statistics
        preprocess_mean_time /= RUNS;
        search_mean_time /= RUNS;

        for (i = 0; i < RUNS; i++) {
            preprocess_time_stdev += (preprocess_time[i] - preprocess_mean_time) * (preprocess_time[i] - preprocess_mean_time);
            search_time_stdev += (search_time[i] - search_mean_time) * (search_time[i] - search_mean_time);
        }

        preprocess_time_stdev /= RUNS - 1; // Subtract 1 to get unbiased estimator
        preprocess_time_stdev = sqrt(preprocess_time_stdev);
        search_time_stdev /= RUNS - 1; // Subtract 1 to get unbiased estimator
        search_time_stdev = sqrt(search_time_stdev);

        preprocess << s.length() << "," << preprocess_mean_time << "," << preprocess_time_stdev << std::endl;
        search << s.length() << "," << search_mean_time << "," << search_time_stdev << std::endl;
    }

    search.close();
    preprocess.close();

    return 0;
}
