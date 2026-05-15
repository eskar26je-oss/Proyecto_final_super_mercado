#pragma once
#include <iostream>
#include <string>
#include "CONEXION_BD.h"
#include <mysql.h>

using namespace std;

class Marca {

private:
    int id_marca;
    string marca;

public:

    Marca() {
        id_marca = 0;
        marca = "";
    }

    void setId_marca(int id) { id_marca = id; }
    void setMarca(string mar) { marca = mar; }

    void crear() {

        ConexionBD cn;
        cn.abrir_conexion();

        if (cn.get_conexion()) {

            string consulta = "INSERT INTO marcas(marca) VALUES('" + marca + "')";

            int q_estado = mysql_query(cn.get_conexion(), consulta.c_str());

            if (q_estado == 0)
                cout << "Marca agregada exitosamente." << endl;
            else
                cout << "Error: " << mysql_error(cn.get_conexion()) << endl;
        }

        cn.cerrar_conexion();
    }

    void leer() {

        ConexionBD cn;
        cn.abrir_conexion();

        if (cn.get_conexion()) {

            string consulta = "SELECT * FROM marcas";

            int q_estado = mysql_query(cn.get_conexion(), consulta.c_str());

            if (q_estado == 0) {

                MYSQL_RES* resultado = mysql_store_result(cn.get_conexion());
                MYSQL_ROW fila;

                while ((fila = mysql_fetch_row(resultado))) {

                    cout << "ID: " << fila[0]
                        << " | Marca: " << fila[1]
                        << endl;
                }

                mysql_free_result(resultado);
            }
            else {
                cout << "Error: " << mysql_error(cn.get_conexion()) << endl;
            }
        }

        cn.cerrar_conexion();
    }

    void actualizar() {

        ConexionBD cn;
        cn.abrir_conexion();

        if (cn.get_conexion()) {

            string consulta = "UPDATE marcas SET marca='" + marca +
                "' WHERE id_marca=" + to_string(id_marca);

            int q_estado = mysql_query(cn.get_conexion(), consulta.c_str());

            if (q_estado == 0)
                cout << "Marca actualizada exitosamente." << endl;
            else
                cout << "Error: " << mysql_error(cn.get_conexion()) << endl;
        }

        cn.cerrar_conexion();
    }

    void eliminar() {

        ConexionBD cn;
        cn.abrir_conexion();

        if (cn.get_conexion()) {

            string consulta = "DELETE FROM marcas WHERE id_marca=" + to_string(id_marca);

            int q_estado = mysql_query(cn.get_conexion(), consulta.c_str());

            if (q_estado == 0)
                cout << "Marca eliminada exitosamente." << endl;
            else
                cout << "Error: " << mysql_error(cn.get_conexion()) << endl;
        }

        cn.cerrar_conexion();
    }
};
