#include "suffix_array.hpp"


suffix_array::suffix_array(const std::vector<std::string> &docs)
{
    // Create string of concatenated docs separated by ETX
    uint64_t N, D;
    char ETX = 3;
    D = docs.size();
    doc_start.resize(D);
    text = "";
    for (uint64_t i = 0; i < D; i++) {
        doc_start[i] = text.length(); // Store first index of doc i in t
        text += docs[i] + ETX;
    }

    // Compute Suffix Array
    N = text.length();
    SA.width(sdsl::bits::hi(N) + 1);
    SA.resize(N);
    sdsl::algorithm::calculate_sa((unsigned char *) text.c_str(), N, SA);
}


std::vector<uint64_t> suffix_array::locate(const std::string &s)
{
    uint64_t N, lo, mi, hi, i;
    std::unordered_set<uint64_t> doc_matches;
    std::vector<uint64_t> matches;
    N = text.length();

    // Find lower bound
    lo = 0;
    hi = N;
    while (lo < hi) {
        mi = lo + (hi - lo) / 2;
        if (text.substr(SA[mi]) < s)
            lo = mi + 1;
        else
            hi = mi;
    }
    i = lo;

    // Find upper bound
    // Do not set lo = 0 since it is already at lower bound
    hi = N;
    while (lo < hi) {
        mi = lo + (hi - lo) / 2;
        if (text.substr(SA[mi]).starts_with(s))
            lo = mi + 1;
        else
            hi = mi;
    }

    // Mark documents which match s
    for ( ; i < hi; i++) {
        auto j = std::upper_bound(doc_start.begin(), doc_start.end(), SA[i]) - doc_start.begin();
        doc_matches.insert(j - 1); // Minus one to get doc index
    }

    for (uint64_t d : doc_matches)
        matches.push_back(d);

    return matches;
}
