#ifndef CAPAS_CPP
#define CAPAS_CPP

#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <cstdlib>
#include <new>
#include <locale>

using namespace std;

struct NodoPixel {
    int x;
    int y;
    string color;
    NodoPixel* derecha;
    NodoPixel* abajo;
    NodoPixel* siguienteHash;

    NodoPixel(int px, int py, string pcolor) {
        x = px;
        y = py;
        color = pcolor;
        derecha = NULL;
        abajo = NULL;
        siguienteHash = NULL;
    }
};

struct NodoEncabezado {
    int id;
    NodoPixel* acceso;
    NodoEncabezado* siguiente;
    NodoEncabezado* siguienteHash;

    NodoEncabezado(int pid) {
        id = pid;
        acceso = NULL;
        siguiente = NULL;
        siguienteHash = NULL;
    }
};

class ListaEncabezados {
private:
    NodoEncabezado* primero;
    NodoEncabezado** tabla;
    int capacidad;
    int cantidad;

    int hashId(int id) {
        if (id < 0) {
            id = id * -1;
        }
        if (capacidad <= 0) {
            return 0;
        }
        return id % capacidad;
    }

    void iniciarTabla(int nuevaCapacidad) {
        capacidad = nuevaCapacidad;
        tabla = new(nothrow) NodoEncabezado*[capacidad];

        if (tabla == NULL) {
            capacidad = 0;
            return;
        }

        for (int i = 0; i < capacidad; i++) {
            tabla[i] = NULL;
        }
    }

    void rehash() {
        if (capacidad <= 0) {
            iniciarTabla(1024);
            return;
        }

        int nuevaCapacidad = capacidad * 2;
        NodoEncabezado** nuevaTabla = new(nothrow) NodoEncabezado*[nuevaCapacidad];

        if (nuevaTabla == NULL) {
            return;
        }

        for (int i = 0; i < nuevaCapacidad; i++) {
            nuevaTabla[i] = NULL;
        }

        NodoEncabezado* actual = primero;

        while (actual != NULL) {
            int pos = actual->id;
            if (pos < 0) {
                pos = pos * -1;
            }
            pos = pos % nuevaCapacidad;
            actual->siguienteHash = nuevaTabla[pos];
            nuevaTabla[pos] = actual;
            actual = actual->siguiente;
        }

        delete[] tabla;
        tabla = nuevaTabla;
        capacidad = nuevaCapacidad;
    }

public:
    ListaEncabezados() {
        primero = NULL;
        tabla = NULL;
        capacidad = 0;
        cantidad = 0;
        iniciarTabla(1024);
    }

    ~ListaEncabezados() {
        limpiar();
    }

    void limpiar() {
        NodoEncabezado* actual = primero;

        while (actual != NULL) {
            NodoEncabezado* borrar = actual;
            actual = actual->siguiente;
            delete borrar;
        }

        primero = NULL;
        cantidad = 0;

        if (tabla != NULL) {
            delete[] tabla;
            tabla = NULL;
        }

        capacidad = 0;
    }

    NodoEncabezado* obtenerPrimero() {
        return primero;
    }

    NodoEncabezado* buscar(int id) {
        if (tabla == NULL || capacidad <= 0) {
            return NULL;
        }

        int pos = hashId(id);
        NodoEncabezado* actual = tabla[pos];

        while (actual != NULL) {
            if (actual->id == id) {
                return actual;
            }
            actual = actual->siguienteHash;
        }

        return NULL;
    }

    NodoEncabezado* insertar(int id) {
        if (id < 0) {
            return NULL;
        }

        if (tabla == NULL || capacidad <= 0) {
            iniciarTabla(1024);
        }

        NodoEncabezado* encontrado = buscar(id);
        if (encontrado != NULL) {
            return encontrado;
        }

        if (cantidad > capacidad * 2) {
            rehash();
        }

        NodoEncabezado* nuevo = new(nothrow) NodoEncabezado(id);

        if (nuevo == NULL) {
            return NULL;
        }

        nuevo->siguiente = primero;
        primero = nuevo;

        int pos = hashId(id);
        nuevo->siguienteHash = tabla[pos];
        tabla[pos] = nuevo;
        cantidad++;

        return nuevo;
    }
};

class MatrizDispersa {
private:
    ListaEncabezados filas;
    ListaEncabezados columnas;
    NodoPixel** tablaPixeles;
    int capacidadHash;
    int maxX;
    int maxY;
    int totalPixeles;
    bool sinMemoria;

    int hashPixel(int x, int y) {
        long long valor = (long long)x * 73856093LL + (long long)y * 19349663LL;
        if (valor < 0) {
            valor = valor * -1;
        }
        if (capacidadHash <= 0) {
            return 0;
        }
        return (int)(valor % capacidadHash);
    }

    void iniciarHash(int nuevaCapacidad) {
        capacidadHash = nuevaCapacidad;
        tablaPixeles = new(nothrow) NodoPixel*[capacidadHash];

        if (tablaPixeles == NULL) {
            capacidadHash = 0;
            sinMemoria = true;
            return;
        }

        for (int i = 0; i < capacidadHash; i++) {
            tablaPixeles[i] = NULL;
        }
    }

    void rehashPixeles() {
        if (capacidadHash <= 0) {
            iniciarHash(2048);
            return;
        }

        int nuevaCapacidad = capacidadHash * 2;
        NodoPixel** nuevaTabla = new(nothrow) NodoPixel*[nuevaCapacidad];

        if (nuevaTabla == NULL) {
            return;
        }

        for (int i = 0; i < nuevaCapacidad; i++) {
            nuevaTabla[i] = NULL;
        }

        NodoEncabezado* fila = filas.obtenerPrimero();

        while (fila != NULL) {
            NodoPixel* pixel = fila->acceso;

            while (pixel != NULL) {
                long long valor = (long long)pixel->x * 73856093LL + (long long)pixel->y * 19349663LL;
                if (valor < 0) {
                    valor = valor * -1;
                }
                int pos = (int)(valor % nuevaCapacidad);
                pixel->siguienteHash = nuevaTabla[pos];
                nuevaTabla[pos] = pixel;
                pixel = pixel->derecha;
            }

            fila = fila->siguiente;
        }

        delete[] tablaPixeles;
        tablaPixeles = nuevaTabla;
        capacidadHash = nuevaCapacidad;
    }

