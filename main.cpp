// Sugiero patrón "AWHCRSG".

#include "suffix_array.hpp"
#include "FM_index.hpp"

using namespace sdsl;
using namespace std;


int main(int argc, char** argv) {
    // Verificando argumentos.
    if (argc != 4) {
            cout << "Uso: " << argv[0] << " <dataset de 0 a 2> <cantidad de archivos> <patrón>\n";
            return 1;
    }
    int dataset = stoi(argv[1]);
    int cantArchivos = stoi(argv[2]);
    string patron = argv[3];
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

    // Buscando ocurrencias de un patron.
    suffix_array SA(docs);
    FM_index fmi(docs);
    vector<uint64_t> ocurrencias1 = SA.locate(patron);
    vector<uint64_t> ocurrencias2 = fmi.locate(patron);

    cout << "Patrón: " << patron << endl;

    cout << "Ocurrencias segun suffix_Array: ";
    for (uint64_t i = 0 ; i < ocurrencias1.size() ; i++) {
        cout << ocurrencias1[i]+1 << " ";
    }
    cout << endl;

    cout << "Ocurrencias según FM_index    : ";
    for (uint64_t i = 0 ; i < ocurrencias2.size() ; i++) {
        cout << ocurrencias2[i]+1 << " ";
    }
    cout << endl;

  return 0;
}
