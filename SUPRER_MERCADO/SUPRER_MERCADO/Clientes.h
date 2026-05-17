#pragma once
#include <iostream>
#include <string>
#include <mysql.h>
#include "CONEXION_BD.h"

using namespace std;

class Cliente {

private:
    int id_clientes;
    string nombres, apellidos, nit, telefono, correo;
    bool genero;

public:

    // CONSTRUCTOR VACIO
    Cliente() {

        id_clientes = 0;
        nombres = "";
        apellidos = "";
        nit = "";
        telefono = "";
        correo = "";
        genero = false;
    }

    // CONSTRUCTOR CON PARAMETROS
    Cliente(
        int id,
        string nom,
        string ape,
        string n,
        bool gen,
        string tel,
        string corr,
        string fecha
    ) {

        id_clientes = id;
        nombres = nom;
        apellidos = ape;
        nit = n;
        genero = gen;
        telefono = tel;
        correo = corr;
    }

    bool validarDatos() {

        if (nombres.empty()) {
            cout << "Error\n";
            return false;
        }

        return true;
    }

    // ================= CREAR =================

    void crear() {

        if (!validarDatos()) return;

        ConexionBD cn;
        cn.abrir_conexion();

        if (cn.get_conexion() == NULL) {
            cout << "Error de conexion\n";
            return;
        }

        string consulta =
            "INSERT INTO clientes(nombres,apellidos,nit,genero,telefono,correo_electronico) VALUES('" +
            nombres + "','" +
            apellidos + "','" +
            nit + "'," +
            to_string(genero) + ",'" +
            telefono + "','" +
            correo + "')";

        int q_estado =
            mysql_query(cn.get_conexion(), consulta.c_str());

        if (!q_estado) {
            cout << "Cliente agregado\n";
        }
        else {
            cout << "Error: "
                << mysql_error(cn.get_conexion())
                << endl;
        }

        cn.cerrar_conexion();
    }

    // ================= LEER =================

    void leer() {

        ConexionBD cn;
        cn.abrir_conexion();

        if (cn.get_conexion() == NULL) {
            cout << "Error de conexion\n";
            return;
        }

        MYSQL_ROW fila;
        MYSQL_RES* resultado;

        string consulta =
            "SELECT * FROM clientes";

        int q_estado =
            mysql_query(cn.get_conexion(), consulta.c_str());

        if (!q_estado) {

            resultado =
                mysql_store_result(cn.get_conexion());

            while ((fila = mysql_fetch_row(resultado))) {

                cout << fila[0] << " | "
                    << fila[1] << " | "
                    << fila[2] << " | "
                    << fila[3] << endl;
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

    // ================= ACTUALIZAR =================

    void actualizar() {

        ConexionBD cn;
        cn.abrir_conexion();

        if (cn.get_conexion() == NULL) {
            cout << "Error de conexion\n";
            return;
        }

        string consulta =
            "UPDATE clientes SET nombres='" + nombres +
            "', apellidos='" + apellidos +
            "', nit='" + nit +
            "', genero=" + to_string(genero) +
            ", telefono='" + telefono +
            "', correo_electronico='" + correo +
            "' WHERE id_clientes=" +
            to_string(id_clientes);

        int q_estado =
            mysql_query(cn.get_conexion(), consulta.c_str());

        if (!q_estado) {
            cout << "Cliente actualizado\n";
        }
        else {
            cout << "Error: "
                << mysql_error(cn.get_conexion())
                << endl;
        }

        cn.cerrar_conexion();
    }

    // ================= ELIMINAR =================

    void eliminar() {

        ConexionBD cn;
        cn.abrir_conexion();

        if (cn.get_conexion() == NULL) {
            cout << "Error de conexion\n";
            return;
        }

        string consulta =
            "DELETE FROM clientes WHERE id_clientes=" +
            to_string(id_clientes);

        int q_estado =
            mysql_query(cn.get_conexion(), consulta.c_str());

        if (!q_estado) {
            cout << "Cliente eliminado\n";
        }
        else {
            cout << "Error: "
                << mysql_error(cn.get_conexion())
                << endl;
        }

        cn.cerrar_conexion();
    }
};