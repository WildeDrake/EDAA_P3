/** Testing pattern matching over fixed text
 * Author: LELE
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
#include <random>
#include <sstream>
#include <string>
#include <vector>

#include "kmp.cpp"
#include "suffix_array.cpp"

// Change these depending on application
#define RUNS 64
#define LOWER 1
#define UPPER 1024
#define STEP 2

#define TEXT "" // Location of text

int main(void)
{
    // Set up clock variables
    int64_t i, j;
    double time[RUNS];
    double mean_time, time_stdev, dev;
    auto begin_time = std::chrono::high_resolution_clock::now();
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed_time = end_time - begin_time;

    // Load text
    std::string_view s, t;

    std::ifstream text_file(TEXT);
    std::stringstream text_buffer;
    text_buffer << text_file.rdbuf();
    std::string text = text_buffer.str();
    t = text;

    // Load patterns
    // These are picked at random from the text
    std::vector<std::string> pattern;
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int64_t> distr(0, text.length() - UPPER);
    for (i = LOWER; i <= UPPER; i *= STEP)
        pattern.push_back(text.substr(distr(rng), i));

    // File to write KMP preprocess time data
    std::ofstream time_data;
    time_data.open("experimental_data/pattern/kmp_preprocess_ms.csv");
    time_data << "m,t_mean,t_stdev" << std::endl;

    // Begin testing of KMP preprocess
    for (j = 0; j < pattern.size(); j++) {
        mean_time = 0;
        time_stdev = 0;

        s = pattern[j];

        // Run to compute elapsed time
        for (i = 0; i < RUNS; i++) {
            begin_time = std::chrono::high_resolution_clock::now();
            // Function to test goes here
            kmp_table(s);
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

        time_data << s.length() << "," << mean_time << "," << time_stdev << std::endl;
    }

    time_data.close();

    // File to write KMP search time data
    time_data.open("experimental_data/pattern/kmp_search_ms.csv");
    time_data << "m,t_mean,t_stdev" << std::endl;

    // Begin testing of KMP search
    std::vector<int64_t> table;
    for (j = 0; j < pattern.size(); j++) {
        mean_time = 0;
        time_stdev = 0;

        s = pattern[j];
        table = kmp_table(s);

        // Run to compute elapsed time
        for (i = 0; i < RUNS; i++) {
            begin_time = std::chrono::high_resolution_clock::now();
            // Function to test goes here
            kmp_match(s, t, table);
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

        time_data << s.length() << "," << mean_time << "," << time_stdev << std::endl;
    }

    time_data.close();

    // We do not test Suffix Array preprocess since this only depends on text length

    // File to write Suffix Array search time data
    time_data.open("experimental_data/pattern/suffix_array_search_ms.csv");
    time_data << "m,t_mean,t_stdev" << std::endl;

    suffix_array SA(text);
    // Begin testing of Suffix Array search
    for (j = 0; j < pattern.size(); j++) {
        mean_time = 0;
        time_stdev = 0;

        s = pattern[j];

        // Run to compute elapsed time
        for (i = 0; i < RUNS; i++) {
            begin_time = std::chrono::high_resolution_clock::now();
            // Function to test goes here
            SA.count(s);
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

        time_data << s.length() << "," << mean_time << "," << time_stdev << std::endl;
    }

    time_data.close();

    return 0;
}