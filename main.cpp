#include <iostream>
#include <string>
#include <limits>
#include "capas.cpp"
#include "imagenes.cpp"
#include "usuarios.cpp"

using namespace std;

int leerEntero(string mensaje) {
    int numero;
    bool correcto = false;

    while (correcto == false) {
        cout << mensaje;
        cin >> numero;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Debe ingresar un numero valido." << endl;
        } else {
            correcto = true;
        }
    }

    return numero;
}

string leerTexto(string mensaje) {
    string texto;
    cout << mensaje;
    cin >> texto;
    return texto;
}

char leerCaracter(string mensaje) {
    char letra;
    cout << mensaje;
    cin >> letra;
    return letra;
}

void mostrarMenu() {
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
}

int main() {
    ArbolCapas arbolCapas;
    ListaImagenes listaImagenes;
    ArbolUsuarios arbolUsuarios;
    int opcion = -1;
    string ruta;
    bool capasCargadas = false;
    bool imagenesCargadas = false;
    bool usuariosCargados = false;

    while (opcion != 0) {
        mostrarMenu();
        opcion = leerEntero("Seleccione una opcion: ");

        if (opcion == 1) {
            ruta = leerTexto("Ingrese la ruta del archivo .cap: ");

            if (arbolCapas.cargarArchivo(ruta)) {
                capasCargadas = true;
                cout << "Capas cargadas correctamente." << endl;
            } else {
                cout << "No se pudo cargar el archivo." << endl;
            }
        } else if (opcion == 2) {
            if (capasCargadas == false) {
                cout << "Primero debe cargar las capas." << endl;
            } else {
                cout << endl;
                cout << "Capas en inorden:" << endl;
                arbolCapas.mostrarInorden();
            }
        } else if (opcion == 3) {
            if (capasCargadas == false) {
                cout << "Primero debe cargar las capas." << endl;
            } else {
                int id = leerEntero("Ingrese el id de la capa: ");
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
            }
        } else if (opcion == 4) {
            if (capasCargadas == false) {
                cout << "Primero debe cargar las capas para que las imagenes apunten al ABB." << endl;
            } else {
                ruta = leerTexto("Ingrese la ruta del archivo .im: ");

                if (listaImagenes.cargarArchivo(ruta, arbolCapas)) {
                    imagenesCargadas = true;
                    cout << "Imagenes cargadas correctamente." << endl;
                } else {
                    cout << "No se pudo cargar el archivo." << endl;
                }
            }
        } else if (opcion == 5) {
            if (imagenesCargadas == false) {
                cout << "Primero debe cargar las imagenes." << endl;
            } else {
                cout << endl;
                cout << "Lista circular doble de imagenes:" << endl;
                listaImagenes.mostrarImagenes();
            }
        } else if (opcion == 6) {
            if (imagenesCargadas == false) {
                cout << "Primero debe cargar las imagenes." << endl;
            } else {
                int id = leerEntero("Ingrese el id de la imagen: ");
                listaImagenes.mostrarDetalleImagen(id);
            }
        } else if (opcion == 7) {
            if (imagenesCargadas == false) {
                cout << "Primero debe cargar las imagenes para que los usuarios apunten a la lista circular." << endl;
            } else {
                ruta = leerTexto("Ingrese la ruta del archivo .usr: ");

                if (arbolUsuarios.cargarArchivo(ruta, listaImagenes)) {
                    usuariosCargados = true;
                    cout << "Usuarios cargados correctamente." << endl;
                } else {
                    cout << "No se pudo cargar el archivo." << endl;
                }
            }
        } else if (opcion == 8) {
            if (usuariosCargados == false) {
                cout << "Primero debe cargar los usuarios." << endl;
            } else {
                cout << endl;
                cout << "Usuarios en inorden:" << endl;
                arbolUsuarios.mostrarUsuarios();
            }
        } else if (opcion == 9) {
            if (usuariosCargados == false) {
                cout << "Primero debe cargar los usuarios." << endl;
            } else {
                string nombre = leerTexto("Ingrese el nombre del usuario: ");
                arbolUsuarios.mostrarDetalleUsuario(nombre);
            }
        } else if (opcion == 10) {
            if (capasCargadas == false) {
                cout << "Primero debe cargar las capas." << endl;
            } else {
                int id = leerEntero("Ingrese el id de la capa a generar: ");

                if (arbolCapas.generarImagenCapa(id)) {
                    cout << "Imagen generada correctamente." << endl;
                    cout << "Revise los archivos capa_" << id << ".dot y capa_" << id << ".png" << endl;
                } else {
                    cout << "No se pudo generar la imagen." << endl;
                }
            }
        } else if (opcion == 11) {
            if (imagenesCargadas == false) {
                cout << "Primero debe cargar las imagenes." << endl;
            } else {
                int id = leerEntero("Ingrese el id de la imagen a generar: ");

                if (listaImagenes.generarImagenPorId(id)) {
                    cout << "Imagen generada correctamente." << endl;
                    cout << "Revise los archivos imagen_" << id << ".dot y imagen_" << id << ".png" << endl;
                } else {
                    cout << "No se pudo generar la imagen." << endl;
                }
            }
        } else if (opcion == 12) {
            if (usuariosCargados == false) {
                cout << "Primero debe cargar los usuarios." << endl;
            } else {
                string nombre = leerTexto("Ingrese el nombre del usuario: ");
                int idImagen = leerEntero("Ingrese el id de la imagen del usuario: ");

                if (arbolUsuarios.generarImagenDeUsuario(nombre, idImagen)) {
                    cout << "Imagen generada correctamente." << endl;
                    cout << "Revise los archivos usuario_" << nombre << "_imagen_" << idImagen << ".dot y usuario_" << nombre << "_imagen_" << idImagen << ".png" << endl;
                } else {
                    cout << "No se pudo generar la imagen." << endl;
                    cout << "Revise que el usuario exista y que tenga esa imagen asignada." << endl;
                }
            }
        } else if (opcion == 13) {
            if (capasCargadas == false) {
                cout << "Primero debe cargar las capas." << endl;
            } else if (arbolCapas.graficarArbolCapas()) {
                cout << "Arbol de capas generado correctamente." << endl;
                cout << "Revise los archivos arbol_capas.dot y arbol_capas.png" << endl;
            } else {
                cout << "No se pudo generar el arbol de capas." << endl;
            }
        } else if (opcion == 14) {
            if (imagenesCargadas == false) {
                cout << "Primero debe cargar las imagenes." << endl;
            } else if (listaImagenes.graficarListaImagenes()) {
                cout << "Lista de imagenes generada correctamente." << endl;
                cout << "Revise los archivos lista_imagenes.dot y lista_imagenes.png" << endl;
            } else {
                cout << "No se pudo generar la lista de imagenes." << endl;
            }
        } else if (opcion == 15) {
            if (usuariosCargados == false) {
                cout << "Primero debe cargar los usuarios." << endl;
            } else if (arbolUsuarios.graficarArbolUsuarios()) {
                cout << "Arbol de usuarios generado correctamente." << endl;
                cout << "Revise los archivos arbol_usuarios.dot y arbol_usuarios.png" << endl;
            } else {
                cout << "No se pudo generar el arbol de usuarios." << endl;
            }
        } else if (opcion == 16) {
            if (capasCargadas == false || imagenesCargadas == false) {
                cout << "Primero debe cargar capas e imagenes." << endl;
            } else {
                int id = leerEntero("Ingrese el id de la imagen: ");

                if (listaImagenes.graficarImagenYArbol(id, arbolCapas)) {
                    cout << "Reporte generado correctamente." << endl;
                    cout << "Revise los archivos imagen_arbol_" << id << ".dot y imagen_arbol_" << id << ".png" << endl;
                } else {
                    cout << "No se pudo generar el reporte." << endl;
                }
            }
        } else if (opcion == 17) {
            string nombre = leerTexto("Ingrese el nombre del nuevo usuario: ");

            if (arbolUsuarios.agregarUsuarioManual(nombre)) {
                usuariosCargados = true;
                cout << "Usuario agregado correctamente." << endl;
            } else {
                cout << "No se pudo agregar el usuario." << endl;
                cout << "Revise que no exista o que el nombre no este vacio." << endl;
            }
        } else if (opcion == 18) {
            string nombreActual = leerTexto("Ingrese el nombre actual del usuario: ");
            string nombreNuevo = leerTexto("Ingrese el nuevo nombre del usuario: ");

            if (arbolUsuarios.modificarUsuario(nombreActual, nombreNuevo)) {
                cout << "Usuario modificado correctamente." << endl;
            } else {
                cout << "No se pudo modificar el usuario." << endl;
                cout << "Revise que el usuario exista y que el nuevo nombre no este repetido." << endl;
            }
        } else if (opcion == 19) {
            string nombre = leerTexto("Ingrese el nombre del usuario a eliminar: ");
            char confirmar = leerCaracter("Seguro que desea eliminarlo? s/n: ");

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
            if (capasCargadas == false) {
                cout << "Primero debe cargar las capas." << endl;
            } else {
                string nombreUsuario = leerTexto("Ingrese el usuario al que se le agregara la imagen: ");
                int idImagen = leerEntero("Ingrese el id de la nueva imagen: ");
                string capasTexto = leerTexto("Ingrese las capas separadas por coma. Ejemplo 1,2,3: ");

                if (listaImagenes.agregarImagenManual(idImagen, capasTexto, arbolCapas)) {
                    NodoImagen* imagenNueva = listaImagenes.buscar(idImagen);

                    if (arbolUsuarios.agregarImagenAUsuario(nombreUsuario, idImagen, imagenNueva)) {
                        imagenesCargadas = true;
                        usuariosCargados = true;
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
            }
        } else if (opcion == 21) {
            string nombreUsuario = leerTexto("Ingrese el usuario dueño de la imagen: ");
            int idImagen = leerEntero("Ingrese el id de la imagen a eliminar: ");
            char confirmar = leerCaracter("Seguro que desea eliminar la imagen? s/n: ");

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
            if (capasCargadas == false) {
                cout << "Primero debe cargar las capas." << endl;
            } else {
                int cantidad = leerEntero("Ingrese la cantidad de capas a utilizar: ");
                int tipo;
                string nombreRecorrido;

                cout << "Seleccione el recorrido:" << endl;
                cout << "1. Preorden" << endl;
                cout << "2. Inorden" << endl;
                cout << "3. Postorden" << endl;
                tipo = leerEntero("Opcion: ");

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
            }
        } else if (opcion == 23) {
            if (capasCargadas == false) {
                cout << "Primero debe cargar las capas." << endl;
            } else {
                int id = leerEntero("Ingrese el id de la capa a graficar como matriz dispersa: ");

                if (arbolCapas.graficarMatrizCapa(id)) {
                    cout << "Matriz dispersa generada correctamente." << endl;
                    cout << "Revise los archivos matriz_capa_" << id << ".dot y matriz_capa_" << id << ".png" << endl;
                } else {
                    cout << "No se pudo generar la matriz dispersa." << endl;
                    cout << "Revise que la capa exista y que primero haya cargado las capas." << endl;
                }
            }
        } else if (opcion == 0) {
            cout << "Saliendo del programa..." << endl;
        } else {
            cout << "Opcion invalida." << endl;
        }
    }

    return 0;
}
