#ifndef CAPAS_CPP
#define CAPAS_CPP

#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <cstdlib>

using namespace std;

struct NodoPixel {
    int x;
    int y;
    string color;
    NodoPixel* derecha;
    NodoPixel* abajo;

    NodoPixel(int px, int py, string pcolor) {
        x = px;
        y = py;
        color = pcolor;
        derecha = NULL;
        abajo = NULL;
    }
};

struct NodoEncabezado {
    int id;
    NodoPixel* acceso;
    NodoEncabezado* siguiente;

    NodoEncabezado(int pid) {
        id = pid;
        acceso = NULL;
        siguiente = NULL;
    }
};

class ListaEncabezados {
private:
    NodoEncabezado* primero;

public:
    ListaEncabezados() {
        primero = NULL;
    }

    NodoEncabezado* obtenerPrimero() {
        return primero;
    }

    NodoEncabezado* buscar(int id) {
        NodoEncabezado* actual = primero;

        while (actual != NULL) {
            if (actual->id == id) {
                return actual;
            }

            actual = actual->siguiente;
        }

        return NULL;
    }

    NodoEncabezado* insertar(int id) {
        NodoEncabezado* encontrado = buscar(id);

        if (encontrado != NULL) {
            return encontrado;
        }

        NodoEncabezado* nuevo = new NodoEncabezado(id);

        if (primero == NULL) {
            primero = nuevo;
            return nuevo;
        }

        if (id < primero->id) {
            nuevo->siguiente = primero;
            primero = nuevo;
            return nuevo;
        }

        NodoEncabezado* actual = primero;

        while (actual->siguiente != NULL && actual->siguiente->id < id) {
            actual = actual->siguiente;
        }

        nuevo->siguiente = actual->siguiente;
        actual->siguiente = nuevo;

        return nuevo;
    }
};

class MatrizDispersa {
private:
    ListaEncabezados filas;
    ListaEncabezados columnas;
    int maxX;
    int maxY;

public:
    MatrizDispersa() {
        maxX = 0;
        maxY = 0;
    }

    int obtenerMaxX() {
        return maxX;
    }

    int obtenerMaxY() {
        return maxY;
    }

    void insertar(int x, int y, string color) {
        NodoPixel* nuevo = new NodoPixel(x, y, color);

        NodoEncabezado* fila = filas.insertar(y);
        NodoEncabezado* columna = columnas.insertar(x);

        if (fila->acceso == NULL) {
            fila->acceso = nuevo;
        } else if (x < fila->acceso->x) {
            nuevo->derecha = fila->acceso;
            fila->acceso = nuevo;
        } else {
            NodoPixel* actual = fila->acceso;

            while (actual->derecha != NULL && actual->derecha->x < x) {
                actual = actual->derecha;
            }

            nuevo->derecha = actual->derecha;
            actual->derecha = nuevo;
        }

        if (columna->acceso == NULL) {
            columna->acceso = nuevo;
        } else if (y < columna->acceso->y) {
            nuevo->abajo = columna->acceso;
            columna->acceso = nuevo;
        } else {
            NodoPixel* actual = columna->acceso;

            while (actual->abajo != NULL && actual->abajo->y < y) {
                actual = actual->abajo;
            }

            nuevo->abajo = actual->abajo;
            actual->abajo = nuevo;
        }

        if (x > maxX) {
            maxX = x;
        }

        if (y > maxY) {
            maxY = y;
        }
    }

    int contarPixeles() {
        int contador = 0;
        NodoEncabezado* fila = filas.obtenerPrimero();

        while (fila != NULL) {
            NodoPixel* pixel = fila->acceso;

            while (pixel != NULL) {
                contador++;
                pixel = pixel->derecha;
            }

            fila = fila->siguiente;
        }

        return contador;
    }

