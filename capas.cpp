#ifndef CAPAS_CPP
#define CAPAS_CPP

#include <iostream>
#include <fstream>
#include <string>
#include <cctype>

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
};

#endif
