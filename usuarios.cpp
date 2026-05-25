#ifndef USUARIOS_CPP
#define USUARIOS_CPP

#include <iostream>
#include <fstream>
#include <string>
#include <cctype>

#ifndef IMAGENES_CPP
#include "imagenes.cpp"
#endif

using namespace std;

struct NodoImagenUsuario {
    int idImagen;
    NodoImagen* imagen;
    NodoImagenUsuario* siguiente;

    NodoImagenUsuario(int pidImagen, NodoImagen* pimagen) {
        idImagen = pidImagen;
        imagen = pimagen;
        siguiente = NULL;
    }
};

struct NodoUsuario {
    string nombre;
    NodoImagenUsuario* primeraImagen;
    NodoUsuario* izquierda;
    NodoUsuario* derecha;

    NodoUsuario(string pnombre) {
        nombre = pnombre;
        primeraImagen = NULL;
        izquierda = NULL;
        derecha = NULL;
    }
};

class ArbolUsuarios {
private:
    NodoUsuario* raiz;

    NodoUsuario* insertarRecursivo(NodoUsuario* actual, string nombre) {
        if (actual == NULL) {
            actual = new NodoUsuario(nombre);
            return actual;
        }

        if (nombre < actual->nombre) {
            actual->izquierda = insertarRecursivo(actual->izquierda, nombre);
        } else if (nombre > actual->nombre) {
            actual->derecha = insertarRecursivo(actual->derecha, nombre);
        }

        return actual;
    }

    NodoUsuario* buscarRecursivo(NodoUsuario* actual, string nombre) {
        if (actual == NULL) {
            return NULL;
        }

        if (actual->nombre == nombre) {
            return actual;
        }

        if (nombre < actual->nombre) {
            return buscarRecursivo(actual->izquierda, nombre);
        } else {
            return buscarRecursivo(actual->derecha, nombre);
        }
    }

    void mostrarInordenRecursivo(NodoUsuario* actual) {
        if (actual != NULL) {
            mostrarInordenRecursivo(actual->izquierda);
            cout << "Usuario: " << actual->nombre << " - Imagenes: " << contarImagenes(actual) << endl;
            mostrarInordenRecursivo(actual->derecha);
        }
    }

    int contarImagenes(NodoUsuario* usuario) {
        int contador = 0;

        if (usuario == NULL) {
            return contador;
        }

        NodoImagenUsuario* actual = usuario->primeraImagen;

        while (actual != NULL) {
            contador++;
            actual = actual->siguiente;
        }

        return contador;
    }

    void saltarEspacios(string texto, int& i) {
        while (i < texto.length() && isspace(texto[i])) {
            i++;
        }
    }

    string leerNombre(string texto, int& i) {
        string nombre = "";

        while (i < texto.length() && texto[i] != ':' && texto[i] != ';' && texto[i] != '\n' && texto[i] != '\r') {
            if (!isspace(texto[i])) {
                nombre += texto[i];
            }

            i++;
        }

        return nombre;
    }

    int leerNumero(string texto, int& i) {
        int numero = 0;

        while (i < texto.length() && isdigit(texto[i])) {
            numero = numero * 10 + (texto[i] - '0');
            i++;
        }

        return numero;
    }

    void agregarImagenAUsuario(NodoUsuario* usuario, int idImagen, ListaImagenes& listaImagenes) {
        if (usuario == NULL) {
            return;
        }

        NodoImagen* imagenEncontrada = listaImagenes.buscar(idImagen);
        NodoImagenUsuario* nueva = new NodoImagenUsuario(idImagen, imagenEncontrada);

        if (usuario->primeraImagen == NULL) {
            usuario->primeraImagen = nueva;
        } else {
            NodoImagenUsuario* actual = usuario->primeraImagen;

            while (actual->siguiente != NULL) {
                actual = actual->siguiente;
            }

            actual->siguiente = nueva;
        }
    }

public:
    ArbolUsuarios() {
        raiz = NULL;
    }

    void insertar(string nombre) {
        raiz = insertarRecursivo(raiz, nombre);
    }

    NodoUsuario* buscar(string nombre) {
        return buscarRecursivo(raiz, nombre);
    }

    bool cargarArchivo(string ruta, ListaImagenes& listaImagenes) {
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

            string nombre = leerNombre(texto, i);

            if (nombre != "") {
                insertar(nombre);
                NodoUsuario* usuarioActual = buscar(nombre);

                saltarEspacios(texto, i);

                if (i < texto.length() && texto[i] == ':') {
                    i++;
                }

                while (i < texto.length() && texto[i] != ';') {
                    saltarEspacios(texto, i);

                    if (i < texto.length() && isdigit(texto[i])) {
                        int idImagen = leerNumero(texto, i);
                        agregarImagenAUsuario(usuarioActual, idImagen, listaImagenes);
                    } else {
                        i++;
                    }

                    saltarEspacios(texto, i);

                    if (i < texto.length() && texto[i] == ',') {
                        i++;
                    }
                }

                if (i < texto.length() && texto[i] == ';') {
                    i++;
                }
            } else {
                i++;
            }
        }

        return true;
    }

    void mostrarUsuarios() {
        if (raiz == NULL) {
            cout << "No hay usuarios cargados." << endl;
        } else {
            mostrarInordenRecursivo(raiz);
        }
    }

    void mostrarDetalleUsuario(string nombre) {
        NodoUsuario* usuario = buscar(nombre);

        if (usuario == NULL) {
            cout << "No existe ese usuario." << endl;
            return;
        }

        cout << "Usuario encontrado: " << usuario->nombre << endl;
        cout << "Imagenes registradas:" << endl;

        NodoImagenUsuario* actual = usuario->primeraImagen;
        int contador = 1;

        if (actual == NULL) {
            cout << "Este usuario no tiene imagenes." << endl;
        }

        while (actual != NULL) {
            cout << contador << ". Imagen " << actual->idImagen;

            if (actual->imagen != NULL) {
                cout << " si apunta a la lista circular";
            } else {
                cout << " no encontrada en la lista circular";
            }

            cout << endl;
            contador++;
            actual = actual->siguiente;
        }
    }
};

#endif
