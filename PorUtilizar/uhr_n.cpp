/** Testing pattern matching over variable text
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
#include <string>
#include <vector>

#include "kmp.cpp"
#include "suffix_array.cpp"

// Change these depending on application
#define RUNS 64
#define LOWER 256
#define UPPER 1048576
#define STEP 2

#define PATTERN 128 // Length of pattern

int main(void)
{
    // Set up clock variables
    int64_t i, j;
    double time[RUNS];
    double mean_time, time_stdev, dev;
    auto begin_time = std::chrono::high_resolution_clock::now();
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed_time = end_time - begin_time;

    // Create texts
    // These are created randomly over printable ASCII characters
    std::string_view s, t;
    std::string _t;
    std::vector<std::string> text;
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int8_t> distr(32, 126);
    for (i = LOWER; i <= UPPER; i *= STEP) {
        _t = "";
        for (j = 0; j < i; j++)
            _t += distr(rng);

        text.push_back(_t);
    }

    // Create random pattern
    std::string pattern = "";
    for (i = 0; i < PATTERN; i++)
        pattern += distr(rng);

    s = pattern;

    // We do not test KMP preprocess since it only depends on pattern

    // File to write KMP search time data
    std::ofstream time_data;
    time_data.open("experimental_data/text/kmp_search_ms.csv");
    time_data << "n,t_mean,t_stdev" << std::endl;

    // Begin testing of KMP search 
    std::vector<int64_t> table = kmp_table(s);
    for (j = 0; j < text.size(); j++) {
        mean_time = 0;
        time_stdev = 0;

        t = text[j];

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

        time_data << t.length() << "," << mean_time << "," << time_stdev << std::endl;
    }

    time_data.close();

    // File to write SA preprocess time data
    time_data.open("experimental_data/text/suffix_array_preprocess_ms.csv");
    time_data << "n,t_mean,t_stdev" << std::endl;

    // Begin testing of Suffix Array construction
    for (j = 0; j < text.size(); j++) {
        mean_time = 0;
        time_stdev = 0;

        // Run to compute elapsed time
        for (i = 0; i < RUNS; i++) {
            begin_time = std::chrono::high_resolution_clock::now();
            // Function to test goes here
            suffix_array SA(text[j]);
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

        time_data << text[j].length() << "," << mean_time << "," << time_stdev << std::endl;
    }

    time_data.close();

    // File to write SA search time data
    time_data.open("experimental_data/text/suffix_array_search_ms.csv");
    time_data << "n,t_mean,t_stdev" << std::endl;

    // Begin testing of Suffix Array search
    for (j = 0; j < text.size(); j++) {
        mean_time = 0;
        time_stdev = 0;

        suffix_array SA(text[j]);

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

        time_data << text[j].length() << "," << mean_time << "," << time_stdev << std::endl;
    }

    time_data.close();

    return 0;
}