    void mostrarPixeles() {
        NodoEncabezado* fila = filas.obtenerPrimero();

        while (fila != NULL) {
            NodoPixel* pixel = fila->acceso;

            while (pixel != NULL) {
                cout << "(" << pixel->x << "," << pixel->y << ") " << pixel->color << endl;
                pixel = pixel->derecha;
            }

            fila = fila->siguiente;
        }
    }

    string obtenerColor(int x, int y) {
        NodoEncabezado* fila = filas.buscar(y);

        if (fila == NULL) {
            return "#FFFFFF";
        }

        NodoPixel* pixel = fila->acceso;

        while (pixel != NULL) {
            if (pixel->x == x) {
                return pixel->color;
            }

            pixel = pixel->derecha;
        }

        return "#FFFFFF";
    }

    string obtenerColorReal(int x, int y) {
        NodoEncabezado* fila = filas.buscar(y);

        if (fila == NULL) {
            return "";
        }

        NodoPixel* pixel = fila->acceso;

        while (pixel != NULL) {
            if (pixel->x == x) {
                return pixel->color;
            }

            pixel = pixel->derecha;
        }

        return "";
    }

    bool graficarEstructura(string nombreDot, string nombrePng, int idCapa) {
        ofstream archivo(nombreDot.c_str());
        char q = 34;

        if (!archivo.is_open()) {
            return false;
        }

        archivo << "digraph G {" << endl;
        archivo << "rankdir=LR;" << endl;
        archivo << "node [shape=box, style=filled, fontname=" << q << "Arial" << q << "];" << endl;
        archivo << "edge [color=" << q << "#444444" << q << "];" << endl;
        archivo << "titulo [label=" << q << "Matriz dispersa de capa " << idCapa << q << ", fillcolor=" << q << "#FDE1E8" << q << ", color=" << q << "#C2185B" << q << "];" << endl;
        archivo << "matriz [label=" << q << "matriz" << q << ", fillcolor=" << q << "#FDE1E8" << q << ", color=" << q << "#C2185B" << q << "];" << endl;
        archivo << "titulo -> matriz [style=invis];" << endl;

        NodoEncabezado* columna = columnas.obtenerPrimero();

        while (columna != NULL) {
            archivo << "col" << columna->id << " [label=" << q << "Columna " << columna->id << q << ", fillcolor=" << q << "#DDF7F8" << q << ", color=" << q << "#008B95" << q << "];" << endl;
            columna = columna->siguiente;
        }

        NodoEncabezado* fila = filas.obtenerPrimero();

        while (fila != NULL) {
            archivo << "fila" << fila->id << " [label=" << q << "Fila " << fila->id << q << ", fillcolor=" << q << "#FFF3E0" << q << ", color=" << q << "#EF6C00" << q << "];" << endl;
            fila = fila->siguiente;
        }

        fila = filas.obtenerPrimero();

        while (fila != NULL) {
            NodoPixel* pixel = fila->acceso;

            while (pixel != NULL) {
                archivo << "p" << pixel->x << "_" << pixel->y << " [label=" << q << "(" << pixel->x << "," << pixel->y << ")\\n" << pixel->color << q << ", fillcolor=" << q << pixel->color << q << ", color=" << q << "#222222" << q << "];" << endl;
                pixel = pixel->derecha;
            }

            fila = fila->siguiente;
        }

        columna = columnas.obtenerPrimero();

        if (columna != NULL) {
            archivo << "matriz -> col" << columna->id << ";" << endl;

            while (columna->siguiente != NULL) {
                archivo << "col" << columna->id << " -> col" << columna->siguiente->id << ";" << endl;
                columna = columna->siguiente;
            }
        }

        fila = filas.obtenerPrimero();

        if (fila != NULL) {
            archivo << "matriz -> fila" << fila->id << ";" << endl;

            while (fila->siguiente != NULL) {
                archivo << "fila" << fila->id << " -> fila" << fila->siguiente->id << ";" << endl;
                fila = fila->siguiente;
            }
        }

        fila = filas.obtenerPrimero();

        while (fila != NULL) {
            NodoPixel* pixel = fila->acceso;

            if (pixel != NULL) {
                archivo << "fila" << fila->id << " -> p" << pixel->x << "_" << pixel->y << ";" << endl;

                while (pixel->derecha != NULL) {
                    archivo << "p" << pixel->x << "_" << pixel->y << " -> p" << pixel->derecha->x << "_" << pixel->derecha->y << ";" << endl;
                    pixel = pixel->derecha;
                }
            }

            fila = fila->siguiente;
        }

        columna = columnas.obtenerPrimero();

        while (columna != NULL) {
            NodoPixel* pixel = columna->acceso;

            if (pixel != NULL) {
                archivo << "col" << columna->id << " -> p" << pixel->x << "_" << pixel->y << " [color=" << q << "#008B95" << q << "];" << endl;

                while (pixel->abajo != NULL) {
                    archivo << "p" << pixel->x << "_" << pixel->y << " -> p" << pixel->abajo->x << "_" << pixel->abajo->y << " [color=" << q << "#008B95" << q << "];" << endl;
                    pixel = pixel->abajo;
                }
            }

            columna = columna->siguiente;
        }

        archivo << "}" << endl;
        archivo.close();

        string comando = "dot -Tpng \"" + nombreDot + "\" -o \"" + nombrePng + "\"";
        int resultado = system(comando.c_str());

        if (resultado != 0) {
            cout << "Se creo el archivo .dot, pero Graphviz no genero el .png." << endl;
            cout << "Revise que Graphviz este instalado y agregado al PATH." << endl;
        }

        return true;
    }
};

