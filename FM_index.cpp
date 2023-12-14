#include <algorithm>
#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <string>
#include <unordered_set>
#include <vector>
#include <sdsl/bit_vectors.hpp>
#include <sdsl/suffix_arrays.hpp>
#include <sdsl/wavelet_trees.hpp>

#include "FM_index.hpp"

<<<<<<< HEAD

=======
>>>>>>> e86108485d1651eb3bfb91d7ac781465c4be8254
FM_index::FM_index(const std::vector<std::string> &docs)
{
    // Create string of concatenated docs separated by ETX
    uint64_t N, D;
    char ETX = 3;
    D = docs.size();
    doc_start.resize(D);
    std::string text;
    for (uint64_t i = 0; i < D; i++) {
        doc_start[i] = text.length(); // Store first index of doc i in t
        text += docs[i] + ETX;
    }

    std::ofstream t("concatenated.txt");
    // Check if t opened correctly
    if (t.is_open()) {
        t << text;
        t.close();
    } else {
        std::cerr << "Could not open file" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    sdsl::construct(fm_index, "concatenated.txt", 1);
    std::remove("concatenated.txt");
<<<<<<< HEAD
}


std::vector<uint64_t> FM_index::locate(const std::string &s)
{
    uint64_t i, j;
    std::unordered_set<uint64_t> doc_matches;
    std::vector<uint64_t> matches;
    auto occs = sdsl::locate(fm_index, s.begin(), s.end());

    // Mark documents which match s
    for (i = 0; i < occs.size(); i++) {
        auto j = std::upper_bound(doc_start.begin(), doc_start.end(), occs[i]) - doc_start.begin();
        doc_matches.insert(j - 1); // Minus one to get doc index
    }

    for (uint64_t d : doc_matches)
        matches.push_back(d);

    return matches;
}


double FM_index::size(void) {
    return fm_index.size();
}

int FM_index::size_in_mb(void) {
    return size_in_mega_bytes(fm_index) + (doc_start.size()*sizeof(uint64_t) / 1048576);
}
=======
}
std::vector<uint64_t> FM_index::locate(const std::string &s)
{
    uint64_t i, j;
    std::unordered_set<uint64_t> doc_matches;
    std::vector<uint64_t> matches;
    auto occs = sdsl::locate(fm_index, s.begin(), s.end());

    // Mark documents which match s
    for (i = 0; i < occs.size(); i++) {
        auto j = std::upper_bound(doc_start.begin(), doc_start.end(), occs[i]) - doc_start.begin();
        doc_matches.insert(j - 1); // Minus one to get doc index
    }

    for (uint64_t d : doc_matches)
        matches.push_back(d);

    return matches;
}

double FM_index::size(void)
{
    return sdsl::size_in_mega_bytes(fm_index);
}
>>>>>>> e86108485d1651eb3bfb91d7ac781465c4be8254
