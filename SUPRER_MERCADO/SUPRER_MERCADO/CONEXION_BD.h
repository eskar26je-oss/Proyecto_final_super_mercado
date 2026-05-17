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

        conectar = mysql_init(NULL);

        if (conectar == NULL) {
            cout << "Error en mysql_init()" << endl;
            return;
        }

        MYSQL* conexion_temporal = mysql_real_connect(
            conectar,
            "localhost",
            "root",
            "MySQL2026#",
            "super_mercado_profin",
            3306,
            NULL,
            0
        );

        if (conexion_temporal == NULL) {

            cout << "Error al conectar a la base de datos" << endl;
            cout << mysql_error(conectar) << endl;

            conectar = NULL;
        }
        else {

            conectar = conexion_temporal;
            cout << "Conexion exitosa" << endl;
        }
    }

    MYSQL* get_conexion() {
        return conectar;
    }

    void cerrar_conexion() {

        if (conectar != NULL) {
            mysql_close(conectar);
            conectar = NULL;
        }
    }
};