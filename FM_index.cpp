#include "FM_index.hpp"


FM_index::FM_index(const std::vector<std::string> &docs) {
    // Create string of concatenated docs separated by ETX
    uint64_t N, D;
    char ETX = 3;
    D = docs.size();
    doc_start.resize(D);
    std::string text;
    for (uint64_t i = 0; i < D; i++) {
        doc_start[i] = text.length(); // Store first index of doc i in t
        text += docs[i];
        text += ETX;
    }
    std::ofstream archivo("temp.txt");
    // Verificar si el archivo se abrió correctamente
    if (archivo.is_open()) {
        archivo << text << std::endl;
        archivo.close();
    } else {
        std::cerr << "Error al abrir el archivo: " << "temp.txt" << std::endl;
    }
    sdsl::construct(fm_index, "temp.txt", 1);
    std::remove("temp.txt");
}

int FM_index::size() {
    return sdsl::size_in_mega_bytes(fm_index);
}