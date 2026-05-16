#pragma once
#include <mysql.h>
#include <iostream>

using namespace std;

class ConexionBD {
private:
    MYSQL* conectar;

public:
    ConexionBD() {
        conectar = NULL;
    }

    void abrir_conexion() {
        conectar = mysql_init(0);

        if (conectar == NULL) {
            cout << "Error en mysql_init()" << endl;
            return;
        }

        conectar = mysql_real_connect(
            conectar,
            "localhost",
            "root",
            "MySQL2026#",
            "super_mercado",
            3306,
            NULL,
            0
        );

        if (conectar == NULL) {
            cout << "Error al conectar a la base de datos" << endl;
            cout << mysql_error(conectar) << endl;
        }
        else {
            cout << "Conexion exitosa" << endl;
        }
    }

    MYSQL* get_conexion() {
        return conectar;
    }

    void cerrar_conexion() {
        if (conectar != NULL) {
            mysql_close(conectar);
        }
    }
};


