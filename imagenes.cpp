#ifndef IMAGENES_CPP
#define IMAGENES_CPP

#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <cstdlib>

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


    void borrarCapasImagen(NodoCapaImagen* primera) {
        NodoCapaImagen* actual = primera;

        while (actual != NULL) {
            NodoCapaImagen* borrar = actual;
            actual = actual->siguiente;
            delete borrar;
        }
    }

    string obtenerColorFinal(NodoImagen* imagen, int x, int y) {
        string colorFinal = "#FFFFFF";
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


    bool agregarImagenManual(int id, string capasTexto, ArbolCapas& arbolCapas) {
        if (id <= 0) {
            return false;
        }

        if (buscar(id) != NULL) {
            return false;
        }

        NodoImagen* nuevaImagen = insertarOrdenado(id);

        if (nuevaImagen == NULL) {
            return false;
        }

        int i = 0;

        while (i < capasTexto.length()) {
            if (isdigit(capasTexto[i])) {
                int idCapa = leerNumero(capasTexto, i);
                agregarCapaAImagen(nuevaImagen, idCapa, arbolCapas);
            } else {
                i++;
            }
        }

        return true;
    }

    bool eliminarImagen(int id) {
        NodoImagen* imagen = buscar(id);

        if (imagen == NULL) {
            return false;
        }

        borrarCapasImagen(imagen->primeraCapa);

        if (imagen->siguiente == imagen && imagen->anterior == imagen) {
            primero = NULL;
            delete imagen;
            return true;
        }

        imagen->anterior->siguiente = imagen->siguiente;
        imagen->siguiente->anterior = imagen->anterior;

        if (primero == imagen) {
            primero = imagen->siguiente;
        }

        delete imagen;
        return true;
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

    bool generarImagenPorId(int id) {
        NodoImagen* imagen = buscar(id);

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

        string nombreDot = "imagen_" + to_string(id) + ".dot";
        string nombrePng = "imagen_" + to_string(id) + ".png";

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
                    color = obtenerColorFinal(imagen, x, y);
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

    bool graficarListaImagenes() {
        if (primero == NULL) {
            return false;
        }

        string nombreDot = "lista_imagenes.dot";
        string nombrePng = "lista_imagenes.png";

        ofstream archivo(nombreDot.c_str());

        if (!archivo.is_open()) {
            return false;
        }

        archivo << "digraph G {" << endl;
        archivo << "rankdir=LR;" << endl;
        archivo << "node [shape=box, style=filled, fontname=\"Arial\"];" << endl;
        archivo << "edge [fontname=\"Arial\"];" << endl;
        archivo << "subgraph cluster_imagenes {" << endl;
        archivo << "label=\"Lista circular doble de imagenes\";" << endl;
        archivo << "color=\"#C2185B\";" << endl;

        NodoImagen* actual = primero;

        do {
            archivo << "imagen" << actual->id << " [label=\"Imagen " << actual->id << "\", fillcolor=\"#FDE1E8\", color=\"#C2185B\"];" << endl;
            actual = actual->siguiente;
        } while (actual != primero);

        actual = primero;

        do {
            archivo << "imagen" << actual->id << " -> imagen" << actual->siguiente->id << " [label=\"sig\", color=\"#C2185B\"];" << endl;
            archivo << "imagen" << actual->id << " -> imagen" << actual->anterior->id << " [label=\"ant\", color=\"#555555\"];" << endl;
            actual = actual->siguiente;
        } while (actual != primero);

        archivo << "}" << endl;

        actual = primero;

        do {
            NodoCapaImagen* capa = actual->primeraCapa;
            int contador = 1;
            string anterior = "imagen" + to_string(actual->id);

            if (capa == NULL) {
                string nombreNodo = "imagen" + to_string(actual->id) + "capa0";
                archivo << nombreNodo << " [label=\"Sin capas\", fillcolor=\"#FFFFFF\", color=\"#444444\"];" << endl;
                archivo << anterior << " -> " << nombreNodo << " [style=dashed, color=\"#444444\"];" << endl;
            }

            while (capa != NULL) {
                string nombreNodo = "imagen" + to_string(actual->id) + "capa" + to_string(contador);
                archivo << nombreNodo << " [label=\"Capa " << capa->idCapa;

                if (capa->capa != NULL) {
                    archivo << "\\nApunta al ABB";
                } else {
                    archivo << "\\nNo encontrada";
                }

                archivo << "\", fillcolor=\"#E0F7FA\", color=\"#007C89\"];" << endl;
                archivo << anterior << " -> " << nombreNodo << " [color=\"#007C89\"];" << endl;

                anterior = nombreNodo;
                contador++;
                capa = capa->siguiente;
            }

            actual = actual->siguiente;
        } while (actual != primero);

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

    bool graficarImagenYArbol(int id, ArbolCapas& arbolCapas) {
        NodoImagen* imagen = buscar(id);

        if (imagen == NULL) {
            return false;
        }

        if (arbolCapas.arbolVacio()) {
            return false;
        }

        string nombreDot = "imagen_arbol_" + to_string(id) + ".dot";
        string nombrePng = "imagen_arbol_" + to_string(id) + ".png";

        ofstream archivo(nombreDot.c_str());

        if (!archivo.is_open()) {
            return false;
        }

        archivo << "digraph G {" << endl;
        archivo << "rankdir=LR;" << endl;
        archivo << "node [shape=box, style=filled, fontname=\"Arial\"];" << endl;
        archivo << "edge [fontname=\"Arial\"];" << endl;

        archivo << "subgraph cluster_lista {" << endl;
        archivo << "label=\"Lista de capas de la imagen " << id << "\";" << endl;
        archivo << "color=\"#007C89\";" << endl;
        archivo << "imagen_seleccionada [label=\"Imagen " << id << "\", fillcolor=\"#E0F7FA\", color=\"#007C89\"];" << endl;

        NodoCapaImagen* capa = imagen->primeraCapa;
        int contador = 1;
        string anterior = "imagen_seleccionada";

        if (capa == NULL) {
            archivo << "lista_sin_capas [label=\"Sin capas\", fillcolor=\"#FFFFFF\", color=\"#444444\"];" << endl;
            archivo << "imagen_seleccionada -> lista_sin_capas [color=\"#444444\"];" << endl;
        }

        while (capa != NULL) {
            string nombreNodo = "lista_" + to_string(id) + "_" + to_string(contador);
            archivo << nombreNodo << " [label=\"Capa " << capa->idCapa;

            if (capa->capa != NULL) {
                archivo << "\\nApunta al ABB";
            } else {
                archivo << "\\nNo encontrada";
            }

            archivo << "\", fillcolor=\"#E0F7FA\", color=\"#007C89\"];" << endl;
            archivo << anterior << " -> " << nombreNodo << " [color=\"#007C89\"];" << endl;

            anterior = nombreNodo;
            contador++;
            capa = capa->siguiente;
        }

        archivo << "}" << endl;

        archivo << "subgraph cluster_arbol {" << endl;
        archivo << "label=\"Arbol ABB de capas\";" << endl;
        archivo << "color=\"#C2185B\";" << endl;
        archivo << "node [shape=box, style=filled, fillcolor=\"#FDE1E8\", color=\"#C2185B\", fontname=\"Arial\"];" << endl;
        arbolCapas.escribirArbolEnArchivo(archivo);
        archivo << "}" << endl;

        capa = imagen->primeraCapa;
        contador = 1;

        while (capa != NULL) {
            if (capa->capa != NULL) {
                archivo << "lista_" << id << "_" << contador << " -> nodo" << capa->idCapa << " [style=dashed, color=\"#E91E63\", label=\"apunta\"];" << endl;
            }

            contador++;
            capa = capa->siguiente;
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

#endif