struct NodoCapa {
    int id;
    MatrizDispersa matriz;
    NodoCapa* izquierda;
    NodoCapa* derecha;

    NodoCapa(int pid) {
        id = pid;
        izquierda = NULL;
        derecha = NULL;
    }
};

class ArbolCapas {
private:
    NodoCapa* raiz;

    NodoCapa* insertarRecursivo(NodoCapa* actual, int id) {
        if (actual == NULL) {
            actual = new NodoCapa(id);
            return actual;
        }

        if (id < actual->id) {
            actual->izquierda = insertarRecursivo(actual->izquierda, id);
        } else if (id > actual->id) {
            actual->derecha = insertarRecursivo(actual->derecha, id);
        }

        return actual;
    }

    NodoCapa* buscarRecursivo(NodoCapa* actual, int id) {
        if (actual == NULL) {
            return NULL;
        }

        if (actual->id == id) {
            return actual;
        }

        if (id < actual->id) {
            return buscarRecursivo(actual->izquierda, id);
        } else {
            return buscarRecursivo(actual->derecha, id);
        }
    }

    void mostrarInordenRecursivo(NodoCapa* actual) {
        if (actual != NULL) {
            mostrarInordenRecursivo(actual->izquierda);
            cout << "Capa " << actual->id << " - Pixeles: " << actual->matriz.contarPixeles() << endl;
            mostrarInordenRecursivo(actual->derecha);
        }
    }

    void saltarEspacios(string texto, int& i) {
        while (i < texto.length() && isspace(texto[i])) {
            i++;
        }
    }

    int leerNumero(string texto, int& i) {
        int numero = 0;

        while (i < texto.length() && isdigit(texto[i])) {
            numero = numero * 10 + (texto[i] - '0');
            i++;
        }

        return numero;
    }

    string leerColor(string texto, int& i) {
        string color = "";

        while (i < texto.length() && texto[i] != ';' && texto[i] != '\n' && texto[i] != '\r') {
            if (!isspace(texto[i]) && texto[i] != ',') {
                color += texto[i];
            }

            i++;
        }

        return color;
    }

    void escribirNodosArbol(NodoCapa* actual, ofstream& archivo) {
        if (actual == NULL) {
            return;
        }

        archivo << "nodo" << actual->id << " [label=\"Capa " << actual->id << "\\nPixeles: " << actual->matriz.contarPixeles() << "\"];" << endl;

        if (actual->izquierda != NULL) {
            archivo << "nodo" << actual->id << " -> nodo" << actual->izquierda->id << ";" << endl;
        }

        if (actual->derecha != NULL) {
            archivo << "nodo" << actual->id << " -> nodo" << actual->derecha->id << ";" << endl;
        }

        escribirNodosArbol(actual->izquierda, archivo);
        escribirNodosArbol(actual->derecha, archivo);
    }