    bool esHex(char c) {
        c = toupper((unsigned char)c);
        if (c >= '0' && c <= '9') {
            return true;
        }
        if (c >= 'A' && c <= 'F') {
            return true;
        }
        return false;
    }

    int valorHexadecimal(char c) {
        c = toupper((unsigned char)c);
        if (c >= '0' && c <= '9') {
            return c - '0';
        }
        if (c >= 'A' && c <= 'F') {
            return c - 'A' + 10;
        }
        return 0;
    }

    string normalizarColor(string color) {
        string limpio = "";

        for (int i = 0; i < (int)color.length(); i++) {
            if (!isspace((unsigned char)color[i]) && color[i] != ',') {
                limpio += color[i];
            }
        }

        if (limpio.length() == 6) {
            limpio = "#" + limpio;
        }

        if (limpio.length() != 7) {
            return "";
        }

        if (limpio[0] != '#') {
            return "";
        }

        for (int i = 1; i < (int)limpio.length(); i++) {
            if (esHex(limpio[i]) == false) {
                return "";
            }
            limpio[i] = toupper((unsigned char)limpio[i]);
        }

        return limpio;
    }

    string colorLetra(string color) {
        if (color.length() < 7) {
            return "#000000";
        }

        int rojo = valorHexadecimal(color[1]) * 16 + valorHexadecimal(color[2]);
        int verde = valorHexadecimal(color[3]) * 16 + valorHexadecimal(color[4]);
        int azul = valorHexadecimal(color[5]) * 16 + valorHexadecimal(color[6]);
        int brillo = (rojo * 299 + verde * 587 + azul * 114) / 1000;

        if (brillo < 140) {
            return "#FFFFFF";
        }

        return "#000000";
    }

    NodoPixel* buscarPixel(int x, int y) {
        if (tablaPixeles == NULL || capacidadHash <= 0) {
            return NULL;
        }

        int pos = hashPixel(x, y);
        NodoPixel* actual = tablaPixeles[pos];

        while (actual != NULL) {
            if (actual->x == x && actual->y == y) {
                return actual;
            }
            actual = actual->siguienteHash;
        }

        return NULL;
    }

public:
    MatrizDispersa() {
        tablaPixeles = NULL;
        capacidadHash = 0;
        maxX = 0;
        maxY = 0;
        totalPixeles = 0;
        sinMemoria = false;
        iniciarHash(2048);
    }

    ~MatrizDispersa() {
        limpiar();
    }

    void limpiar() {
        NodoEncabezado* fila = filas.obtenerPrimero();

        while (fila != NULL) {
            NodoPixel* pixel = fila->acceso;

            while (pixel != NULL) {
                NodoPixel* borrar = pixel;
                pixel = pixel->derecha;
                delete borrar;
            }

            fila = fila->siguiente;
        }

        filas.limpiar();
        columnas.limpiar();

        if (tablaPixeles != NULL) {
            delete[] tablaPixeles;
            tablaPixeles = NULL;
        }

        capacidadHash = 0;
        maxX = 0;
        maxY = 0;
        totalPixeles = 0;
        sinMemoria = false;
    }

    int obtenerMaxX() {
        return maxX;
    }

    int obtenerMaxY() {
        return maxY;
    }

    int contarPixeles() {
        return totalPixeles;
    }

    bool huboErrorMemoria() {
        return sinMemoria;
    }

    bool insertar(int x, int y, string color) {
        if (x <= 0 || y <= 0) {
            return false;
        }

        color = normalizarColor(color);

        if (color == "") {
            return false;
        }

        if (tablaPixeles == NULL || capacidadHash <= 0) {
            iniciarHash(2048);
        }

        NodoPixel* repetido = buscarPixel(x, y);

        if (repetido != NULL) {
            repetido->color = color;
            return true;
        }

        if (totalPixeles > capacidadHash * 2) {
            rehashPixeles();
        }

        NodoEncabezado* fila = filas.insertar(y);
        NodoEncabezado* columna = columnas.insertar(x);

        if (fila == NULL || columna == NULL) {
            sinMemoria = true;
            return false;
        }

        NodoPixel* nuevo = new(nothrow) NodoPixel(x, y, color);

        if (nuevo == NULL) {
            sinMemoria = true;
            return false;
        }

        nuevo->derecha = fila->acceso;
        fila->acceso = nuevo;

        nuevo->abajo = columna->acceso;
        columna->acceso = nuevo;

        int pos = hashPixel(x, y);
        nuevo->siguienteHash = tablaPixeles[pos];
        tablaPixeles[pos] = nuevo;

        totalPixeles++;

        if (x > maxX) {
            maxX = x;
        }

        if (y > maxY) {
            maxY = y;
        }

        return true;
    }

    void mostrarPixeles() {
        NodoEncabezado* fila = filas.obtenerPrimero();
        int mostrados = 0;
        int limite = 200;

        while (fila != NULL) {
            NodoPixel* pixel = fila->acceso;

            while (pixel != NULL) {
                if (mostrados < limite) {
                    cout << "(" << pixel->x << "," << pixel->y << ") " << pixel->color << endl;
                }

                mostrados++;
                pixel = pixel->derecha;
            }

            fila = fila->siguiente;
        }

        if (mostrados > limite) {
            cout << "Se mostraron solo los primeros " << limite << " pixeles de " << mostrados << "." << endl;
        }
    }

    string obtenerColor(int x, int y) {
        NodoPixel* pixel = buscarPixel(x, y);

        if (pixel == NULL) {
            return "#FFFFFF";
        }

        return pixel->color;
    }

    string obtenerColorReal(int x, int y) {
        NodoPixel* pixel = buscarPixel(x, y);

        if (pixel == NULL) {
            return "";
        }

        return pixel->color;
    }

    bool tienePixelesEntreFilas(int inicioY, int finY) {
        NodoEncabezado* fila = filas.obtenerPrimero();

        while (fila != NULL) {
            if (fila->id >= inicioY && fila->id <= finY && fila->acceso != NULL) {
                return true;
            }
            fila = fila->siguiente;
        }

        return false;
    }

    int contarPixelesEntreFilas(int inicioY, int finY) {
        int cantidad = 0;
        NodoEncabezado* fila = filas.obtenerPrimero();

        while (fila != NULL) {
            if (fila->id >= inicioY && fila->id <= finY) {
                NodoPixel* pixel = fila->acceso;

                while (pixel != NULL) {
                    cantidad++;
                    pixel = pixel->derecha;
                }
            }
            fila = fila->siguiente;
        }

        return cantidad;
    }

