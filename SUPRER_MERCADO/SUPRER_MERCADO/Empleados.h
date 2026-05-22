#pragma once
#include <mysql.h>
#include <iostream>
#include <string>
#include <limits>

#include "validaciones.h"

using namespace std;

void mostrar_puestos(MYSQL* conectar);

// ================= STRUCT =================
struct Empleados {

    int id_empleados;
    char nombres[60];
    char apellidos[60];
    char direccion[80];
    char telefono[9];
    char cui[14];
    int genero;
    char fecha_nacimiento[11];
    int id_puesto;
    char fecha_inicio_de_labores[11];
    char fecha_ingreso[11];
};

// ================= FUNCIONES AUXILIARES =================
inline void limpiar_buffer() {

    cin.clear();
    cin.ignore((numeric_limits<streamsize>::max)(), '\n');
}

inline int leer_entero(string mensaje) {

    string texto;
    int numero;

    while (true) {

        cout << mensaje;
        getline(cin, texto);

        if (!solo_numeros(texto)) {

            cout << "Ingrese solo numeros.\n";
            continue;
        }

        numero = stoi(texto);

        return numero;
    }
}

// ================= CREATE =================
inline void crear_empleados(MYSQL* conectar) {

    Empleados empleados;
    string entrada;
    char respuesta;

    do {

        // ================= NOMBRES =================
        do {

            cout << "Ingrese nombres: ";
            getline(cin, entrada);

            if (!validar_nombre(entrada, 60)) {

                cout << "Nombre invalido. Solo letras y espacios.\n";
            }

        } while (!validar_nombre(entrada, 60));

        strcpy_s(
            empleados.nombres,
            sizeof(empleados.nombres),
            entrada.c_str()
        );

        // ================= APELLIDOS =================
        do {

            cout << "Ingrese apellidos: ";
            getline(cin, entrada);

            if (!validar_nombre(entrada, 60)) {

                cout << "Apellidos invalidos.\n";
            }

        } while (!validar_nombre(entrada, 60));

        strcpy_s(
            empleados.apellidos,
            sizeof(empleados.apellidos),
            entrada.c_str()
        );

        // ================= DIRECCION =================
        do {

            cout << "Ingrese direccion: ";
            getline(cin, entrada);

            if (!validar_direccion(entrada, 80)) {

                cout << "Direccion invalida.\n";
            }

        } while (!validar_direccion(entrada, 80));

        strcpy_s(
            empleados.direccion,
            sizeof(empleados.direccion),
            entrada.c_str()
        );

        // ================= TELEFONO =================
        do {

            cout << "Ingrese telefono (8 digitos): ";
            getline(cin, entrada);

            if (!validar_telefono_gt(entrada)) {

                cout << "Telefono invalido.\n";
            }

        } while (!validar_telefono_gt(entrada));

        strcpy_s(
            empleados.telefono,
            sizeof(empleados.telefono),
            entrada.c_str()
        );

        // ================= CUI =================
        do {

            cout << "Ingrese CUI (13 digitos): ";
            getline(cin, entrada);

            if (!validar_cui(entrada)) {

                cout << "CUI invalido.\n";
            }

        } while (!validar_cui(entrada));

        strcpy_s(
            empleados.cui,
            sizeof(empleados.cui),
            entrada.c_str()
        );

        // ================= GENERO =================
        do {

            empleados.genero =
                leer_entero(
                    "Genero (0=Femenino, 1=Masculino): "
                );

            if (!validar_genero(empleados.genero)) {

                cout << "Genero invalido.\n";
            }

        } while (!validar_genero(empleados.genero));

        // ================= FECHA NACIMIENTO =================
        do {

            cout << "Fecha nacimiento (YYYY-MM-DD): ";
            getline(cin, entrada);

            if (!validar_fecha(entrada)) {

                cout << "Fecha invalida.\n";
            }

        } while (!validar_fecha(entrada));

        strcpy_s(
            empleados.fecha_nacimiento,
            sizeof(empleados.fecha_nacimiento),
            entrada.c_str()
        );

        // ================= MOSTRAR PUESTOS =================
        mostrar_puestos(conectar);

        // ================= ID PUESTO =================
        do {

            empleados.id_puesto =
                leer_entero("ID puesto: ");

            if (!validar_id(empleados.id_puesto)) {

                cout << "ID invalido.\n";
            }

        } while (!validar_id(empleados.id_puesto));

        // ================= FECHA INICIO =================
        do {

            cout << "Fecha inicio labores (YYYY-MM-DD): ";
            getline(cin, entrada);

            if (!validar_fecha(entrada)) {

                cout << "Fecha invalida.\n";
            }

        } while (!validar_fecha(entrada));

        strcpy_s(
            empleados.fecha_inicio_de_labores,
            sizeof(empleados.fecha_inicio_de_labores),
            entrada.c_str()
        );

        // ================= FECHA INGRESO =================
        do {

            cout << "Fecha ingreso (YYYY-MM-DD): ";
            getline(cin, entrada);

            if (!validar_fecha(entrada)) {

                cout << "Fecha invalida.\n";
            }

        } while (!validar_fecha(entrada));

        strcpy_s(
            empleados.fecha_ingreso,
            sizeof(empleados.fecha_ingreso),
            entrada.c_str()
        );

        // ================= QUERY =================
        string consulta =
            "INSERT INTO empleados("
            "nombres,"
            "apellidos,"
            "direccion,"
            "telefono,"
            "cui,"
            "genero,"
            "fecha_nacimiento,"
            "id_puesto,"
            "fecha_inicio_de_labores,"
            "fecha_ingreso"
            ") VALUES('";

        consulta += string(empleados.nombres) + "','";
        consulta += string(empleados.apellidos) + "','";
        consulta += string(empleados.direccion) + "','";
        consulta += string(empleados.telefono) + "','";
        consulta += string(empleados.cui) + "',";
        consulta += to_string(empleados.genero) + ",'";
        consulta += string(empleados.fecha_nacimiento) + "',";
        consulta += to_string(empleados.id_puesto) + ",'";
        consulta += string(empleados.fecha_inicio_de_labores) + "','";
        consulta += string(empleados.fecha_ingreso) + "')";

        int estado =
            mysql_query(
                conectar,
                consulta.c_str()
            );

        if (estado == 0) {

            cout << "Empleado agregado correctamente.\n";
        }
        else {

            cout << "Error: "
                << mysql_error(conectar)
                << endl;
        }

        cout << "Desea agregar otro? (s/n): ";

        cin >> respuesta;
        limpiar_buffer();

    } while (
        respuesta == 's' ||
        respuesta == 'S'
        );
}

