#pragma once
#include <iostream>
#include <string>
#include <mysql.h>
#include "CONEXION_BD.h"

using namespace std;

class Proveedor {

private:
    int id_proveedor;
    string proveedor, nit, direccion, telefono;

public:

    // CONSTRUCTOR VACIO
    Proveedor() {
        id_proveedor = 0;
        proveedor = "";
        nit = "";
        direccion = "";
        telefono = "";
    }

    // CONSTRUCTOR CON PARAMETROS
    Proveedor(
        int id,
        string prov,
        string n,
        string dir,
        string tel
    ) {
        id_proveedor = id;
        proveedor = prov;
        nit = n;
        direccion = dir;
        telefono = tel;
    }

    bool validarDatos() {

        if (proveedor.empty() || nit.empty()) {
            cout << "Error: proveedor o nit vacio\n";
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
            "INSERT INTO proveedores(proveedor,nit,direccion,telefono) VALUES('" +
            proveedor + "','" +
            nit + "','" +
            direccion + "','" +
            telefono + "')";

        int q_estado = mysql_query(cn.get_conexion(), consulta.c_str());

        if (!q_estado) {
            cout << "Proveedor agregado\n";
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
            "SELECT id_proveedor, proveedor, nit, direccion, telefono FROM proveedores";

        int q_estado =
            mysql_query(cn.get_conexion(), consulta.c_str());

        if (!q_estado) {

            resultado =
                mysql_store_result(cn.get_conexion());

            if (resultado == NULL) {
                cout << "No hay datos\n";
                return;
            }

            while ((fila = mysql_fetch_row(resultado))) {

                cout << fila[0] << " | "
                    << fila[1] << " | "
                    << fila[2] << " | ";

                if (fila[3] != NULL)
                    cout << fila[3];
                else
                    cout << "NULL";

                cout << " | ";

                if (fila[4] != NULL)
                    cout << fila[4];
                else
                    cout << "NULL";

                cout << endl;
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
            "UPDATE proveedores SET proveedor='" + proveedor +
            "', nit='" + nit +
            "', direccion='" + direccion +
            "', telefono='" + telefono +
            "' WHERE id_proveedor=" + to_string(id_proveedor);

        int q_estado =
            mysql_query(cn.get_conexion(), consulta.c_str());

        if (!q_estado) {
            cout << "Proveedor actualizado\n";
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
            "DELETE FROM proveedores WHERE id_proveedor=" +
            to_string(id_proveedor);

        int q_estado =
            mysql_query(cn.get_conexion(), consulta.c_str());

        if (!q_estado) {
            cout << "Proveedor eliminado\n";
        }
        else {
            cout << "Error: "
                << mysql_error(cn.get_conexion())
                << endl;
        }

        cn.cerrar_conexion();
    }
};