    void marcarColumnasUsadas(bool* usadas, int inicioY, int finY) {
        for (int i = 0; i <= maxX; i++) {
            usadas[i] = false;
        }

        NodoEncabezado* fila = filas.obtenerPrimero();

        while (fila != NULL) {
            if (fila->id >= inicioY && fila->id <= finY) {
                NodoPixel* pixel = fila->acceso;

                while (pixel != NULL) {
                    if (pixel->x >= 1 && pixel->x <= maxX) {
                        usadas[pixel->x] = true;
                    }
                    pixel = pixel->derecha;
                }
            }
            fila = fila->siguiente;
        }
    }

    bool graficarEstructuraRango(string nombreDot, string nombrePng, int idCapa, int inicioY, int finY, int numeroParte, int totalPartes) {
        char q = 34;

        if (tienePixelesEntreFilas(inicioY, finY) == false) {
            return false;
        }

        bool* columnasUsadas = new(nothrow) bool[maxX + 1];
        if (columnasUsadas == NULL) {
            return false;
        }

        marcarColumnasUsadas(columnasUsadas, inicioY, finY);

        ofstream archivo(nombreDot.c_str());

        if (!archivo.is_open()) {
            delete[] columnasUsadas;
            return false;
        }

        int pixelesParte = contarPixelesEntreFilas(inicioY, finY);

        archivo << "digraph G {" << endl;
        archivo << "graph [rankdir=TB, bgcolor=" << q << "white" << q << ", splines=false, nodesep=" << q << "0.22" << q << ", ranksep=" << q << "0.25" << q << "];" << endl;
        archivo << "node [shape=box, style=filled, fontname=" << q << "Arial" << q << ", fontsize=9, margin=" << q << "0.04" << q << "];" << endl;
        archivo << "edge [color=" << q << "#222222" << q << ", arrowsize=0.45, dir=both];" << endl;

        archivo << "titulo [label=" << q << "Matriz dispersa de capa " << idCapa << "\\nParte " << numeroParte << " de " << totalPartes << "    Filas " << inicioY << " a " << finY << "\\nPixeles en esta parte: " << pixelesParte << q << ", fillcolor=" << q << "#FDE1E8" << q << ", color=" << q << "#C2185B" << q << ", fontsize=16];" << endl;
        archivo << "matriz [label=" << q << "matriz" << q << ", fillcolor=" << q << "#FDE1E8" << q << ", color=" << q << "#C2185B" << q << "];" << endl;
        archivo << "titulo -> matriz [style=invis, dir=none];" << endl;

        for (int x = 1; x <= maxX; x++) {
            if (columnasUsadas[x] == true) {
                archivo << "col" << x << " [label=" << q << "Columna " << x << q << ", fillcolor=" << q << "#DDF7F8" << q << ", color=" << q << "#008B95" << q << "];" << endl;
            }
        }

        NodoEncabezado* fila = filas.obtenerPrimero();

        while (fila != NULL) {
            if (fila->id >= inicioY && fila->id <= finY) {
                archivo << "fila" << fila->id << " [label=" << q << "Fila " << fila->id << q << ", fillcolor=" << q << "#FFF3E0" << q << ", color=" << q << "#EF6C00" << q << "];" << endl;

                NodoPixel* pixel = fila->acceso;
                while (pixel != NULL) {
                    archivo << "p" << pixel->x << "_" << pixel->y << " [label=" << q << pixel->color << q << ", fillcolor=" << q << "#FFFFFF" << q << ", fontcolor=" << q << "#000000" << q << ", color=" << q << "#444444" << q << ", width=0.68, height=0.25];" << endl;
                    pixel = pixel->derecha;
                }
            }
            fila = fila->siguiente;
        }

        archivo << "{ rank=same; matriz; ";
        for (int x = 1; x <= maxX; x++) {
            if (columnasUsadas[x] == true) {
                archivo << "col" << x << "; ";
            }
        }
        archivo << "}" << endl;

        fila = filas.obtenerPrimero();
        while (fila != NULL) {
            if (fila->id >= inicioY && fila->id <= finY) {
                archivo << "{ rank=same; fila" << fila->id << "; ";
                NodoPixel* pixel = fila->acceso;
                while (pixel != NULL) {
                    archivo << "p" << pixel->x << "_" << pixel->y << "; ";
                    pixel = pixel->derecha;
                }
                archivo << "}" << endl;
            }
            fila = fila->siguiente;
        }

        int anteriorCol = -1;
        for (int x = 1; x <= maxX; x++) {
            if (columnasUsadas[x] == true) {
                if (anteriorCol == -1) {
                    archivo << "matriz -> col" << x << " [color=" << q << "#008B95" << q << "];" << endl;
                } else {
                    archivo << "col" << anteriorCol << " -> col" << x << " [color=" << q << "#008B95" << q << "];" << endl;
                }
                anteriorCol = x;
            }
        }

        int anteriorFila = -1;
        for (int y = inicioY; y <= finY; y++) {
            NodoEncabezado* filaActual = filas.buscar(y);
            if (filaActual != NULL) {
                if (anteriorFila == -1) {
                    archivo << "matriz -> fila" << y << " [color=" << q << "#EF6C00" << q << "];" << endl;
                } else {
                    archivo << "fila" << anteriorFila << " -> fila" << y << " [color=" << q << "#EF6C00" << q << "];" << endl;
                }
                anteriorFila = y;
            }
        }

        for (int y = inicioY; y <= finY; y++) {
            NodoEncabezado* filaActual = filas.buscar(y);
            if (filaActual != NULL) {
                NodoPixel* pixel = filaActual->acceso;
                if (pixel != NULL) {
                    archivo << "fila" << y << " -> p" << pixel->x << "_" << pixel->y << " [color=" << q << "#444444" << q << "];" << endl;
                    while (pixel->derecha != NULL) {
                        archivo << "p" << pixel->x << "_" << pixel->y << " -> p" << pixel->derecha->x << "_" << pixel->derecha->y << " [color=" << q << "#444444" << q << "];" << endl;
                        pixel = pixel->derecha;
                    }
                }
            }
        }

        for (int x = 1; x <= maxX; x++) {
            if (columnasUsadas[x] == true) {
                NodoPixel* primeroColumna = NULL;
                NodoPixel* anteriorPixel = NULL;
                NodoEncabezado* columnaActual = columnas.buscar(x);

                if (columnaActual != NULL) {
                    NodoPixel* pixel = columnaActual->acceso;

                    while (pixel != NULL) {
                        if (pixel->y >= inicioY && pixel->y <= finY) {
                            if (primeroColumna == NULL) {
                                primeroColumna = pixel;
                            }

                            if (anteriorPixel != NULL) {
                                archivo << "p" << anteriorPixel->x << "_" << anteriorPixel->y << " -> p" << pixel->x << "_" << pixel->y << " [color=" << q << "#008B95" << q << "];" << endl;
                            }
                            anteriorPixel = pixel;
                        }
                        pixel = pixel->abajo;
                    }
                }

                if (primeroColumna != NULL) {
                    archivo << "col" << x << " -> p" << primeroColumna->x << "_" << primeroColumna->y << " [color=" << q << "#008B95" << q << "];" << endl;
                }
            }
        }

        archivo << "}" << endl;
        archivo.close();

        delete[] columnasUsadas;

        string comando = "dot -Tpng \"" + nombreDot + "\" -o \"" + nombrePng + "\"";
        int resultado = system(comando.c_str());

        if (resultado != 0) {
            string nombreSvg = nombreDot;
            int pos = (int)nombreSvg.find(".dot");
            if (pos >= 0) {
                nombreSvg = nombreSvg.substr(0, pos) + ".svg";
            } else {
                nombreSvg = nombreDot + ".svg";
            }
            string comandoSvg = "dot -Tsvg \"" + nombreDot + "\" -o \"" + nombreSvg + "\"";
            system(comandoSvg.c_str());
        }

        return true;
    }

