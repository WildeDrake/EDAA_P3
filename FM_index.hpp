#ifndef FM_INDEX
#define FM_INDEX

#include <iostream>
#include <fstream>
#include <cstdio>
#include <algorithm>
#include <cstdint>
#include <string>
#include <vector>
#include <unordered_set>
#include <sdsl/suffix_arrays.hpp>
#include <sdsl/wavelet_trees.hpp>


class FM_index {
    private:
        sdsl::csa_wt<sdsl::wt_int<sdsl::rrr_vector<> > > fm_index;
        std::vector<uint64_t> doc_start; // First index of each doc

    public:
        FM_index(const std::vector<std::string> &docs);
        std::vector<uint64_t> locate(const std::string &s);
        double size(void);
        int size_in_mb();
};

#endif