    void recorrerPreordenLimitado(NodoCapa* actual, NodoCapa** lista, int& contador, int limite) {
        if (actual == NULL) {
            return;
        }

        if (contador >= limite) {
            return;
        }

        lista[contador] = actual;
        contador++;

        recorrerPreordenLimitado(actual->izquierda, lista, contador, limite);
        recorrerPreordenLimitado(actual->derecha, lista, contador, limite);
    }

    void recorrerInordenLimitado(NodoCapa* actual, NodoCapa** lista, int& contador, int limite) {
        if (actual == NULL) {
            return;
        }

        if (contador >= limite) {
            return;
        }

        recorrerInordenLimitado(actual->izquierda, lista, contador, limite);

        if (contador < limite) {
            lista[contador] = actual;
            contador++;
        }

        recorrerInordenLimitado(actual->derecha, lista, contador, limite);
    }

    void recorrerPostordenLimitado(NodoCapa* actual, NodoCapa** lista, int& contador, int limite) {
        if (actual == NULL) {
            return;
        }

        if (contador >= limite) {
            return;
        }

        recorrerPostordenLimitado(actual->izquierda, lista, contador, limite);
        recorrerPostordenLimitado(actual->derecha, lista, contador, limite);

        if (contador < limite) {
            lista[contador] = actual;
            contador++;
        }
    }

    string obtenerColorFinalRecorrido(NodoCapa** lista, int total, int x, int y) {
        string colorFinal = "#FFFFFF";

        for (int i = 0; i < total; i++) {
            if (lista[i] != NULL) {
                string colorCapa = lista[i]->matriz.obtenerColorReal(x, y);

                if (colorCapa != "") {
                    colorFinal = colorCapa;
                }
            }
        }

        return colorFinal;
    }

public:
    ArbolCapas() {
        raiz = NULL;
    }

    void insertar(int id) {
        raiz = insertarRecursivo(raiz, id);
    }

    NodoCapa* buscar(int id) {
        return buscarRecursivo(raiz, id);
    }

    void mostrarInorden() {
        if (raiz == NULL) {
            cout << "No hay capas cargadas." << endl;
        } else {
            mostrarInordenRecursivo(raiz);
        }
    }

    bool cargarArchivo(string ruta) {
        ifstream archivo(ruta.c_str());

        if (!archivo.is_open()) {
            return false;
        }

        string texto = "";
        string linea = "";

        while (getline(archivo, linea)) {
            texto += linea;
            texto += "\n";
        }

        archivo.close();

        int i = 0;
        int idAutomatico = 1;

        while (i < texto.length()) {
            saltarEspacios(texto, i);

            if (i >= texto.length()) {
                break;
            }

            int idCapa = 0;

            if (isdigit(texto[i])) {
                idCapa = leerNumero(texto, i);
            } else if (texto[i] == '{') {
                idCapa = idAutomatico;
            } else {
                i++;
                continue;
            }

            saltarEspacios(texto, i);

            if (i < texto.length() && texto[i] == '{') {
                i++;
                insertar(idCapa);
                NodoCapa* capaActual = buscar(idCapa);

                if (idCapa >= idAutomatico) {
                    idAutomatico = idCapa + 1;
                }

                while (i < texto.length() && texto[i] != '}') {
                    saltarEspacios(texto, i);

                    if (i < texto.length() && texto[i] == '}') {
                        break;
                    }

                    if (i < texto.length() && isdigit(texto[i])) {
                        int x = leerNumero(texto, i);

                        saltarEspacios(texto, i);

                        if (i < texto.length() && texto[i] == ',') {
                            i++;
                        }

                        saltarEspacios(texto, i);

                        int y = leerNumero(texto, i);

                        saltarEspacios(texto, i);

                        if (i < texto.length() && texto[i] == ',') {
                            i++;
                        }

                        saltarEspacios(texto, i);

                        string color = leerColor(texto, i);

                        if (i < texto.length() && texto[i] == ';') {
                            i++;
                        }

                        if (capaActual != NULL && color != "") {
                            capaActual->matriz.insertar(x, y, color);
                        }
                    } else {
                        i++;
                    }
                }

                if (i < texto.length() && texto[i] == '}') {
                    i++;
                }
            } else {
                i++;
            }
        }

        return true;
    }

