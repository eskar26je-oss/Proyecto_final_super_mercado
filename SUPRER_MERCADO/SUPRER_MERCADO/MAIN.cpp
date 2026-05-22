#define NOMINMAX

#include <iostream>
#include <limits>
#include <string>
#include <mysql.h>

#include "puestos.h"
#include "empleados.h"
#include "marca.h"
#include "productos.h"
#include "proveedores.h"
#include "clientes.h"
#include "validaciones.h"

using namespace std;

// ================= LIMPIAR BUFFER =================

void limpiarBuffer() {

    cin.clear();

    cin.ignore(
        (numeric_limits<streamsize>::max)(),
        '\n'
    );
}

// ================= LEER TEXTO =================

string leerTexto(string mensaje) {

    string valor;

    cout << mensaje;

    getline(cin, valor);

    return valor;
}

// ================= LEER ENTERO =================

int leerEntero(string mensaje) {

    int valor;

    cout << mensaje;

    while (!(cin >> valor)) {

        cout << "Ingrese un numero valido: ";

        limpiarBuffer();
    }

    limpiarBuffer();

    return valor;
}

// ================= LEER DECIMAL =================

double leerDecimal(string mensaje) {

    double valor;

    cout << mensaje;

    while (!(cin >> valor)) {

        cout << "Ingrese un decimal valido: ";

        limpiarBuffer();
    }

    limpiarBuffer();

    return valor;
}

// ================= CONEXION MYSQL =================

void conectar_bd(MYSQL*& conectar) {

    conectar = mysql_init(NULL);

    if (!conectar) {

        cout << "Error al inicializar MYSQL.\n";

        exit(1);
    }

    conectar = mysql_real_connect(
        conectar,
        "localhost",
        "root",
        "root08",
        "super_mercado",
        3306,
        NULL,
        0
    );

    if (conectar) {

        cout << "Conexion exitosa.\n";
    }
    else {

        cout << "\n========== ERROR MYSQL ==========\n";

        cout << mysql_error(conectar) << endl;

        cout << "=================================\n";

        exit(1);
    }
}

// ================= MENU PUESTOS =================

void menuPuestos(MYSQL* conectar) {

    int opcion;

    do {

        cout << "\n===== PUESTOS =====\n";

        cout << "1. Crear\n";
        cout << "2. Ver\n";
        cout << "3. Actualizar\n";
        cout << "4. Eliminar\n";
        cout << "0. Regresar\n";

        opcion = leerEntero("Opcion: ");

        switch (opcion) {

        case 1:
            crear_puestos(conectar);
            break;

        case 2:
            mostrar_puestos(conectar);
            break;

        case 3:
            actualizar_puestos(conectar);
            break;

        case 4:
            eliminar_puestos(conectar);
            break;
        }

    } while (opcion != 0);
}

// ================= MENU EMPLEADOS =================

void menuEmpleados(MYSQL* conectar) {

    int opcion;

    do {

        cout << "\n===== EMPLEADOS =====\n";

        cout << "1. Crear\n";
        cout << "2. Ver\n";
        cout << "3. Actualizar\n";
        cout << "4. Eliminar\n";
        cout << "0. Regresar\n";

        opcion = leerEntero("Opcion: ");

        switch (opcion) {

        case 1:
            crear_empleados(conectar);
            break;

        case 2:
            mostrar_empleados(conectar);
            break;

        case 3:
            actualizar_empleados(conectar);
            break;

        case 4:
            eliminar_empleados(conectar);
            break;
        }

    } while (opcion != 0);
}

// ================= MENU MARCAS =================

void menuMarcas() {

    int opcion;

    Marca m;

    do {

        cout << "\n===== MARCAS =====\n";

        cout << "1. Crear\n";
        cout << "2. Ver\n";
        cout << "3. Actualizar\n";
        cout << "4. Eliminar\n";
        cout << "0. Regresar\n";

        opcion = leerEntero("Opcion: ");

        switch (opcion) {

        case 1: {

            string mar;

            do {

                mar = leerTexto("Marca: ");

                if (!validar_texto_general(mar, 50)) {

                    cout << "Marca invalida.\n";
                }

            } while (!validar_texto_general(mar, 50));

            m.setMarca(mar);

            m.crear();

            break;
        }

        case 2:

            m.leer();

            break;

        case 3: {

            int id;

            string mar;

            do {

                id = leerEntero("ID: ");

            } while (!validar_id(id));

            do {

                mar = leerTexto("Nuevo nombre: ");

            } while (!validar_texto_general(mar, 50));

            m.setId_marca(id);

            m.setMarca(mar);

            m.actualizar();

            break;
        }

        case 4: {

            int id;

            do {

                id = leerEntero("ID: ");

            } while (!validar_id(id));

            m.setId_marca(id);

            m.eliminar();

            break;
        }
        }

    } while (opcion != 0);
}

// ================= MAIN =================

int main() {

    MYSQL* conectar;

    conectar_bd(conectar);

    int opcion;

    do {

        cout << "\n===== SISTEMA SUPERMERCADO =====\n";

        cout << "1. Puestos\n";
        cout << "2. Empleados\n";
        cout << "3. Marcas\n";
        cout << "4. Productos\n";
        cout << "5. Proveedores\n";
        cout << "6. Clientes\n";
        cout << "0. Salir\n";

        opcion = leerEntero("Opcion: ");

        switch (opcion) {

        case 1:
            menuPuestos(conectar);
            break;

        case 2:
            menuEmpleados(conectar);
            break;

        case 3:
            menuMarcas();
            break;
        }

    } while (opcion != 0);

    mysql_close(conectar);

    return 0;
}