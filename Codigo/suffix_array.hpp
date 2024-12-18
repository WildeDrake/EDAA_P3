#ifndef SUFFIX_ARRAY
#define SUFFIX_ARRAY

#include <cstdint>
#include <string>
#include <vector>
#include <sdsl/suffix_arrays.hpp>

class suffix_array
{
    private:
        std::string text;
        sdsl::int_vector<> SA;
        std::vector<uint64_t> doc_start; // First index of each doc

    public:
        suffix_array(const std::vector<std::string> &docs);

        std::vector<uint64_t> locate(const std::string &s);

	double size(void);
};

#endif
