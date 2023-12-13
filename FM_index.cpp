#include <algorithm>
#include <cstdint>
#include <string>
#include <vector>
#include <sdsl/suffix_arrays.hpp>

#include "suffix_array.hpp"

FM_index::FM_index(const std::vector<std::string> &docs) {
    // Create string of concatenated docs separated by ETX
    uint64_t D = docs.size();
    char ETX = 3;
    doc_start.resize(D);
    text = "";
    for (uint64_t i = 0; i < D; i++) {
        doc_start[i] = text.length(); // Almacenar el primer índice del documento i en t
        text += docs[i] + ETX;
    }
    // Compute FM_index
    construct(fm_index, text);
}

std::vector<uint64_t> suffix_array::locate(const std::string &s){
    uint64_t hi, lo;
    lo = 0;
    hi = N;
    // FM index retorna dos strings? Uno para bwt el otro de SA
    std::vector<int> L(n);
    for (int i = 0; i < n; ++i) {
        firstColumn[i] = i;
    }


}
