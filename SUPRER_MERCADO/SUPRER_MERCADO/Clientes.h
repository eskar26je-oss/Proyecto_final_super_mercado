#pragma once
#include <iostream>
#include <string>
#include <mysql.h>
#include <limits>

#include "CONEXION_BD.h"
#include "validaciones.h"

using namespace std;

class Cliente {

private:
    int id_clientes;
    string nombres, apellidos, nit, telefono, correo;
    int genero;

public:

    // ================= CONSTRUCTOR VACIO =================
    Cliente() {

        id_clientes = 0;
        nombres = "";
        apellidos = "";
        nit = "";
        telefono = "";
        correo = "";
        genero = 0;
    }

    // ================= CONSTRUCTOR CON PARAMETROS =================
    Cliente(
        int id,
        string nom,
        string ape,
        string n,
        int gen,
        string tel,
        string corr
    ) {

        id_clientes = id;
        nombres = nom;
        apellidos = ape;
        nit = n;
        genero = gen;
        telefono = tel;
        correo = corr;
    }

    // ================= LIMPIAR BUFFER =================
    void limpiar_buffer() {

        cin.clear();
        cin.ignore((numeric_limits<streamsize>::max)(), '\n');
    }

    // ================= LEER ENTERO =================
    int leer_entero(string mensaje) {

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

    // ================= PEDIR DATOS =================
    void pedir_datos() {

        // ================= NOMBRES =================
        do {

            cout << "Nombres: ";
            getline(cin, nombres);

            if (!validar_nombre(nombres, 60)) {

                cout << "Nombre invalido. Solo letras y espacios.\n";
            }

        } while (!validar_nombre(nombres, 60));

        // ================= APELLIDOS =================
        do {

            cout << "Apellidos: ";
            getline(cin, apellidos);

            if (!validar_nombre(apellidos, 60)) {

                cout << "Apellidos invalidos. Solo letras y espacios.\n";
            }

        } while (!validar_nombre(apellidos, 60));

        // ================= NIT =================
        do {

            cout << "NIT: ";
            getline(cin, nit);

            if (!validar_nit(nit) || !validar_longitud_max(nit, 12)) {

                cout << "NIT invalido.\n";
            }

        } while (!validar_nit(nit) || !validar_longitud_max(nit, 12));

        // ================= GENERO =================
        do {

            genero = leer_entero("Genero (0=Femenino, 1=Masculino): ");

            if (!validar_genero(genero)) {

                cout << "Genero invalido. Ingrese 0 o 1.\n";
            }

        } while (!validar_genero(genero));

        // ================= TELEFONO =================
        do {

            cout << "Telefono (8 digitos): ";
            getline(cin, telefono);

            if (!validar_telefono_gt(telefono)) {

                cout << "Telefono invalido.\n";
            }

        } while (!validar_telefono_gt(telefono));

        // ================= CORREO =================
        do {

            cout << "Correo electronico: ";
            getline(cin, correo);

            if (
                !validar_correo(correo) ||
                !validar_longitud_max(correo, 45)
                ) {

                cout << "Correo invalido.\n";
            }

        } while (
            !validar_correo(correo) ||
            !validar_longitud_max(correo, 45)
            );
    }

    // ================= CREAR =================
    void crear() {

        pedir_datos();

        ConexionBD cn;
        cn.abrir_conexion();

        if (cn.get_conexion() == NULL) {

            cout << "Error de conexion\n";
            return;
        }

        string consulta =
            "INSERT INTO clientes("
            "nombres,"
            "apellidos,"
            "nit,"
            "genero,"
            "telefono,"
            "correo_electronico"
            ") VALUES('"
            + nombres + "','"
            + apellidos + "','"
            + nit + "',"
            + to_string(genero) + ",'"
            + telefono + "','"
            + correo + "')";

        int q_estado =
            mysql_query(
                cn.get_conexion(),
                consulta.c_str()
            );

        if (!q_estado) {

            cout << "Cliente agregado correctamente\n";
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
            mysql_query(
                cn.get_conexion(),
                consulta.c_str()
            );

        if (!q_estado) {

            resultado =
                mysql_store_result(
                    cn.get_conexion()
                );

            while (
                (fila = mysql_fetch_row(resultado))
                ) {

                cout
                    << fila[0] << " | "
                    << fila[1] << " | "
                    << fila[2] << " | "
                    << fila[3] << " | "
                    << fila[4] << " | "
                    << fila[5] << " | "
                    << fila[6]
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

    // ================= ACTUALIZAR =================
    void actualizar() {

        do {

            id_clientes =
                leer_entero(
                    "ID cliente a actualizar: "
                );

            if (!validar_id(id_clientes)) {

                cout << "ID invalido.\n";
            }

        } while (!validar_id(id_clientes));

        pedir_datos();

        ConexionBD cn;
        cn.abrir_conexion();

        if (cn.get_conexion() == NULL) {

            cout << "Error de conexion\n";
            return;
        }

        string consulta =
            "UPDATE clientes SET "
            "nombres='" + nombres +
            "', apellidos='" + apellidos +
            "', nit='" + nit +
            "', genero=" + to_string(genero) +
            ", telefono='" + telefono +
            "', correo_electronico='" + correo +
            "' WHERE id_clientes=" +
            to_string(id_clientes);

        int q_estado =
            mysql_query(
                cn.get_conexion(),
                consulta.c_str()
            );

        if (!q_estado) {

            cout << "Cliente actualizado correctamente\n";
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

        do {

            id_clientes =
                leer_entero(
                    "ID cliente a eliminar: "
                );

            if (!validar_id(id_clientes)) {

                cout << "ID invalido.\n";
            }

        } while (!validar_id(id_clientes));

        ConexionBD cn;
        cn.abrir_conexion();

        if (cn.get_conexion() == NULL) {

            cout << "Error de conexion\n";
            return;
        }

        string consulta =
            "DELETE FROM clientes "
            "WHERE id_clientes=" +
            to_string(id_clientes);

        int q_estado =
            mysql_query(
                cn.get_conexion(),
                consulta.c_str()
            );

        if (!q_estado) {

            cout << "Cliente eliminado correctamente\n";
        }
        else {

            cout << "Error: "
                << mysql_error(cn.get_conexion())
                << endl;
        }

        cn.cerrar_conexion();
    }
};