    bool generarImagenCapa(int id) {
        NodoCapa* capa = buscar(id);

        if (capa == NULL) {
            return false;
        }

        int ancho = capa->matriz.obtenerMaxX();
        int alto = capa->matriz.obtenerMaxY();

        if (ancho <= 0 || alto <= 0) {
            return false;
        }

        string nombreDot = "capa_" + to_string(id) + ".dot";
        string nombrePng = "capa_" + to_string(id) + ".png";

        ofstream archivo(nombreDot.c_str());

        if (!archivo.is_open()) {
            return false;
        }

        archivo << "digraph G {" << endl;
        archivo << "node [shape=plain]" << endl;
        archivo << "tabla [label=<" << endl;
        archivo << "<TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING=\"0\">" << endl;

        for (int y = 1; y <= alto; y++) {
            archivo << "<TR>" << endl;

            for (int x = 1; x <= ancho; x++) {
                string color = capa->matriz.obtenerColor(x, y);
                archivo << "<TD WIDTH=\"25\" HEIGHT=\"25\" BGCOLOR=\"" << color << "\"></TD>" << endl;
            }

            archivo << "</TR>" << endl;
        }

        archivo << "</TABLE>" << endl;
        archivo << ">];" << endl;
        archivo << "}" << endl;
        archivo.close();

        string comando = "dot -Tpng \"" + nombreDot + "\" -o \"" + nombrePng + "\"";
        int resultado = system(comando.c_str());

        if (resultado != 0) {
            cout << "Se creo el archivo .dot, pero Graphviz no genero el .png." << endl;
            cout << "Revise que Graphviz este instalado y agregado al PATH." << endl;
        }

        return true;
    }
    bool generarImagenRecorridoLimitado(int cantidad, int tipo) {
        if (raiz == NULL) {
            return false;
        }

        if (cantidad <= 0) {
            return false;
        }

        if (tipo < 1 || tipo > 3) {
            return false;
        }

        NodoCapa** lista = new NodoCapa*[cantidad];

        for (int i = 0; i < cantidad; i++) {
            lista[i] = NULL;
        }

        int contador = 0;
        string nombreRecorrido = "";

        if (tipo == 1) {
            nombreRecorrido = "preorden";
            recorrerPreordenLimitado(raiz, lista, contador, cantidad);
        } else if (tipo == 2) {
            nombreRecorrido = "inorden";
            recorrerInordenLimitado(raiz, lista, contador, cantidad);
        } else if (tipo == 3) {
            nombreRecorrido = "postorden";
            recorrerPostordenLimitado(raiz, lista, contador, cantidad);
        }

        if (contador <= 0) {
            delete[] lista;
            return false;
        }

        int ancho = 0;
        int alto = 0;

        for (int i = 0; i < contador; i++) {
            if (lista[i] != NULL) {
                if (lista[i]->matriz.obtenerMaxX() > ancho) {
                    ancho = lista[i]->matriz.obtenerMaxX();
                }

                if (lista[i]->matriz.obtenerMaxY() > alto) {
                    alto = lista[i]->matriz.obtenerMaxY();
                }
            }
        }

        if (ancho <= 0) {
            ancho = 1;
        }

        if (alto <= 0) {
            alto = 1;
        }

        string nombreDot = "recorrido_" + nombreRecorrido + "_" + to_string(cantidad) + ".dot";
        string nombrePng = "recorrido_" + nombreRecorrido + "_" + to_string(cantidad) + ".png";

        ofstream archivo(nombreDot.c_str());

        if (!archivo.is_open()) {
            delete[] lista;
            return false;
        }

        archivo << "digraph G {" << endl;
        archivo << "node [shape=plain]" << endl;
        archivo << "tabla [label=<" << endl;
        archivo << "<TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING=\"0\">" << endl;
        archivo << "<TR>" << endl;
        archivo << "<TD COLSPAN=\"" << ancho << "\" BGCOLOR=\"#FDE1E8\"><FONT POINT-SIZE=\"18\">Recorrido " << nombreRecorrido << "</FONT></TD>" << endl;
        archivo << "</TR>" << endl;
        archivo << "<TR>" << endl;
        archivo << "<TD COLSPAN=\"" << ancho << "\" BGCOLOR=\"#FFFFFF\">Capas: ";

        for (int i = 0; i < contador; i++) {
            archivo << lista[i]->id;

            if (i < contador - 1) {
                archivo << " - ";
            }
        }

        archivo << "</TD>" << endl;
        archivo << "</TR>" << endl;

        for (int y = 1; y <= alto; y++) {
            archivo << "<TR>" << endl;

            for (int x = 1; x <= ancho; x++) {
                string color = obtenerColorFinalRecorrido(lista, contador, x, y);
                archivo << "<TD WIDTH=\"25\" HEIGHT=\"25\" BGCOLOR=\"" << color << "\"></TD>" << endl;
            }

            archivo << "</TR>" << endl;
        }

        archivo << "</TABLE>" << endl;
        archivo << ">];" << endl;
        archivo << "}" << endl;
        archivo.close();

        string comando = "dot -Tpng \"" + nombreDot + "\" -o \"" + nombrePng + "\"";
        int resultado = system(comando.c_str());

        if (resultado != 0) {
            cout << "Se creo el archivo .dot, pero Graphviz no genero el .png." << endl;
            cout << "Revise que Graphviz este instalado y agregado al PATH." << endl;
        }

        delete[] lista;
        return true;
    }

