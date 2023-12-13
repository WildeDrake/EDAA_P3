#include "FM_index.hpp"

#include <iostream>
#include <fstream>

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
        // Escribir el string en el archivo
        archivo << text << std::endl;

        // Cerrar el archivo
        archivo.close();

        std::cout << "El text se ha guardado correctamente en " << "temp.txt" << std::endl;
    } else {
        std::cerr << "Error al abrir el archivo: " << "temp.txt" << std::endl;
    }
    
/*
    // Compute Suffix Array
    N = text.length();
    sdsl::int_vector<> SA;
    SA.width(sdsl::bits::hi(N) + 1);
    SA.resize(N);
    sdsl::algorithm::calculate_sa((unsigned char *)text.c_str(), N, SA);

    std::cout << "SA size: " << SA.size() << std::endl;
    // Compute FM Index from Suffix Array
*/

    sdsl::construct_im(fm_index, "temp.txt", 1);
}



    int FM_index::size() {
    return sdsl::size_in_mega_bytes(fm_index);
}