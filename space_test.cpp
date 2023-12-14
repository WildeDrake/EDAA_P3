// Para compilar:
// g++ -O3 -o a space_test.cpp suffix_array.cpp FM_index.cpp -lsdsl -ldivsufsort -ldivsufsort64 -std=c++20
// Para ejecutar:
// for n in {1..10..1}; do ./a 0 $n >> space_test_dblp.csv
// for n in {1..10..1}; do ./a 1 $n >> space_test_proteins.csv
// for n in {1..10..1}; do ./a 2 $n >> space_test_sources.csv

#include "suffix_array.hpp"
#include "FM_index.hpp"

using namespace std;

int main(int argc, char** argv) {
    if (argc != 3) {
            cout << "Uso: " << argv[0] << " <dataset de 0 a 2> <cantidad de archivos>\n";
            return 1;
    }
    int dataset = stoi(argv[1]);
    int cantArchivos = stoi(argv[2]);

    string directorio;
    switch (dataset){
    case 0:
        directorio = "datasets/dblp/dblp5MB_";
        break;
    case 1:
        directorio = "datasets/proteins/proteins5MB_";
        break;
    case 2:
        directorio = "datasets/sources/sources5MB_";
        break;
    default:
        cout << "Dataset no válido." << endl;
        return 1;
    }
    vector<string> docs;
    for (int i = 1 ; i < cantArchivos + 1 ; i++) {
            stringstream ss;
            ss << directorio << i << ".xml";
            string nombreArchivo = ss.str();
            ifstream archivo(nombreArchivo);
            if (archivo.is_open()) {
                stringstream contenidoArchivo;
                contenidoArchivo << archivo.rdbuf();
                docs.push_back(contenidoArchivo.str());
                archivo.close();
            } else {
                cout << "No se pudo abrir el archivo: " << nombreArchivo << endl;
                return 1;
            }
    }
    suffix_array SA(docs);
    FM_index fmi(docs);
    cout << cantArchivos << ";" << fmi.size_in_mb() << ";" << SA.size_in_mb() << endl;
    return 0;
}