    bool graficarIndicePartes(string nombreDot, string nombrePng, int idCapa, int totalPartes, int filasPorParte) {
        char q = 34;
        ofstream archivo(nombreDot.c_str());

        if (!archivo.is_open()) {
            return false;
        }

        archivo << "digraph G {" << endl;
        archivo << "rankdir=TB;" << endl;
        archivo << "node [shape=box, style=filled, fontname=" << q << "Arial" << q << "];" << endl;
        archivo << "titulo [label=" << q << "Matriz dispersa de capa " << idCapa << "\\nReporte dividido por partes para que Graphviz no se trabe\\nPixeles: " << totalPixeles << "   Filas: " << maxY << "   Columnas: " << maxX << q << ", fillcolor=" << q << "#FDE1E8" << q << ", color=" << q << "#C2185B" << q << "];" << endl;

        for (int i = 1; i <= totalPartes; i++) {
            int inicio = ((i - 1) * filasPorParte) + 1;
            int fin = inicio + filasPorParte - 1;
            if (fin > maxY) {
                fin = maxY;
            }
            archivo << "parte" << i << " [label=" << q << "Parte " << i << "\\nFilas " << inicio << " a " << fin << "\\nArchivo: matriz_capa_" << idCapa << "_parte_" << i << ".png" << q << ", fillcolor=" << q << "#FFF3E0" << q << ", color=" << q << "#EF6C00" << q << "];" << endl;
            archivo << "titulo -> parte" << i << ";" << endl;
        }

        archivo << "}" << endl;
        archivo.close();

        string comando = "dot -Tpng \"" + nombreDot + "\" -o \"" + nombrePng + "\"";
        system(comando.c_str());
        return true;
    }

    bool graficarEstructuraPorPartes(string nombreDot, string nombrePng, int idCapa) {
        int filasPorParte = 8;

        if (maxX > 80) {
            filasPorParte = 5;
        }

        if (maxX > 130) {
            filasPorParte = 3;
        }

        int totalPartes = maxY / filasPorParte;
        if (maxY % filasPorParte != 0) {
            totalPartes++;
        }

        cout << "La matriz es grande. Se generara por partes para que Graphviz no se quede trabado." << endl;
        cout << "Total de partes: " << totalPartes << endl;

        int generadas = 0;

        for (int parte = 1; parte <= totalPartes; parte++) {
            int inicioY = ((parte - 1) * filasPorParte) + 1;
            int finY = inicioY + filasPorParte - 1;

            if (finY > maxY) {
                finY = maxY;
            }

            string nombreParteDot = "matriz_capa_" + to_string(idCapa) + "_parte_" + to_string(parte) + ".dot";
            string nombrePartePng = "matriz_capa_" + to_string(idCapa) + "_parte_" + to_string(parte) + ".png";

            cout << "Generando parte " << parte << " de " << totalPartes << " (filas " << inicioY << " a " << finY << ")..." << endl;

            if (graficarEstructuraRango(nombreParteDot, nombrePartePng, idCapa, inicioY, finY, parte, totalPartes)) {
                generadas++;
            }
        }

        graficarIndicePartes(nombreDot, nombrePng, idCapa, totalPartes, filasPorParte);

        cout << "Partes generadas: " << generadas << endl;
        cout << "Abra matriz_capa_" << idCapa << ".png para ver el indice." << endl;
        cout << "Abra matriz_capa_" << idCapa << "_parte_1.png, parte_2, etc. para ver la matriz dispersa." << endl;

        return generadas > 0;
    }

