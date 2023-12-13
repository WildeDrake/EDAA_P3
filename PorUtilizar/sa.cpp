// Construcción del suffix array y la BWT de un texto
//
// Prerrequisitos: Tener la biblioteca SDSL instalada
//
// Compilación: g++ -O3 -o sa sa.cpp -lsdsl -ldivsufsort -ldivsufsort64

#include <sdsl/suffix_arrays.hpp>
#include <sdsl/int_vector.hpp>
#include <string>
#include <iostream>
#include <algorithm>

using namespace sdsl;
using namespace std;

int main(int argc, char** argv) {
  // Verificando argumentos.
  if (argc != 3) {
        cout << "Uso: " << argv[0] << " <dataset de 0 a 2> <cantidad de archivos>\n";
        return 1;
  }
  int dataset = stoi(argv[1]);
  int cantArchivos = stoi(argv[2]);

  // Seleccionando dataset.
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

  // Leyendo archivos. (creo que se puede optimizar).
  int_vector<> seq;
  for (int i = 1 ; i < cantArchivos + 1 ; i++) {
        stringstream ss;
        ss << directorio << i << ".xml";
        string nombreArchivo = ss.str();
        ifstream archivo(nombreArchivo);
        if (archivo.is_open()) {
            stringstream contenidoArchivo;
            contenidoArchivo << archivo.rdbuf();
            string contenido = contenidoArchivo.str();
            seq.resize(seq.size() + contenido.size() + 1);
            copy(contenido.begin(), contenido.end(), seq.begin() + seq.size() - contenido.size() - 1);
            seq[seq.size() - 1] = 0;

            archivo.close();
        } else {
            cout << "No se pudo abrir el archivo: " << nombreArchivo << endl;
            return 1;
        }
  }
  int32_t n = seq.size();
  
  // Construyendo el Suffix Array 
  int_vector<> sa(1, 0, bits::hi(n)+1);
  sa.resize(n);
  algorithm::calculate_sa((const unsigned char*)seq.data(), n, sa);

  cout << "Tamaño del SA " << size_in_mega_bytes(sa) << " MB." << endl;
  
  return 0;
}