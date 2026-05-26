#ifndef USUARIOS_CPP
#define USUARIOS_CPP

#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <cstdlib>

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


    string obtenerColorFinalUsuario(NodoImagen* imagen, int x, int y) {
        string colorFinal = "#FFFFFF";

        if (imagen == NULL) {
            return colorFinal;
        }

        NodoCapaImagen* actual = imagen->primeraCapa;

        while (actual != NULL) {
            if (actual->capa != NULL) {
                string colorCapa = actual->capa->matriz.obtenerColorReal(x, y);

                if (colorCapa != "") {
                    colorFinal = colorCapa;
                }
            }

            actual = actual->siguiente;
        }

        return colorFinal;
    }

    NodoImagen* buscarImagenEnUsuario(NodoUsuario* usuario, int idImagen) {
        if (usuario == NULL) {
            return NULL;
        }

        NodoImagenUsuario* actual = usuario->primeraImagen;

        while (actual != NULL) {
            if (actual->idImagen == idImagen) {
                return actual->imagen;
            }

            actual = actual->siguiente;
        }

        return NULL;
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


    bool generarImagenDeUsuario(string nombre, int idImagen) {
        NodoUsuario* usuario = buscar(nombre);

        if (usuario == NULL) {
            return false;
        }

        NodoImagen* imagen = buscarImagenEnUsuario(usuario, idImagen);

        if (imagen == NULL) {
            return false;
        }

        int ancho = 0;
        int alto = 0;
        NodoCapaImagen* actual = imagen->primeraCapa;

        while (actual != NULL) {
            if (actual->capa != NULL) {
                if (actual->capa->matriz.obtenerMaxX() > ancho) {
                    ancho = actual->capa->matriz.obtenerMaxX();
                }

                if (actual->capa->matriz.obtenerMaxY() > alto) {
                    alto = actual->capa->matriz.obtenerMaxY();
                }
            }

            actual = actual->siguiente;
        }

        if (ancho <= 0) {
            ancho = 1;
        }

        if (alto <= 0) {
            alto = 1;
        }

        string nombreDot = "usuario_" + nombre + "_imagen_" + to_string(idImagen) + ".dot";
        string nombrePng = "usuario_" + nombre + "_imagen_" + to_string(idImagen) + ".png";

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
                string color = "#FFFFFF";

                if (imagen->primeraCapa == NULL) {
                    color = "#000000";
                } else {
                    color = obtenerColorFinalUsuario(imagen, x, y);
                }

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

};

#endif
