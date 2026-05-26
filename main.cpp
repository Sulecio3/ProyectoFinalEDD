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
        cout << "     GENERADOR DE IMAGENES POR CAPAS  " << endl;
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
        cout << "12. Generar imagen por usuario" << endl;
        cout << "13. Graficar arbol de capas" << endl;
        cout << "14. Graficar lista circular de imagenes" << endl;
        cout << "15. Graficar arbol de usuarios" << endl;
        cout << "16. Graficar imagen y arbol de capas" << endl;
        cout << "17. Agregar usuario" << endl;
        cout << "18. Modificar usuario" << endl;
        cout << "19. Eliminar usuario" << endl;
        cout << "20. Agregar imagen" << endl;
        cout << "21. Eliminar imagen" << endl;
        cout << "22. Generar imagen por recorrido limitado" << endl;
        cout << "23. Graficar capa como matriz dispersa" << endl;
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
        } else if (opcion == 12) {
            string nombre;
            int idImagen;

            cout << "Ingrese el nombre del usuario: ";
            cin >> nombre;
            cout << "Ingrese el id de la imagen del usuario: ";
            cin >> idImagen;

            if (arbolUsuarios.generarImagenDeUsuario(nombre, idImagen)) {
                cout << "Imagen generada correctamente." << endl;
                cout << "Revise los archivos usuario_" << nombre << "_imagen_" << idImagen << ".dot y usuario_" << nombre << "_imagen_" << idImagen << ".png" << endl;
            } else {
                cout << "No se pudo generar la imagen." << endl;
                cout << "Revise que el usuario exista y que tenga esa imagen asignada." << endl;
            }
        } else if (opcion == 13) {
            if (arbolCapas.graficarArbolCapas()) {
                cout << "Arbol de capas generado correctamente." << endl;
                cout << "Revise los archivos arbol_capas.dot y arbol_capas.png" << endl;
            } else {
                cout << "No se pudo generar el arbol de capas." << endl;
                cout << "Revise que primero haya cargado las capas." << endl;
            }
        } else if (opcion == 14) {
            if (listaImagenes.graficarListaImagenes()) {
                cout << "Lista de imagenes generada correctamente." << endl;
                cout << "Revise los archivos lista_imagenes.dot y lista_imagenes.png" << endl;
            } else {
                cout << "No se pudo generar la lista de imagenes." << endl;
                cout << "Revise que primero haya cargado las imagenes." << endl;
            }
        } else if (opcion == 15) {
            if (arbolUsuarios.graficarArbolUsuarios()) {
                cout << "Arbol de usuarios generado correctamente." << endl;
                cout << "Revise los archivos arbol_usuarios.dot y arbol_usuarios.png" << endl;
            } else {
                cout << "No se pudo generar el arbol de usuarios." << endl;
                cout << "Revise que primero haya cargado los usuarios." << endl;
            }
        } else if (opcion == 16) {
            int id;
            cout << "Ingrese el id de la imagen: ";
            cin >> id;

            if (listaImagenes.graficarImagenYArbol(id, arbolCapas)) {
                cout << "Reporte generado correctamente." << endl;
                cout << "Revise los archivos imagen_arbol_" << id << ".dot y imagen_arbol_" << id << ".png" << endl;
            } else {
                cout << "No se pudo generar el reporte." << endl;
                cout << "Revise que primero haya cargado capas e imagenes." << endl;
            }
        } else if (opcion == 17) {
            string nombre;
            cout << "Ingrese el nombre del nuevo usuario: ";
            cin >> nombre;

            if (arbolUsuarios.agregarUsuarioManual(nombre)) {
                cout << "Usuario agregado correctamente." << endl;
            } else {
                cout << "No se pudo agregar el usuario." << endl;
                cout << "Revise que no exista o que el nombre no este vacio." << endl;
            }
        } else if (opcion == 18) {
            string nombreActual;
            string nombreNuevo;

            cout << "Ingrese el nombre actual del usuario: ";
            cin >> nombreActual;
            cout << "Ingrese el nuevo nombre del usuario: ";
            cin >> nombreNuevo;

            if (arbolUsuarios.modificarUsuario(nombreActual, nombreNuevo)) {
                cout << "Usuario modificado correctamente." << endl;
            } else {
                cout << "No se pudo modificar el usuario." << endl;
                cout << "Revise que el usuario exista y que el nuevo nombre no este repetido." << endl;
            }
        } else if (opcion == 19) {
            string nombre;
            char confirmar;

            cout << "Ingrese el nombre del usuario a eliminar: ";
            cin >> nombre;
            cout << "Seguro que desea eliminarlo? s/n: ";
            cin >> confirmar;

            if (confirmar == 's' || confirmar == 'S') {
                if (arbolUsuarios.eliminarUsuario(nombre)) {
                    cout << "Usuario eliminado correctamente." << endl;
                } else {
                    cout << "No se pudo eliminar el usuario." << endl;
                    cout << "Revise que el usuario exista." << endl;
                }
            } else {
                cout << "Eliminacion cancelada." << endl;
            }
        } else if (opcion == 20) {
            string nombreUsuario;
            int idImagen;
            string capasTexto;

            cout << "Ingrese el usuario al que se le agregara la imagen: ";
            cin >> nombreUsuario;
            cout << "Ingrese el id de la nueva imagen: ";
            cin >> idImagen;
            cout << "Ingrese las capas separadas por coma. Ejemplo 1,2,3: ";
            cin >> capasTexto;

            if (listaImagenes.agregarImagenManual(idImagen, capasTexto, arbolCapas)) {
                NodoImagen* imagenNueva = listaImagenes.buscar(idImagen);

                if (arbolUsuarios.agregarImagenAUsuario(nombreUsuario, idImagen, imagenNueva)) {
                    cout << "Imagen agregada correctamente." << endl;
                } else {
                    listaImagenes.eliminarImagen(idImagen);
                    cout << "No se pudo agregar la imagen al usuario." << endl;
                    cout << "Revise que el usuario exista y que no tenga ya esa imagen." << endl;
                }
            } else {
                cout << "No se pudo agregar la imagen." << endl;
                cout << "Revise que el id no exista y que las capas esten cargadas." << endl;
            }
        } else if (opcion == 21) {
            string nombreUsuario;
            int idImagen;
            char confirmar;

            cout << "Ingrese el usuario dueño de la imagen: ";
            cin >> nombreUsuario;
            cout << "Ingrese el id de la imagen a eliminar: ";
            cin >> idImagen;
            cout << "Seguro que desea eliminar la imagen? s/n: ";
            cin >> confirmar;

            if (confirmar == 's' || confirmar == 'S') {
                if (arbolUsuarios.eliminarImagenDeUsuario(nombreUsuario, idImagen)) {
                    arbolUsuarios.eliminarImagenDeTodos(idImagen);

                    if (listaImagenes.eliminarImagen(idImagen)) {
                        cout << "Imagen eliminada correctamente." << endl;
                    } else {
                        cout << "La imagen se quito del usuario, pero no estaba en la lista circular." << endl;
                    }
                } else {
                    cout << "No se pudo eliminar la imagen." << endl;
                    cout << "Revise que el usuario exista y que tenga esa imagen." << endl;
                }
            } else {
                cout << "Eliminacion cancelada." << endl;
            }
        } else if (opcion == 22) {
            int cantidad;
            int tipo;
            string nombreRecorrido;

            cout << "Ingrese la cantidad de capas a utilizar: ";
            cin >> cantidad;
            cout << "Seleccione el recorrido:" << endl;
            cout << "1. Preorden" << endl;
            cout << "2. Inorden" << endl;
            cout << "3. Postorden" << endl;
            cout << "Opcion: ";
            cin >> tipo;

            if (tipo == 1) {
                nombreRecorrido = "preorden";
            } else if (tipo == 2) {
                nombreRecorrido = "inorden";
            } else if (tipo == 3) {
                nombreRecorrido = "postorden";
            } else {
                nombreRecorrido = "recorrido";
            }

            if (arbolCapas.generarImagenRecorridoLimitado(cantidad, tipo)) {
                cout << "Imagen generada correctamente." << endl;
                cout << "Revise los archivos recorrido_" << nombreRecorrido << "_" << cantidad << ".dot y recorrido_" << nombreRecorrido << "_" << cantidad << ".png" << endl;
            } else {
                cout << "No se pudo generar la imagen." << endl;
                cout << "Revise que haya cargado las capas y que el recorrido sea valido." << endl;
            }
        } else if (opcion == 23) {
            int id;
            cout << "Ingrese el id de la capa a graficar como matriz dispersa: ";
            cin >> id;

            if (arbolCapas.graficarMatrizCapa(id)) {
                cout << "Matriz dispersa generada correctamente." << endl;
                cout << "Revise los archivos matriz_capa_" << id << ".dot y matriz_capa_" << id << ".png" << endl;
            } else {
                cout << "No se pudo generar la matriz dispersa." << endl;
                cout << "Revise que la capa exista y que primero haya cargado las capas." << endl;
            }
        } else if (opcion == 0) {
            cout << "Saliendo del programa" << endl;
        } else {
            cout << "Opcion no valida, intente de nuevo." << endl;
        }
    }

    return 0;
}