    bool arbolVacio() {
        if (raiz == NULL) {
            return true;
        }

        return false;
    }

    void escribirArbolEnArchivo(ofstream& archivo) {
        escribirNodosArbol(raiz, archivo);
    }

    bool graficarArbolCapas() {
        if (raiz == NULL) {
            return false;
        }

        string nombreDot = "arbol_capas.dot";
        string nombrePng = "arbol_capas.png";

        ofstream archivo(nombreDot.c_str());

        if (!archivo.is_open()) {
            return false;
        }

        archivo << "digraph G {" << endl;
        archivo << "rankdir=TB;" << endl;
        archivo << "node [shape=box, style=filled, fillcolor=\"#FDE1E8\", color=\"#C2185B\", fontname=\"Arial\"];" << endl;
        archivo << "edge [color=\"#444444\"];" << endl;
        escribirNodosArbol(raiz, archivo);
        archivo << "}" << endl;
        archivo.close();

        string comando = "dot -Tpng \"" + nombreDot + "\" -o \"" + nombrePng + "\"";
        int resultado = system(comando.c_str());

        if (resultado != 0) {
            cout << "Se creo el archivo .dot, pero Graphviz no genero el .png." << endl;
            cout << "Revise que Graphviz este instalado y agregado al PATH." << endl;
        }

        return true;
    }


    bool graficarMatrizCapa(int id) {
        NodoCapa* capa = buscar(id);

        if (capa == NULL) {
            return false;
        }

        if (capa->matriz.contarPixeles() <= 0) {
            return false;
        }

        string nombreDot = "matriz_capa_" + to_string(id) + ".dot";
        string nombrePng = "matriz_capa_" + to_string(id) + ".png";

        return capa->matriz.graficarEstructura(nombreDot, nombrePng, id);
    }

};

#endif
