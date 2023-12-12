// Construcción del FM index de un texto
//
// Prerrequisitos: Tener la biblioteca SDSL instalada
//
// Compilación: g++ -O3 -o fmi FM-index.cpp -lsdsl -ldivsufsort -ldivsufsort64

#include <sdsl/suffix_arrays.hpp>
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
    directorio = "/dblp/dblp5MB_";
    break;
  case 1:
    directorio = "/proteins/proteins5MB_";
    break;
  case 2:
    directorio = "/sources/sources5MB_";
    break;
  default:
    cout << "Dataset no válido." << endl;
    return 1;
  }
  
  // Leyendo archivos. (creo que se puede optimizar)
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
            for (char c : contenido) {
                seq.push_back(c);
            }
            seq.push_back(0);
            archivo.close();
        } else {
            cout << "No se pudo abrir el archivo: " << nombreArchivo << endl;
            return 1;
        }
  }
  seq.push_back(0);
  int32_t n = seq.size();
  
  // Construyendo el Suffix Array.
  int_vector<> sa(1, 0, bits::hi(n)+1);
  sa.resize(n);
  algorithm::calculate_sa((const unsigned char*)seq.data(), n, sa);

  // Construyendo el FM-index.
  csa_wt<wt_int<>> fm_index;
  construct(fm_index, sa);
  
  cout << "Tamaño del FM-index " << size_in_mega_bytes(fm_index) << " MB." << endl;





  // Cosas que dejo el Profe.
    string patron;
    cout << "Ingrese un patrón a buscar: ";
    cin >> patron;

    // Tamaño del patrón
    size_t m  = patron.size();

    // Buscando las ocurrencias del patrón
    size_t occs = sdsl::count(fm_index, patron.begin(), patron.end());

    cout << "# de ocurrencias: " << occs << endl;
    if (occs > 0) {
      cout << "Las ocurrencias comienzan en las siguientes posiciones: " << endl;
      //      auto locations = locate(fm_index, query.begin(), query.begin()+m)
      auto posiciones = sdsl::locate(fm_index, patron.begin(), patron.end());
      sort(posiciones.begin(), posiciones.end());
      
      for (size_t i = 0; i < occs; ++i) {
	cout << posiciones[i] << endl;
      }
    }
    return 0;
}
