#include "FM_index.hpp"


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
    FM_index fmi(docs);
    cout << " 2 " << endl;
    //vector<uint64_t> ocurrencias = fmi.locate("http://dx.doi.org/10.1006/jath.2002.3673");

    //cout << "Ocurrencias: " << ocurrencias.size() << endl;
    cout << "espacio: " << fmi.size() << endl;
  return 0;
}