    bool graficarEstructuraCompleta(string nombreDot, string nombrePng, int idCapa) {
        char q = 34;

        if (totalPixeles <= 0 || maxX <= 0 || maxY <= 0) {
            return false;
        }

        ofstream archivo(nombreDot.c_str());

        if (!archivo.is_open()) {
            return false;
        }

        archivo << "digraph G {" << endl;
        archivo << "graph [rankdir=TB, bgcolor=" << q << "white" << q << ", splines=false, nodesep=" << q << "0.28" << q << ", ranksep=" << q << "0.35" << q << "];" << endl;
        archivo << "node [shape=box, style=filled, fontname=" << q << "Arial" << q << ", fontsize=10, margin=" << q << "0.05" << q << "];" << endl;
        archivo << "edge [color=" << q << "#222222" << q << ", arrowsize=0.55, dir=both];" << endl;

        archivo << "titulo [label=" << q << "Matriz dispersa de capa " << idCapa << "\\nPixeles: " << totalPixeles << "   Filas: " << maxY << "   Columnas: " << maxX << q << ", fillcolor=" << q << "#FDE1E8" << q << ", color=" << q << "#C2185B" << q << ", fontsize=18];" << endl;
        archivo << "matriz [label=" << q << "matriz" << q << ", fillcolor=" << q << "#FDE1E8" << q << ", color=" << q << "#C2185B" << q << "];" << endl;
        archivo << "titulo -> matriz [style=invis, dir=none];" << endl;

        for (int x = 1; x <= maxX; x++) {
            NodoEncabezado* columna = columnas.buscar(x);
            if (columna != NULL) {
                archivo << "col" << x << " [label=" << q << "Columna " << x << q << ", fillcolor=" << q << "#DDF7F8" << q << ", color=" << q << "#008B95" << q << "];" << endl;
            }
        }

        for (int y = 1; y <= maxY; y++) {
            NodoEncabezado* fila = filas.buscar(y);
            if (fila != NULL) {
                archivo << "fila" << y << " [label=" << q << "Fila " << y << q << ", fillcolor=" << q << "#FFF3E0" << q << ", color=" << q << "#EF6C00" << q << "];" << endl;
            }
        }

        NodoEncabezado* fila = filas.obtenerPrimero();

        while (fila != NULL) {
            NodoPixel* pixel = fila->acceso;

            while (pixel != NULL) {
                archivo << "p" << pixel->x << "_" << pixel->y << " [label=" << q << pixel->color << q << ", fillcolor=" << q << "#FFFFFF" << q << ", fontcolor=" << q << "#000000" << q << ", color=" << q << "#444444" << q << ", width=0.75, height=0.28];" << endl;
                pixel = pixel->derecha;
            }

            fila = fila->siguiente;
        }

        archivo << "{ rank=same; matriz; ";
        for (int x = 1; x <= maxX; x++) {
            if (columnas.buscar(x) != NULL) {
                archivo << "col" << x << "; ";
            }
        }
        archivo << "}" << endl;

        for (int y = 1; y <= maxY; y++) {
            NodoEncabezado* filaActual = filas.buscar(y);
            if (filaActual != NULL) {
                archivo << "{ rank=same; fila" << y << "; ";
                NodoPixel* pixel = filaActual->acceso;
                while (pixel != NULL) {
                    archivo << "p" << pixel->x << "_" << pixel->y << "; ";
                    pixel = pixel->derecha;
                }
                archivo << "}" << endl;
            }
        }

        int anteriorCol = -1;
        for (int x = 1; x <= maxX; x++) {
            if (columnas.buscar(x) != NULL) {
                if (anteriorCol == -1) {
                    archivo << "matriz -> col" << x << " [color=" << q << "#008B95" << q << "];" << endl;
                } else {
                    archivo << "col" << anteriorCol << " -> col" << x << " [color=" << q << "#008B95" << q << "];" << endl;
                }
                anteriorCol = x;
            }
        }

        int anteriorFila = -1;
        for (int y = 1; y <= maxY; y++) {
            if (filas.buscar(y) != NULL) {
                if (anteriorFila == -1) {
                    archivo << "matriz -> fila" << y << " [color=" << q << "#EF6C00" << q << "];" << endl;
                } else {
                    archivo << "fila" << anteriorFila << " -> fila" << y << " [color=" << q << "#EF6C00" << q << "];" << endl;
                }
                anteriorFila = y;
            }
        }

        for (int y = 1; y <= maxY; y++) {
            NodoEncabezado* filaActual = filas.buscar(y);
            if (filaActual != NULL) {
                NodoPixel* pixel = filaActual->acceso;
                if (pixel != NULL) {
                    archivo << "fila" << y << " -> p" << pixel->x << "_" << pixel->y << " [color=" << q << "#444444" << q << "];" << endl;
                    while (pixel->derecha != NULL) {
                        archivo << "p" << pixel->x << "_" << pixel->y << " -> p" << pixel->derecha->x << "_" << pixel->derecha->y << " [color=" << q << "#444444" << q << "];" << endl;
                        pixel = pixel->derecha;
                    }
                }
            }
        }

        for (int x = 1; x <= maxX; x++) {
            NodoEncabezado* columnaActual = columnas.buscar(x);
            if (columnaActual != NULL) {
                NodoPixel* pixel = columnaActual->acceso;
                if (pixel != NULL) {
                    archivo << "col" << x << " -> p" << pixel->x << "_" << pixel->y << " [color=" << q << "#008B95" << q << "];" << endl;
                    while (pixel->abajo != NULL) {
                        archivo << "p" << pixel->x << "_" << pixel->y << " -> p" << pixel->abajo->x << "_" << pixel->abajo->y << " [color=" << q << "#008B95" << q << "];" << endl;
                        pixel = pixel->abajo;
                    }
                }
            }
        }

        archivo << "}" << endl;
        archivo.close();

        string comando = "dot -Tpng \"" + nombreDot + "\" -o \"" + nombrePng + "\"";
        int resultado = system(comando.c_str());

        if (resultado != 0) {
            string nombreSvg = "matriz_capa_" + to_string(idCapa) + ".svg";
            string comandoSvg = "dot -Tsvg \"" + nombreDot + "\" -o \"" + nombreSvg + "\"";
            system(comandoSvg.c_str());
        }

        return true;
    }

