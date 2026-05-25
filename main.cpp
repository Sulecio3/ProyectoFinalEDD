#include <iostream>
#include <string>

using namespace std;

int main()
{
    int opcion = -1;
    int opcionGenerar = -1;
    int opcionCrud = -1;
    int opcionReportes = -1;

    while (opcion != 0)
    {
        cout << "=====================================" << endl;
        cout << " GENERADOR DE IMAGENES POR CAPAS" << endl;
        cout << "=====================================" << endl;
        cout << "1. Carga masiva de capas" << endl;
        cout << "2. Carga masiva de imagenes" << endl;
        cout << "3. Carga masiva de usuarios" << endl;
        cout << "4. Generacion de imagenes" << endl;
        cout << "5. CRUD" << endl;
        cout << "6. Estado de la memoria" << endl;
        cout << "0. Salir" << endl;
        cout << "Ingrese una opcion: ";
        cin >> opcion;

        if (opcion == 1)
        {
            cout << endl;
            cout << "Carga masiva de capas pendiente." << endl;
            cout << endl;
        }
        else if (opcion == 2)
        {
            cout << endl;
            cout << "Carga masiva de imagenes pendiente." << endl;
            cout << endl;
        }
        else if (opcion == 3)
        {
            cout << endl;
            cout << "Carga masiva de usuarios pendiente." << endl;
            cout << endl;
        }
        else if (opcion == 4)
        {
            opcionGenerar = -1;

            while (opcionGenerar != 0)
            {
                cout << "=====================================" << endl;
                cout << " GENERACION DE IMAGENES" << endl;
                cout << "=====================================" << endl;
                cout << "1. Por recorrido limitado" << endl;
                cout << "2. Por lista de imagenes" << endl;
                cout << "3. Por capa" << endl;
                cout << "4. Por usuario" << endl;
                cout << "0. Regresar" << endl;
                cout << "Ingrese una opcion: ";
                cin >> opcionGenerar;

                if (opcionGenerar == 1)
                {
                    cout << endl;
                    cout << "Generar por recorrido limitado pendiente." << endl;
                    cout << endl;
                }
                else if (opcionGenerar == 2)
                {
                    cout << endl;
                    cout << "Generar por lista de imagenes pendiente." << endl;
                    cout << endl;
                }
                else if (opcionGenerar == 3)
                {
                    cout << endl;
                    cout << "Generar por capa pendiente." << endl;
                    cout << endl;
                }
                else if (opcionGenerar == 4)
                {
                    cout << endl;
                    cout << "Generar por usuario pendiente." << endl;
                    cout << endl;
                }
                else if (opcionGenerar == 0)
                {
                    cout << endl;
                    cout << "Regresando al menu principal..." << endl;
                    cout << endl;
                }
                else
                {
                    cout << endl;
                    cout << "Opcion no valida :( intente de nuevo" << endl;
                    cout << endl;
                }
            }
        }
        else if (opcion == 5)
        {
            opcionCrud = -1;

            while (opcionCrud != 0)
            {
                cout << "=====================================" << endl;
                cout << " CRUD" << endl;
                cout << "=====================================" << endl;
                cout << "1. Usuarios" << endl;
                cout << "2. Imagenes" << endl;
                cout << "0. Regresar" << endl;
                cout << "Ingrese una opcion: ";
                cin >> opcionCrud;

                if (opcionCrud == 1)
                {
                    cout << endl;
                    cout << "CRUD de usuarios pendiente." << endl;
                    cout << endl;
                }
                else if (opcionCrud == 2)
                {
                    cout << endl;
                    cout << "CRUD de imagenes pendiente." << endl;
                    cout << endl;
                }
                else if (opcionCrud == 0)
                {
                    cout << endl;
                    cout << "Regresando al menu principal..." << endl;
                    cout << endl;
                }
                else
                {
                    cout << endl;
                    cout << "Opcion no valida :( intente de nuevo" << endl;
                    cout << endl;
                }
            }
        }
        else if (opcion == 6)
        {
            opcionReportes = -1;

            while (opcionReportes != 0)
            {
                cout << "=====================================" << endl;
                cout << " ESTADO DE LA MEMORIA" << endl;
                cout << "=====================================" << endl;
                cout << "1. Ver lista de imagenes" << endl;
                cout << "2. Ver arbol de capas" << endl;
                cout << "3. Ver capa" << endl;
                cout << "4. Ver imagen y arbol de capas" << endl;
                cout << "5. Ver arbol de usuarios" << endl;
                cout << "0. Regresar" << endl;
                cout << "Ingrese una opcion: ";
                cin >> opcionReportes;

                if (opcionReportes == 1)
                {
                    cout << endl;
                    cout << "Reporte de lista de imagenes pendiente." << endl;
                    cout << endl;
                }
                else if (opcionReportes == 2)
                {
                    cout << endl;
                    cout << "Reporte de arbol de capas pendiente." << endl;
                    cout << endl;
                }
                else if (opcionReportes == 3)
                {
                    cout << endl;
                    cout << "Reporte de capa pendiente." << endl;
                    cout << endl;
                }
                else if (opcionReportes == 4)
                {
                    cout << endl;
                    cout << "Reporte de imagen y arbol pendiente." << endl;
                    cout << endl;
                }
                else if (opcionReportes == 5)
                {
                    cout << endl;
                    cout << "Reporte de arbol de usuarios pendiente." << endl;
                    cout << endl;
                }
                else if (opcionReportes == 0)
                {
                    cout << endl;
                    cout << "Opcion no valida :( intente de nuevo" << endl;
                    cout << endl;
                }
                else
                {
                    cout << endl;
                    cout << "Opcion invalida." << endl;
                    cout << endl;
                }
            }
        }
        else if (opcion == 0)
        {
            cout << endl;
            cout << "Saliendo del programa" << endl;
            cout << endl;
        }
        else
        {
            cout << endl;
            cout << "Opcion no valida :( intente de nuevo" << endl;
            cout << endl;
        }
    }

    return 0;
}
