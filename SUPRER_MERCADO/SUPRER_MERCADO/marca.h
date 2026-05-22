#pragma once
#include <iostream>
#include <string>

#include <mysql.h>

#include "CONEXION_BD.h"
#include "validaciones.h"

using namespace std;

class Marca {

private:

    int id_marca;
    string marca;

public:

    // ================= CONSTRUCTOR =================
    Marca() {

        id_marca = 0;
        marca = "";
    }

    // ================= SETTERS =================
    void setId_marca(int id) {

        if (validar_id(id)) {

            id_marca = id;
        }
        else {

            cout << "ID invalido.\n";
        }
    }

    void setMarca(string mar) {

        if (validar_texto_general(mar, 50)) {

            marca = mar;
        }
        else {

            cout << "Marca invalida.\n";
        }
    }

    // ================= CREATE =================
    void crear() {

        if (!validar_texto_general(marca, 50)) {

            cout << "Marca invalida.\n";
            return;
        }

        ConexionBD cn;
        cn.abrir_conexion();

        if (!cn.get_conexion()) {

            cout << "Error de conexion.\n";
            return;
        }

        string consulta =
            "INSERT INTO marcas(marca) "
            "VALUES('"
            + marca +
            "')";

        int q_estado =
            mysql_query(
                cn.get_conexion(),
                consulta.c_str()
            );

        if (q_estado == 0) {

            cout << "Marca agregada exitosamente.\n";
        }
        else {

            cout << "Error: "
                << mysql_error(cn.get_conexion())
                << endl;
        }

        cn.cerrar_conexion();
    }

    // ================= READ =================
    void leer() {

        ConexionBD cn;
        cn.abrir_conexion();

        if (!cn.get_conexion()) {

            cout << "Error de conexion.\n";
            return;
        }

        string consulta =
            "SELECT * FROM marcas";

        int q_estado =
            mysql_query(
                cn.get_conexion(),
                consulta.c_str()
            );

        if (q_estado == 0) {

            MYSQL_RES* resultado =
                mysql_store_result(
                    cn.get_conexion()
                );

            MYSQL_ROW fila;

            while (
                (fila = mysql_fetch_row(resultado))
                ) {

                cout
                    << "ID: " << fila[0]
                    << " | Marca: "
                    << fila[1]
                    << endl;
            }

            mysql_free_result(resultado);
        }
        else {

            cout << "Error: "
                << mysql_error(cn.get_conexion())
                << endl;
        }

        cn.cerrar_conexion();
    }

    // ================= UPDATE =================
    void actualizar() {

        if (!validar_id(id_marca)) {

            cout << "ID invalido.\n";
            return;
        }

        if (!validar_texto_general(marca, 50)) {

            cout << "Marca invalida.\n";
            return;
        }

        ConexionBD cn;
        cn.abrir_conexion();

        if (!cn.get_conexion()) {

            cout << "Error de conexion.\n";
            return;
        }

        string consulta =
            "UPDATE marcas SET marca='"
            + marca +
            "' WHERE id_marca="
            + to_string(id_marca);

        int q_estado =
            mysql_query(
                cn.get_conexion(),
                consulta.c_str()
            );

        if (q_estado == 0) {

            cout << "Marca actualizada exitosamente.\n";
        }
        else {

            cout << "Error: "
                << mysql_error(cn.get_conexion())
                << endl;
        }

        cn.cerrar_conexion();
    }

    // ================= DELETE =================
    void eliminar() {

        if (!validar_id(id_marca)) {

            cout << "ID invalido.\n";
            return;
        }

        ConexionBD cn;
        cn.abrir_conexion();

        if (!cn.get_conexion()) {

            cout << "Error de conexion.\n";
            return;
        }

        string consulta =
            "DELETE FROM marcas "
            "WHERE id_marca="
            + to_string(id_marca);

        int q_estado =
            mysql_query(
                cn.get_conexion(),
                consulta.c_str()
            );

        if (q_estado == 0) {

            cout << "Marca eliminada exitosamente.\n";
        }
        else {

            cout << "Error: "
                << mysql_error(cn.get_conexion())
                << endl;
        }

        cn.cerrar_conexion();
    }
};