// ================= READ =================
inline void mostrar_empleados(MYSQL* conectar) {

    MYSQL_ROW fila;
    MYSQL_RES* resultado;

    string consulta =
        "SELECT e.id_empleados, "
        "e.nombres, "
        "e.apellidos, "
        "p.puesto "
        "FROM empleados e "
        "INNER JOIN puestos p "
        "ON e.id_puesto = p.id_puestos";

    mysql_query(
        conectar,
        consulta.c_str()
    );

    resultado =
        mysql_store_result(conectar);

    while (
        (fila = mysql_fetch_row(resultado))
        ) {

        cout
            << "ID: " << fila[0]
            << " | Nombre: "
            << fila[1]
            << " "
            << fila[2]
            << " | Puesto: "
            << fila[3]
            << endl;
    }

    mysql_free_result(resultado);
}

// ================= UPDATE =================
inline void actualizar_empleados(MYSQL* conectar) {

    Empleados e;
    string entrada;

    do {

        e.id_empleados =
            leer_entero(
                "ID empleado: "
            );

        if (!validar_id(e.id_empleados)) {

            cout << "ID invalido.\n";
        }

    } while (!validar_id(e.id_empleados));

    do {

        cout << "Nuevo nombre: ";
        getline(cin, entrada);

        if (!validar_nombre(entrada, 60)) {

            cout << "Nombre invalido.\n";
        }

    } while (!validar_nombre(entrada, 60));

    strcpy_s(
        e.nombres,
        sizeof(e.nombres),
        entrada.c_str()
    );

    string consulta =
        "UPDATE empleados SET nombres='"
        + string(e.nombres)
        + "' WHERE id_empleados="
        + to_string(e.id_empleados);

    int estado =
        mysql_query(
            conectar,
            consulta.c_str()
        );

    if (estado == 0) {

        cout << "Empleado actualizado correctamente.\n";
    }
    else {

        cout << "Error: "
            << mysql_error(conectar)
            << endl;
    }
}

// ================= DELETE =================
inline void eliminar_empleados(MYSQL* conectar) {

    int id;

    do {

        id =
            leer_entero(
                "ID eliminar: "
            );

        if (!validar_id(id)) {

            cout << "ID invalido.\n";
        }

    } while (!validar_id(id));

    string consulta =
        "DELETE FROM empleados "
        "WHERE id_empleados="
        + to_string(id);

    int estado =
        mysql_query(
            conectar,
            consulta.c_str()
        );

    if (estado == 0) {

        cout << "Empleado eliminado correctamente.\n";
    }
    else {

        cout << "Error: "
            << mysql_error(conectar)
            << endl;
    }
}

// ================= MENU =================
inline void menu_empleados(MYSQL* conectar) {

    int opcion;

    do {

        opcion =
            leer_entero(
                "\n===== EMPLEADOS =====\n"
                "1. Crear\n"
                "2. Mostrar\n"
                "3. Actualizar\n"
                "4. Eliminar\n"
                "5. Salir\n"
                "Opcion: "
            );

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

        case 5:
            cout << "Saliendo...\n";
            break;

        default:
            cout << "Opcion invalida.\n";
        }

    } while (opcion != 5);
}