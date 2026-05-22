#pragma once
#include <iostream>
#include <string>

#include <mysql.h>

#include "CONEXION_BD.h"
#include "validaciones.h"

using namespace std;

class Producto {

private:

    int idproductos;
    string productos;
    int id_marca;
    string descripcion;
    string imagen;
    double precio_costo;
    double precio_venta;
    int existencia;
    string fecha_ingreso;

public:

    // ================= CONSTRUCTOR =================
    Producto() {

        idproductos = 0;
        productos = "";
        id_marca = 0;
        descripcion = "";
        imagen = "";
        precio_costo = 0.0;
        precio_venta = 0.0;
        existencia = 0;
        fecha_ingreso = "";
    }

    // ================= SETTERS =================
    void setIdproductos(int idp) {

        if (validar_id(idp)) {

            idproductos = idp;
        }
        else {

            cout << "ID invalido.\n";
        }
    }

    void setProductos(string prod) {

        if (validar_texto_general(prod, 50)) {

            productos = prod;
        }
        else {

            cout << "Producto invalido.\n";
        }
    }

    void setId_marca(int idm) {

        if (validar_id(idm)) {

            id_marca = idm;
        }
        else {

            cout << "ID marca invalido.\n";
        }
    }

    void setDescripcion(string desc) {

        if (validar_descripcion(desc, 100)) {

            descripcion = desc;
        }
        else {

            cout << "Descripcion invalida.\n";
        }
    }

    void setImagen(string img) {

        if (validar_imagen(img, 30)) {

            imagen = img;
        }
        else {

            cout << "Imagen invalida.\n";
        }
    }

    void setPrecio_costo(double costo) {

        if (validar_precio((float)costo)) {

            precio_costo = costo;
        }
        else {

            cout << "Precio costo invalido.\n";
        }
    }

    void setPrecio_venta(double venta) {

        if (validar_precio((float)venta)) {

            precio_venta = venta;
        }
        else {

            cout << "Precio venta invalido.\n";
        }
    }

    void setExistencia(int exist) {

        if (validar_stock(exist)) {

            existencia = exist;
        }
        else {

            cout << "Existencia invalida.\n";
        }
    }

    void setFecha_ingreso(string fecha) {

        if (validar_fecha(fecha)) {

            fecha_ingreso = fecha;
        }
        else {

            cout << "Fecha invalida.\n";
        }
    }

    // ================= CREATE =================
    void crear() {

        // ================= VALIDACIONES =================
        if (!validar_texto_general(productos, 50)) {

            cout << "Producto invalido.\n";
            return;
        }

        if (!validar_id(id_marca)) {

            cout << "ID marca invalido.\n";
            return;
        }

        if (!validar_descripcion(descripcion, 100)) {

            cout << "Descripcion invalida.\n";
            return;
        }

        if (!validar_imagen(imagen, 30)) {

            cout << "Imagen invalida.\n";
            return;
        }

        if (!validar_precio((float)precio_costo)) {

            cout << "Precio costo invalido.\n";
            return;
        }

        if (!validar_precio((float)precio_venta)) {

            cout << "Precio venta invalido.\n";
            return;
        }

        if (!validar_stock(existencia)) {

            cout << "Existencia invalida.\n";
            return;
        }

        if (!validar_fecha(fecha_ingreso)) {

            cout << "Fecha invalida.\n";
            return;
        }

        // ================= CONEXION =================
        ConexionBD cn;
        cn.abrir_conexion();

        if (!cn.get_conexion()) {

            cout << "Error de conexion.\n";
            return;
        }

        // ================= QUERY =================
        string consulta =
            "INSERT INTO productos("
            "productos,"
            "id_marca,"
            "descripcion,"
            "imagen,"
            "precio_costo,"
            "precio_venta,"
            "existencia,"
            "fecha_ingreso"
            ") VALUES('"

            + productos + "',"
            + to_string(id_marca) + ",'"
            + descripcion + "','"
            + imagen + "',"
            + to_string(precio_costo) + ","
            + to_string(precio_venta) + ","
            + to_string(existencia) + ",'"
            + fecha_ingreso + "')";

        int q_estado =
            mysql_query(
                cn.get_conexion(),
                consulta.c_str()
            );

        if (q_estado == 0) {

            cout << "Producto agregado correctamente.\n";
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
            "SELECT * FROM productos";

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
                    << " | Producto: "
                    << fila[1]
                    << " | Marca ID: "
                    << fila[2]
                    << " | Precio Venta: "
                    << fila[6]
                    << " | Existencia: "
                    << fila[7]
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

        // ================= VALIDACIONES =================
        if (!validar_id(idproductos)) {

            cout << "ID invalido.\n";
            return;
        }

        if (!validar_texto_general(productos, 50)) {

            cout << "Producto invalido.\n";
            return;
        }

        if (!validar_id(id_marca)) {

            cout << "ID marca invalido.\n";
            return;
        }

        if (!validar_descripcion(descripcion, 100)) {

            cout << "Descripcion invalida.\n";
            return;
        }

        if (!validar_imagen(imagen, 30)) {

            cout << "Imagen invalida.\n";
            return;
        }

        if (!validar_precio((float)precio_costo)) {

            cout << "Precio costo invalido.\n";
            return;
        }

        if (!validar_precio((float)precio_venta)) {

            cout << "Precio venta invalido.\n";
            return;
        }

        if (!validar_stock(existencia)) {

            cout << "Existencia invalida.\n";
            return;
        }

        if (!validar_fecha(fecha_ingreso)) {

            cout << "Fecha invalida.\n";
            return;
        }

        // ================= CONEXION =================
        ConexionBD cn;
        cn.abrir_conexion();

        if (!cn.get_conexion()) {

            cout << "Error de conexion.\n";
            return;
        }

        // ================= QUERY =================
        string consulta =
            "UPDATE productos SET "
            "productos='" + productos +
            "', id_marca=" + to_string(id_marca) +
            ", descripcion='" + descripcion +
            "', imagen='" + imagen +
            "', precio_costo=" + to_string(precio_costo) +
            ", precio_venta=" + to_string(precio_venta) +
            ", existencia=" + to_string(existencia) +
            ", fecha_ingreso='" + fecha_ingreso +
            "' WHERE idproductos=" +
            to_string(idproductos);

        int q_estado =
            mysql_query(
                cn.get_conexion(),
                consulta.c_str()
            );

        if (q_estado == 0) {

            cout << "Producto actualizado correctamente.\n";
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

        if (!validar_id(idproductos)) {

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
            "DELETE FROM productos "
            "WHERE idproductos="
            + to_string(idproductos);

        int q_estado =
            mysql_query(
                cn.get_conexion(),
                consulta.c_str()
            );

        if (q_estado == 0) {

            cout << "Producto eliminado correctamente.\n";
        }
        else {

            cout << "Error: "
                << mysql_error(cn.get_conexion())
                << endl;
        }

        cn.cerrar_conexion();
    }
};