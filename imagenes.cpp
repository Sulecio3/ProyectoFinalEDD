#ifndef IMAGENES_CPP
#define IMAGENES_CPP

#include <iostream>
#include <fstream>
#include <string>
#include <cctype>

#ifndef CAPAS_CPP
#include "capas.cpp"
#endif

using namespace std;

struct NodoCapaImagen {
    int idCapa;
    NodoCapa* capa;
    NodoCapaImagen* siguiente;

    NodoCapaImagen(int pidCapa, NodoCapa* pcapa) {
        idCapa = pidCapa;
        capa = pcapa;
        siguiente = NULL;
    }
};

struct NodoImagen {
    int id;
    NodoCapaImagen* primeraCapa;
    NodoImagen* siguiente;
    NodoImagen* anterior;

    NodoImagen(int pid) {
        id = pid;
        primeraCapa = NULL;
        siguiente = NULL;
        anterior = NULL;
    }
};

class ListaImagenes {
private:
    NodoImagen* primero;

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

    void agregarCapaAImagen(NodoImagen* imagen, int idCapa, ArbolCapas& arbolCapas) {
        if (imagen == NULL) {
            return;
        }

        NodoCapa* capaEncontrada = arbolCapas.buscar(idCapa);
        NodoCapaImagen* nueva = new NodoCapaImagen(idCapa, capaEncontrada);

        if (imagen->primeraCapa == NULL) {
            imagen->primeraCapa = nueva;
        } else {
            NodoCapaImagen* actual = imagen->primeraCapa;

            while (actual->siguiente != NULL) {
                actual = actual->siguiente;
            }

            actual->siguiente = nueva;
        }
    }

public:
    ListaImagenes() {
        primero = NULL;
    }

    NodoImagen* buscar(int id) {
        if (primero == NULL) {
            return NULL;
        }

        NodoImagen* actual = primero;

        do {
            if (actual->id == id) {
                return actual;
            }

            actual = actual->siguiente;
        } while (actual != primero);

        return NULL;
    }

    NodoImagen* insertarOrdenado(int id) {
        NodoImagen* encontrada = buscar(id);

        if (encontrada != NULL) {
            return encontrada;
        }

        NodoImagen* nueva = new NodoImagen(id);

        if (primero == NULL) {
            primero = nueva;
            nueva->siguiente = nueva;
            nueva->anterior = nueva;
            return nueva;
        }

        NodoImagen* ultimo = primero->anterior;

        if (id < primero->id) {
            nueva->siguiente = primero;
            nueva->anterior = ultimo;
            ultimo->siguiente = nueva;
            primero->anterior = nueva;
            primero = nueva;
            return nueva;
        }

        NodoImagen* actual = primero;

        while (actual->siguiente != primero && actual->siguiente->id < id) {
            actual = actual->siguiente;
        }

        nueva->siguiente = actual->siguiente;
        nueva->anterior = actual;
        actual->siguiente->anterior = nueva;
        actual->siguiente = nueva;

        return nueva;
    }

    bool cargarArchivo(string ruta, ArbolCapas& arbolCapas) {
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

        while (i < texto.length()) {
            saltarEspacios(texto, i);

            if (i >= texto.length()) {
                break;
            }

            if (isdigit(texto[i])) {
                int idImagen = leerNumero(texto, i);
                NodoImagen* imagenActual = insertarOrdenado(idImagen);

                saltarEspacios(texto, i);

                if (i < texto.length() && texto[i] == '{') {
                    i++;

                    while (i < texto.length() && texto[i] != '}') {
                        saltarEspacios(texto, i);

                        if (i < texto.length() && isdigit(texto[i])) {
                            int idCapa = leerNumero(texto, i);
                            agregarCapaAImagen(imagenActual, idCapa, arbolCapas);
                        } else {
                            i++;
                        }

                        saltarEspacios(texto, i);

                        if (i < texto.length() && texto[i] == ',') {
                            i++;
                        }
                    }

                    if (i < texto.length() && texto[i] == '}') {
                        i++;
                    }
                }
            } else {
                i++;
            }
        }

        return true;
    }

    void mostrarImagenes() {
        if (primero == NULL) {
            cout << "No hay imagenes cargadas." << endl;
            return;
        }

        NodoImagen* actual = primero;

        do {
            cout << "Imagen " << actual->id << ": ";

            NodoCapaImagen* capa = actual->primeraCapa;

            if (capa == NULL) {
                cout << "sin capas";
            }

            while (capa != NULL) {
                cout << capa->idCapa;

                if (capa->siguiente != NULL) {
                    cout << " -> ";
                }

                capa = capa->siguiente;
            }

            cout << endl;
            actual = actual->siguiente;
        } while (actual != primero);
    }

    void mostrarDetalleImagen(int id) {
        NodoImagen* imagen = buscar(id);

        if (imagen == NULL) {
            cout << "No existe esa imagen." << endl;
            return;
        }

        cout << "Imagen encontrada: " << imagen->id << endl;
        cout << "Capas en el orden de superposicion:" << endl;

        NodoCapaImagen* capa = imagen->primeraCapa;
        int contador = 1;

        if (capa == NULL) {
            cout << "Esta imagen no tiene capas." << endl;
        }

        while (capa != NULL) {
            cout << contador << ". Capa " << capa->idCapa;

            if (capa->capa != NULL) {
                cout << " si apunta al ABB";
            } else {
                cout << " no encontrada en el ABB";
            }

            cout << endl;
            contador++;
            capa = capa->siguiente;
        }
    }
};

#endif
