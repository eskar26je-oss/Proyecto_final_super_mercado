#pragma once
#include <iostream>
#include <string>
#include "CONEXION_BD.h"
#include <mysql.h>

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

    void setIdproductos(int idp) { idproductos = idp; }
    void setProductos(string prod) { productos = prod; }
    void setId_marca(int idm) { id_marca = idm; }
    void setDescripcion(string desc) { descripcion = desc; }
    void setImagen(string img) { imagen = img; }
    void setPrecio_costo(double costo) { precio_costo = costo; }
    void setPrecio_venta(double venta) { precio_venta = venta; }
    void setExistencia(int exist) { existencia = exist; }
    void setFecha_ingreso(string fecha) { fecha_ingreso = fecha; }

    void crear() {
        ConexionBD cn;
        cn.abrir_conexion();
        if (cn.get_conexion()) {
            // Tabla 'productos' en minusculas
            string consulta = "INSERT INTO productos(productos, id_marca, descripcion, imagen, precio_costo, precio_venta, existencia, fecha_ingreso) VALUES('"
                + productos + "',"
                + to_string(id_marca) + ",'"
                + descripcion + "','"
                + imagen + "',"
                + to_string(precio_costo) + ","
                + to_string(precio_venta) + ","
                + to_string(existencia) + ",'"
                + fecha_ingreso + "')";

            int q_estado = mysql_query(cn.get_conexion(), consulta.c_str());
            if (q_estado == 0) cout << "Producto agregado." << endl;
            else cout << "Error: " << mysql_error(cn.get_conexion()) << endl;
        }
        cn.cerrar_conexion();
    }

    void leer() {
        ConexionBD cn;
        cn.abrir_conexion();
        if (cn.get_conexion()) {
            // Tabla 'productos' en minusculas
            string consulta = "SELECT * FROM productos";
            int q_estado = mysql_query(cn.get_conexion(), consulta.c_str());
            if (q_estado == 0) {
                MYSQL_RES* resultado = mysql_store_result(cn.get_conexion());
                MYSQL_ROW fila;
                while ((fila = mysql_fetch_row(resultado))) {
                    cout << "ID: " << fila[0] << " | Prod: " << fila[1] << " | Stock: " << fila[7] << endl;
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
            // Tabla 'productos' en minusculas
            string consulta = "UPDATE productos SET productos='" + productos +
                "', id_marca=" + to_string(id_marca) +
                ", descripcion='" + descripcion +
                "', precio_venta=" + to_string(precio_venta) +
                ", existencia=" + to_string(existencia) +
                " WHERE idproductos=" + to_string(idproductos);

            int q_estado = mysql_query(cn.get_conexion(), consulta.c_str());
            if (q_estado == 0) cout << "Producto actualizado." << endl;
            else cout << "Error: " << mysql_error(cn.get_conexion()) << endl;
        }
        cn.cerrar_conexion();
    }

    void eliminar() {
        ConexionBD cn;
        cn.abrir_conexion();
        if (cn.get_conexion()) {
            // Tabla 'productos' en minusculas
            string consulta = "DELETE FROM productos WHERE idproductos=" + to_string(idproductos);
            int q_estado = mysql_query(cn.get_conexion(), consulta.c_str());
            if (q_estado == 0) cout << "Producto eliminado." << endl;
            else cout << "Error: " << mysql_error(cn.get_conexion()) << endl;
        }
        cn.cerrar_conexion();
    }
};