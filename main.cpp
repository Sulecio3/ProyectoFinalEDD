#include <iostream>
#include <cstdlib>
using namespace std;

void cargarCapas();
void mostrarCapas();
void buscarCapaMenu();

void pausar() {
    cout << "\nPresione ENTER para continuar...";
    cin.ignore();
    cin.get();
}

void limpiar() {
    system("cls");
}

int main() {
    int opcion = 0;

    do {
        limpiar();
        cout << "====================================\n";
        cout << "   GENERADOR DE IMAGENES POR CAPAS\n";
        cout << "====================================\n";
        cout << "1. Carga masiva de capas\n";
        cout << "2. Ver capas cargadas\n";
        cout << "3. Buscar y ver una capa\n";
        cout << "4. Carga masiva de imagenes\n";
        cout << "5. Carga masiva de usuarios\n";
        cout << "6. Generar imagen\n";
        cout << "7. Reportes\n";
        cout << "8. CRUD\n";
        cout << "9. Salir\n";
        cout << "====================================\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        if (opcion == 1) {
            cin.ignore();
            cargarCapas();
            pausar();
        } else if (opcion == 2) {
            cin.ignore();
            mostrarCapas();
            pausar();
        } else if (opcion == 3) {
            cin.ignore();
            buscarCapaMenu();
            pausar();
        } else if (opcion == 4) {
            cin.ignore();
            cout << "\nEsta opcion se trabajara en el siguiente commit.\n";
            pausar();
        } else if (opcion == 5) {
            cin.ignore();
            cout << "\nEsta opcion se trabajara mas adelante.\n";
            pausar();
        } else if (opcion == 6) {
            cin.ignore();
            cout << "\nEsta opcion se trabajara mas adelante.\n";
            pausar();
        } else if (opcion == 7) {
            cin.ignore();
            cout << "\nEsta opcion se trabajara mas adelante.\n";
            pausar();
        } else if (opcion == 8) {
            cin.ignore();
            cout << "\nEsta opcion se trabajara mas adelante.\n";
            pausar();
        } else if (opcion == 9) {
            cout << "\nSaliendo del programa...\n";
        } else {
            cin.ignore();
            cout << "\nOpcion no valida :( intente de nuevo\n";
            pausar();
        }

    } while (opcion != 9);

    return 0;
}
