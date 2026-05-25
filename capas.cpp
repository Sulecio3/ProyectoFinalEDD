#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
using namespace std;

struct Pixel {
    int x;
    int y;
    string color;
    Pixel* siguiente;
};

struct Capa {
    int id;
    Pixel* primero;
    Capa* izquierda;
    Capa* derecha;
};

Capa* raizCapas = NULL;

string limpiarTexto(string texto) {
    string nuevo = "";
    for (int i = 0; i < texto.length(); i++) {
        if (texto[i] != ' ' && texto[i] != '\t' && texto[i] != '\r' && texto[i] != '\n') {
            nuevo += texto[i];
        }
    }
    return nuevo;
}

Capa* crearCapa(int id) {
    Capa* nueva = new Capa();
    nueva->id = id;
    nueva->primero = NULL;
    nueva->izquierda = NULL;
    nueva->derecha = NULL;
    return nueva;
}

Pixel* crearPixel(int x, int y, string color) {
    Pixel* nuevo = new Pixel();
    nuevo->x = x;
    nuevo->y = y;
    nuevo->color = color;
    nuevo->siguiente = NULL;
    return nuevo;
}

void agregarPixel(Capa* capa, int x, int y, string color) {
    if (capa == NULL) {
        return;
    }

    Pixel* nuevo = crearPixel(x, y, color);

    if (capa->primero == NULL) {
        capa->primero = nuevo;
    } else {
        Pixel* aux = capa->primero;
        while (aux->siguiente != NULL) {
            aux = aux->siguiente;
        }
        aux->siguiente = nuevo;
    }
}

Capa* insertarCapa(Capa* raiz, Capa* nueva) {
    if (raiz == NULL) {
        return nueva;
    }

    if (nueva->id < raiz->id) {
        raiz->izquierda = insertarCapa(raiz->izquierda, nueva);
    } else if (nueva->id > raiz->id) {
        raiz->derecha = insertarCapa(raiz->derecha, nueva);
    } else {
        cout << "La capa " << nueva->id << " ya existe y no se agrego de nuevo.\n";
    }

    return raiz;
}

Capa* buscarCapa(Capa* raiz, int id) {
    if (raiz == NULL) {
        return NULL;
    }

    if (id == raiz->id) {
        return raiz;
    }

    if (id < raiz->id) {
        return buscarCapa(raiz->izquierda, id);
    } else {
        return buscarCapa(raiz->derecha, id);
    }
}

void mostrarPixeles(Capa* capa) {
    if (capa == NULL) {
        cout << "No existe la capa.\n";
        return;
    }

    cout << "\nCapa: " << capa->id << "\n";
    cout << "Pixeles guardados:\n";

    Pixel* aux = capa->primero;

    if (aux == NULL) {
        cout << "La capa no tiene pixeles.\n";
    }

    while (aux != NULL) {
        cout << "Fila: " << aux->y << "  Columna: " << aux->x << "  Color: " << aux->color << "\n";
        aux = aux->siguiente;
    }
}

void inorden(Capa* raiz) {
    if (raiz != NULL) {
        inorden(raiz->izquierda);
        cout << "Capa " << raiz->id << "\n";
        inorden(raiz->derecha);
    }
}

int contarPixeles(Capa* capa) {
    int contador = 0;
    Pixel* aux = capa->primero;

    while (aux != NULL) {
        contador++;
        aux = aux->siguiente;
    }

    return contador;
}

void resumenInorden(Capa* raiz) {
    if (raiz != NULL) {
        resumenInorden(raiz->izquierda);
        cout << "Capa " << raiz->id << " - Pixeles: " << contarPixeles(raiz) << "\n";
        resumenInorden(raiz->derecha);
    }
}

void procesarPixel(string linea, Capa* capaActual) {
    int pos1 = linea.find(',');
    int pos2 = linea.find(',', pos1 + 1);
    int pos3 = linea.find(';');

    if (pos1 == -1 || pos2 == -1 || pos3 == -1) {
        return;
    }

    string sx = linea.substr(0, pos1);
    string sy = linea.substr(pos1 + 1, pos2 - pos1 - 1);
    string color = linea.substr(pos2 + 1, pos3 - pos2 - 1);

    int x = atoi(sx.c_str());
    int y = atoi(sy.c_str());

    agregarPixel(capaActual, x, y, color);
}

void cargarCapas() {
    string ruta;
    cout << "\nIngrese el nombre o ruta del archivo .cap: ";
    getline(cin, ruta);

    ifstream archivo(ruta.c_str());

    if (!archivo.is_open()) {
        cout << "\nNo se pudo abrir el archivo.\n";
        return;
    }

    string linea;
    Capa* capaActual = NULL;
    int idAutomatico = 1;
    int capasLeidas = 0;

    while (getline(archivo, linea)) {
        linea = limpiarTexto(linea);

        if (linea.length() == 0) {
            continue;
        }

        int posicionLlave = linea.find('{');

        if (posicionLlave != -1) {
            string textoId = linea.substr(0, posicionLlave);
            int id = 0;

            if (textoId.length() == 0) {
                id = idAutomatico;
            } else {
                id = atoi(textoId.c_str());
            }

            capaActual = crearCapa(id);
            raizCapas = insertarCapa(raizCapas, capaActual);
            capasLeidas++;
            idAutomatico = id + 1;
        } else if (linea == "}") {
            capaActual = NULL;
        } else {
            if (capaActual != NULL) {
                procesarPixel(linea, capaActual);
            }
        }
    }

    archivo.close();

    cout << "\nCarga de capas finalizada.\n";
    cout << "Capas leidas del archivo: " << capasLeidas << "\n";
}

void mostrarCapas() {
    cout << "\nCapas cargadas en el ABB:\n\n";

    if (raizCapas == NULL) {
        cout << "No hay capas cargadas.\n";
    } else {
        resumenInorden(raizCapas);
    }
}

void buscarCapaMenu() {
    int id;
    cout << "\nIngrese el id de la capa: ";
    cin >> id;

    Capa* encontrada = buscarCapa(raizCapas, id);

    if (encontrada == NULL) {
        cout << "\nNo se encontro la capa.\n";
    } else {
        mostrarPixeles(encontrada);
    }
}