    // Genera la matriz dispersa completa en una sola imagen PNG.
    // Ya no divide por partes: crea un solo .dot y luego un solo .png.
    bool graficarEstructura(string nombreDot, string nombrePng, int idCapa) {
        if (totalPixeles <= 0 || maxX <= 0 || maxY <= 0) {
            return false;
        }

        char q = 34;

        cout << "Generando matriz dispersa completa en un solo archivo." << endl;
        cout << "Esto puede tardar, pero ya no se dividira por partes." << endl;
        cout << "Dimensiones: " << maxX << " columnas x " << maxY << " filas." << endl;
        cout << "Nodos aproximados: " << totalPixeles << endl;

        bool* columnasUsadas = new(nothrow) bool[maxX + 1];
        bool* filasUsadas = new(nothrow) bool[maxY + 1];

        if (columnasUsadas == NULL || filasUsadas == NULL) {
            if (columnasUsadas != NULL) {
                delete[] columnasUsadas;
            }
            if (filasUsadas != NULL) {
                delete[] filasUsadas;
            }
            cout << "No hay memoria suficiente para preparar la matriz completa." << endl;
            return false;
        }

        for (int x = 0; x <= maxX; x++) {
            columnasUsadas[x] = false;
        }

        for (int y = 0; y <= maxY; y++) {
            filasUsadas[y] = false;
        }

        NodoEncabezado* fila = filas.obtenerPrimero();

        while (fila != NULL) {
            if (fila->id >= 1 && fila->id <= maxY) {
                filasUsadas[fila->id] = true;
            }

            NodoPixel* pixel = fila->acceso;

            while (pixel != NULL) {
                if (pixel->x >= 1 && pixel->x <= maxX) {
                    columnasUsadas[pixel->x] = true;
                }
                pixel = pixel->derecha;
            }

            fila = fila->siguiente;
        }

        ofstream archivo(nombreDot.c_str());

        if (!archivo.is_open()) {
            delete[] columnasUsadas;
            delete[] filasUsadas;
            return false;
        }

        archivo << "digraph G {" << endl;
        archivo << "rankdir=LR;" << endl;
        archivo << "graph [bgcolor=" << q << "white" << q << ", splines=false, nodesep=0.18, ranksep=0.28, concentrate=false];" << endl;
        archivo << "node [shape=box, style=filled, fontname=" << q << "Arial" << q << ", fontsize=8, margin=" << q << "0.03" << q << ", fillcolor=" << q << "#FFFFFF" << q << ", color=" << q << "#444444" << q << "];" << endl;
        archivo << "edge [arrowsize=0.45, dir=both, color=" << q << "#555555" << q << "];" << endl;

        archivo << "titulo [label=" << q << "Matriz dispersa completa - Capa " << idCapa
                << "\\nPixeles: " << totalPixeles
                << "  Columnas: " << maxX
                << "  Filas: " << maxY << q
                << ", shape=plaintext, fillcolor=" << q << "#FFFFFF" << q
                << ", fontsize=12];" << endl;

        archivo << "matriz [label=" << q << "matriz" << q
                << ", fillcolor=" << q << "#FFFFFF" << q
                << ", color=" << q << "#444444" << q << "];" << endl;

        archivo << "titulo -> matriz [style=invis, dir=none];" << endl;

        for (int x = 1; x <= maxX; x++) {
            if (columnasUsadas[x] == true) {
                archivo << "col" << x << " [label=" << q << x << q
                        << ", fillcolor=" << q << "#FFFFFF" << q
                        << ", color=" << q << "#444444" << q << "];" << endl;
            }
        }

        for (int y = 1; y <= maxY; y++) {
            if (filasUsadas[y] == true) {
                archivo << "fila" << y << " [label=" << q << y << q
                        << ", fillcolor=" << q << "#FFFFFF" << q
                        << ", color=" << q << "#444444" << q << "];" << endl;
            }
        }

        int escritos = 0;

        for (int y = 1; y <= maxY; y++) {
            if (filasUsadas[y] == true) {
                for (int x = 1; x <= maxX; x++) {
                    if (columnasUsadas[x] == true) {
                        NodoPixel* pixel = buscarPixel(x, y);

                        if (pixel != NULL) {
                            archivo << "p" << x << "_" << y
                                    << " [label=" << q << pixel->color << q
                                    << ", fillcolor=" << q << "#FFFFFF" << q
                                    << ", fontcolor=" << q << "#000000" << q
                                    << ", color=" << q << "#444444" << q << "];" << endl;

                            escritos++;

                            if (escritos % 5000 == 0) {
                                cout << "Nodos de pixeles escritos: " << escritos << endl;
                            }
                        }
                    }
                }
            }
        }

        archivo << "{ rank=same; matriz; ";
        for (int y = 1; y <= maxY; y++) {
            if (filasUsadas[y] == true) {
                archivo << "fila" << y << "; ";
            }
        }
        archivo << "}" << endl;

        for (int x = 1; x <= maxX; x++) {
            if (columnasUsadas[x] == true) {
                archivo << "{ rank=same; col" << x << "; ";

                for (int y = 1; y <= maxY; y++) {
                    if (filasUsadas[y] == true && buscarPixel(x, y) != NULL) {
                        archivo << "p" << x << "_" << y << "; ";
                    }
                }

                archivo << "}" << endl;
            }
        }

        int anteriorFila = -1;
        for (int y = 1; y <= maxY; y++) {
            if (filasUsadas[y] == true) {
                if (anteriorFila == -1) {
                    archivo << "matriz -> fila" << y << ";" << endl;
                } else {
                    archivo << "fila" << anteriorFila << " -> fila" << y << ";" << endl;
                }
                anteriorFila = y;
            }
        }

        int anteriorCol = -1;
        for (int x = 1; x <= maxX; x++) {
            if (columnasUsadas[x] == true) {
                if (anteriorCol == -1) {
                    archivo << "matriz -> col" << x << ";" << endl;
                } else {
                    archivo << "col" << anteriorCol << " -> col" << x << ";" << endl;
                }
                anteriorCol = x;
            }
        }

        for (int y = 1; y <= maxY; y++) {
            if (filasUsadas[y] == true) {
                int anteriorX = -1;

                for (int x = 1; x <= maxX; x++) {
                    if (columnasUsadas[x] == true && buscarPixel(x, y) != NULL) {
                        if (anteriorX == -1) {
                            archivo << "fila" << y << " -> p" << x << "_" << y << ";" << endl;
                        } else {
                            archivo << "p" << anteriorX << "_" << y << " -> p" << x << "_" << y << ";" << endl;
                        }
                        anteriorX = x;
                    }
                }
            }
        }

        for (int x = 1; x <= maxX; x++) {
            if (columnasUsadas[x] == true) {
                int anteriorY = -1;

                for (int y = 1; y <= maxY; y++) {
                    if (filasUsadas[y] == true && buscarPixel(x, y) != NULL) {
                        if (anteriorY == -1) {
                            archivo << "col" << x << " -> p" << x << "_" << y << ";" << endl;
                        } else {
                            archivo << "p" << x << "_" << anteriorY << " -> p" << x << "_" << y << ";" << endl;
                        }
                        anteriorY = y;
                    }
                }
            }
        }

        archivo << "}" << endl;
        archivo.close();

        delete[] columnasUsadas;
        delete[] filasUsadas;

        cout << "Generando PNG completo. Espere un momento..." << endl;

        string comando = "dot -Tpng \"" + nombreDot + "\" -o \"" + nombrePng + "\"";
        int resultado = system(comando.c_str());

        if (resultado != 0) {
            cout << "Se creo el archivo .dot completo, pero Graphviz no genero el .png." << endl;
            cout << "Revise que Graphviz este instalado y agregado al PATH." << endl;
            return false;
        }

        cout << "Matriz dispersa completa generada en: " << nombrePng << endl;
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

struct NodoPilaCapa {
    NodoCapa* dato;
    NodoPilaCapa* siguiente;

    NodoPilaCapa(NodoCapa* pDato) {
        dato = pDato;
        siguiente = NULL;
    }
};

class PilaCapas {
private:
    NodoPilaCapa* cima;

public:
    PilaCapas() {
        cima = NULL;
    }

    ~PilaCapas() {
        while (cima != NULL) {
            NodoPilaCapa* borrar = cima;
            cima = cima->siguiente;
            delete borrar;
        }
    }

    void push(NodoCapa* dato) {
        NodoPilaCapa* nuevo = new(nothrow) NodoPilaCapa(dato);
        if (nuevo == NULL) {
            return;
        }
        nuevo->siguiente = cima;
        cima = nuevo;
    }

    NodoCapa* pop() {
        if (cima == NULL) {
            return NULL;
        }
        NodoPilaCapa* borrar = cima;
        NodoCapa* dato = borrar->dato;
        cima = cima->siguiente;
        delete borrar;
        return dato;
    }

    bool vacia() {
        return cima == NULL;
    }
};

class ArbolCapas {
private:
    NodoCapa* raiz;
    bool errorMemoria;

    void saltarEspacios(ifstream& archivo) {
        while (archivo.good() && isspace((unsigned char)archivo.peek())) {
            archivo.get();
        }
    }

    int leerNumeroDesde(ifstream& archivo, char primero) {
        int numero = 0;
        char c = primero;

        while (archivo.good() && isdigit((unsigned char)c)) {
            numero = numero * 10 + (c - '0');
            if (archivo.good() && isdigit((unsigned char)archivo.peek())) {
                archivo.get(c);
            } else {
                break;
            }
        }

        return numero;
    }

    bool leerNumeroSuelto(ifstream& archivo, int& numero) {
        saltarEspacios(archivo);
        if (!archivo.good()) {
            return false;
        }

        char c = archivo.peek();
        if (!isdigit((unsigned char)c)) {
            return false;
        }

        archivo.get(c);
        numero = leerNumeroDesde(archivo, c);
        return true;
    }

    void saltarHastaComaOPunto(ifstream& archivo) {
        while (archivo.good()) {
            char c = archivo.peek();
            if (c == ',' || c == ';' || c == '}') {
                return;
            }
            if (!isspace((unsigned char)c)) {
                return;
            }
            archivo.get();
        }
    }

    string leerColorArchivo(ifstream& archivo, bool& cerrarDespues) {
        string color = "";
        cerrarDespues = false;

        while (archivo.good()) {
            char c;
            archivo.get(c);

            if (c == ';') {
                break;
            }

            if (c == '}') {
                cerrarDespues = true;
                break;
            }

            if (!isspace((unsigned char)c) && c != ',') {
                color += c;
            }
        }

        return color;
    }

    bool leerInicioCapa(ifstream& archivo, int& idCapa, int& idAutomatico) {
        char c;

        while (archivo.get(c)) {
            if (isspace((unsigned char)c)) {
                continue;
            }

            if (isdigit((unsigned char)c)) {
                int posibleId = leerNumeroDesde(archivo, c);
                saltarEspacios(archivo);

                if (archivo.good() && archivo.peek() == '{') {
                    archivo.get();
                    idCapa = posibleId;
                    if (idCapa >= idAutomatico) {
                        idAutomatico = idCapa + 1;
                    }
                    return true;
                }
            }

            if (c == '{') {
                idCapa = idAutomatico;
                idAutomatico++;
                return true;
            }
        }

        return false;
    }

    bool leerPixelCapa(ifstream& archivo, int& x, int& y, string& color, bool& finCapa) {
        finCapa = false;
        char c;

        while (archivo.get(c)) {
            if (isspace((unsigned char)c) || c == ';') {
                continue;
            }

            if (c == '}') {
                finCapa = true;
                return false;
            }

            if (isdigit((unsigned char)c)) {
                x = leerNumeroDesde(archivo, c);

                saltarEspacios(archivo);
                if (archivo.good() && archivo.peek() == ',') {
                    archivo.get();
                }

                if (leerNumeroSuelto(archivo, y) == false) {
                    continue;
                }

                saltarEspacios(archivo);
                if (archivo.good() && archivo.peek() == ',') {
                    archivo.get();
                }

                bool cerrarDespues = false;
                color = leerColorArchivo(archivo, cerrarDespues);

                if (cerrarDespues) {
                    finCapa = true;
                }

                return true;
            }
        }

        finCapa = true;
        return false;
    }

    void mostrarInordenRecursivo(NodoCapa* actual) {
        if (actual != NULL) {
            mostrarInordenRecursivo(actual->izquierda);
            cout << "Capa " << actual->id << " - Pixeles: " << actual->matriz.contarPixeles() << endl;
            mostrarInordenRecursivo(actual->derecha);
        }
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
        if (actual == NULL || contador >= limite) {
            return;
        }

        lista[contador] = actual;
        contador++;
        recorrerPreordenLimitado(actual->izquierda, lista, contador, limite);
        recorrerPreordenLimitado(actual->derecha, lista, contador, limite);
    }

    void recorrerInordenLimitado(NodoCapa* actual, NodoCapa** lista, int& contador, int limite) {
        if (actual == NULL || contador >= limite) {
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
        if (actual == NULL || contador >= limite) {
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

    void destruirArbolIterativo() {
        if (raiz == NULL) {
            return;
        }

        PilaCapas pila;
        pila.push(raiz);

        while (pila.vacia() == false) {
            NodoCapa* actual = pila.pop();

            if (actual != NULL) {
                if (actual->izquierda != NULL) {
                    pila.push(actual->izquierda);
                }
                if (actual->derecha != NULL) {
                    pila.push(actual->derecha);
                }
                delete actual;
            }
        }

        raiz = NULL;
    }

    void escribirTablaImagen(ofstream& archivo, NodoCapa* capa, int ancho, int alto) {
        int maxCeldas = 40000;
        int pasoX = 1;
        int pasoY = 1;

        while (((ancho + pasoX - 1) / pasoX) * ((alto + pasoY - 1) / pasoY) > maxCeldas) {
            if ((ancho / pasoX) >= (alto / pasoY)) {
                pasoX++;
            } else {
                pasoY++;
            }
        }

        int anchoVista = (ancho + pasoX - 1) / pasoX;

        archivo << "<TR>" << endl;
        archivo << "<TD COLSPAN=\"" << anchoVista << "\" BGCOLOR=\"#FDE1E8\">";
        if (pasoX > 1 || pasoY > 1) {
            archivo << "Vista reducida para archivo grande";
        } else {
            archivo << "Capa completa";
        }
        archivo << "</TD>" << endl;
        archivo << "</TR>" << endl;

        for (int y = 1; y <= alto; y += pasoY) {
            archivo << "<TR>" << endl;

            for (int x = 1; x <= ancho; x += pasoX) {
                string color = capa->matriz.obtenerColor(x, y);
                archivo << "<TD WIDTH=\"8\" HEIGHT=\"8\" BGCOLOR=\"" << color << "\"></TD>" << endl;
            }

            archivo << "</TR>" << endl;
        }
    }

public:
    ArbolCapas() {
        raiz = NULL;
        errorMemoria = false;
    }

    ~ArbolCapas() {
        limpiar();
    }

    void limpiar() {
        destruirArbolIterativo();
        errorMemoria = false;
    }

    void insertar(int id) {
        if (raiz == NULL) {
            raiz = new(nothrow) NodoCapa(id);
            if (raiz == NULL) {
                errorMemoria = true;
            }
            return;
        }

        NodoCapa* actual = raiz;
        NodoCapa* padre = NULL;

        while (actual != NULL) {
            padre = actual;
            if (id == actual->id) {
                return;
            } else if (id < actual->id) {
                actual = actual->izquierda;
            } else {
                actual = actual->derecha;
            }
        }

        NodoCapa* nuevo = new(nothrow) NodoCapa(id);
        if (nuevo == NULL) {
            errorMemoria = true;
            return;
        }

        if (id < padre->id) {
            padre->izquierda = nuevo;
        } else {
            padre->derecha = nuevo;
        }
    }

    NodoCapa* buscar(int id) {
        NodoCapa* actual = raiz;

        while (actual != NULL) {
            if (actual->id == id) {
                return actual;
            }
            if (id < actual->id) {
                actual = actual->izquierda;
            } else {
                actual = actual->derecha;
            }
        }

        return NULL;
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

        limpiar();

        int idAutomatico = 1;
        int totalCapasArchivo = 0;
        int totalPixelesArchivo = 0;
        int pixelesInvalidos = 0;

        cout << "Cargando archivo de capas de forma segura..." << endl;
        cout << "Esto puede tardar si el archivo es muy grande." << endl;

        int idCapa = 0;

        while (leerInicioCapa(archivo, idCapa, idAutomatico)) {
            insertar(idCapa);
            NodoCapa* capaActual = buscar(idCapa);
            totalCapasArchivo++;

            if (totalCapasArchivo % 50 == 0) {
                cout << "Capas leidas: " << totalCapasArchivo << endl;
            }

            bool finCapa = false;

            while (finCapa == false && archivo.good()) {
                int x = 0;
                int y = 0;
                string color = "";
                bool leyoPixel = leerPixelCapa(archivo, x, y, color, finCapa);

                if (leyoPixel) {
                    if (capaActual != NULL) {
                        bool guardado = capaActual->matriz.insertar(x, y, color);
                        if (guardado) {
                            totalPixelesArchivo++;
                        } else {
                            pixelesInvalidos++;
                        }
                    }
                }

                if (totalPixelesArchivo > 0 && totalPixelesArchivo % 10000 == 0) {
                    cout << "Pixeles cargados: " << totalPixelesArchivo << endl;
                }
            }
        }

        archivo.close();

        cout << "Carga terminada." << endl;
        cout << "Total de capas leidas: " << totalCapasArchivo << endl;
        cout << "Total de pixeles guardados: " << totalPixelesArchivo << endl;

        if (pixelesInvalidos > 0) {
            cout << "Pixeles ignorados por formato invalido o memoria: " << pixelesInvalidos << endl;
        }

        if (errorMemoria) {
            cout << "Advertencia: hubo problemas de memoria al crear algunas capas." << endl;
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

        escribirTablaImagen(archivo, capa, ancho, alto);

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
        if (raiz == NULL || cantidad <= 0 || tipo < 1 || tipo > 3) {
            return false;
        }

        NodoCapa** lista = new(nothrow) NodoCapa*[cantidad];

        if (lista == NULL) {
            return false;
        }

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
        } else {
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

        if (ancho <= 0) ancho = 1;
        if (alto <= 0) alto = 1;

        int maxCeldas = 40000;
        int pasoX = 1;
        int pasoY = 1;

        while (((ancho + pasoX - 1) / pasoX) * ((alto + pasoY - 1) / pasoY) > maxCeldas) {
            if ((ancho / pasoX) >= (alto / pasoY)) {
                pasoX++;
            } else {
                pasoY++;
            }
        }

        int anchoVista = (ancho + pasoX - 1) / pasoX;

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
        archivo << "<TR><TD COLSPAN=\"" << anchoVista << "\" BGCOLOR=\"#FDE1E8\">Recorrido " << nombreRecorrido << "</TD></TR>" << endl;
        archivo << "<TR><TD COLSPAN=\"" << anchoVista << "\" BGCOLOR=\"#FFFFFF\">Capas: ";

        for (int i = 0; i < contador; i++) {
            archivo << lista[i]->id;
            if (i < contador - 1) {
                archivo << " - ";
            }
        }

        archivo << "</TD></TR>" << endl;

        for (int y = 1; y <= alto; y += pasoY) {
            archivo << "<TR>" << endl;
            for (int x = 1; x <= ancho; x += pasoX) {
                string color = obtenerColorFinalRecorrido(lista, contador, x, y);
                archivo << "<TD WIDTH=\"8\" HEIGHT=\"8\" BGCOLOR=\"" << color << "\"></TD>" << endl;
            }
            archivo << "</TR>" << endl;
        }

        archivo << "</TABLE>" << endl;
        archivo << ">];" << endl;
        archivo << "}" << endl;
        archivo.close();

        string comando = "dot -Tpng \"" + nombreDot + "\" -o \"" + nombrePng + "\"";
        system(comando.c_str());

        delete[] lista;
        return true;
    }

    bool arbolVacio() {
        return raiz == NULL;
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
        system(comando.c_str());
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
