#include <iostream>
#include <string>
#include "capas.cpp"
#include "imagenes.cpp"
#include "usuarios.cpp"

using namespace std;

int main() {
    ArbolCapas arbolCapas;
    ListaImagenes listaImagenes;
    ArbolUsuarios arbolUsuarios;
    int opcion = -1;
    string ruta;

    while (opcion != 0) {
        cout << endl;
        cout << "======================================" << endl;
        cout << "     GENERADOR DE IMAGENES POR CAPAS  " << endl;
        cout << "======================================" << endl;
        cout << "1. Cargar capas" << endl;
        cout << "2. Ver capas cargadas" << endl;
        cout << "3. Buscar una capa" << endl;
        cout << "4. Cargar imagenes" << endl;
        cout << "5. Ver lista de imagenes" << endl;
        cout << "6. Ver detalle de una imagen" << endl;
        cout << "7. Cargar usuarios" << endl;
        cout << "8. Ver usuarios cargados" << endl;
        cout << "9. Ver detalle de un usuario" << endl;
        cout << "10. Generar imagen por capa" << endl;
        cout << "11. Generar imagen por lista de imagenes" << endl;
        cout << "0. Salir" << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        if (opcion == 1) {
            cout << "Ingrese la ruta del archivo .cap: ";
            cin >> ruta;

            if (arbolCapas.cargarArchivo(ruta)) {
                cout << "Capas cargadas correctamente." << endl;
            } else {
                cout << "No se pudo cargar el archivo." << endl;
            }
        } else if (opcion == 2) {
            cout << endl;
            cout << "Capas en inorden:" << endl;
            arbolCapas.mostrarInorden();
        } else if (opcion == 3) {
            int id;
            cout << "Ingrese el id de la capa: ";
            cin >> id;

            NodoCapa* capa = arbolCapas.buscar(id);

            if (capa != NULL) {
                cout << "Capa encontrada: " << capa->id << endl;
                cout << "Pixeles guardados: " << capa->matriz.contarPixeles() << endl;
                cout << "Ancho maximo: " << capa->matriz.obtenerMaxX() << endl;
                cout << "Alto maximo: " << capa->matriz.obtenerMaxY() << endl;
                capa->matriz.mostrarPixeles();
            } else {
                cout << "No existe esa capa." << endl;
            }
        } else if (opcion == 4) {
            cout << "Primero debe cargar las capas para que las imagenes apunten al ABB." << endl;
            cout << "Ingrese la ruta del archivo .im: ";
            cin >> ruta;

            if (listaImagenes.cargarArchivo(ruta, arbolCapas)) {
                cout << "Imagenes cargadas correctamente." << endl;
            } else {
                cout << "No se pudo cargar el archivo." << endl;
            }
        } else if (opcion == 5) {
            cout << endl;
            cout << "Lista circular doble de imagenes:" << endl;
            listaImagenes.mostrarImagenes();
        } else if (opcion == 6) {
            int id;
            cout << "Ingrese el id de la imagen: ";
            cin >> id;
            listaImagenes.mostrarDetalleImagen(id);
        } else if (opcion == 7) {
            cout << "Primero debe cargar las imagenes para que los usuarios apunten a la lista circular." << endl;
            cout << "Ingrese la ruta del archivo .usr: ";
            cin >> ruta;

            if (arbolUsuarios.cargarArchivo(ruta, listaImagenes)) {
                cout << "Usuarios cargados correctamente." << endl;
            } else {
                cout << "No se pudo cargar el archivo." << endl;
            }
        } else if (opcion == 8) {
            cout << endl;
            cout << "Usuarios en inorden:" << endl;
            arbolUsuarios.mostrarUsuarios();
        } else if (opcion == 9) {
            string nombre;
            cout << "Ingrese el nombre del usuario: ";
            cin >> nombre;
            arbolUsuarios.mostrarDetalleUsuario(nombre);
        } else if (opcion == 10) {
            int id;
            cout << "Ingrese el id de la capa a generar: ";
            cin >> id;

            if (arbolCapas.generarImagenCapa(id)) {
                cout << "Imagen generada correctamente." << endl;
                cout << "Revise los archivos capa_" << id << ".dot y capa_" << id << ".png" << endl;
            } else {
                cout << "No se pudo generar la imagen." << endl;
            }
        } else if (opcion == 11) {
            int id;
            cout << "Ingrese el id de la imagen a generar: ";
            cin >> id;

            if (listaImagenes.generarImagenPorId(id)) {
                cout << "Imagen generada correctamente." << endl;
                cout << "Revise los archivos imagen_" << id << ".dot y imagen_" << id << ".png" << endl;
            } else {
                cout << "No se pudo generar la imagen." << endl;
            }
        } else if (opcion == 0) {
            cout << "Saliendo del programa..." << endl;
        } else {
            cout << "Opcion invalida." << endl;
        }
    }

    return 0;
}
