#ifndef FM_INDEX
#define FM_INDEX

#include <cstdint>
#include <string>
#include <vector>
#include <sdsl/suffix_arrays.hpp>

class FM_index {
    private:
        csa_wt<wt_int<>> fm_index;
        std::vector<uint64_t> doc_start; // First index of each doc

    public:
        FM_index(const std::vector<std::string> &docs);
        std::vector<uint64_t> locate(const std::string &s);
};

#endif