#pragma once

#include <vector>
#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <mysql.h>
#include "CONEXION_BD.h"

using namespace std;

// Estructura para detalles de compra
struct DetalleCompra {
    int id_detalle;
    int id_producto;
    string nombre_producto;
    int cantidad;
    double precio_costo_unitario;
    double subtotal;

    DetalleCompra() : id_detalle(0), id_producto(0), cantidad(0),
        precio_costo_unitario(0), subtotal(0) {
    }
};

// Clase para gestionar Compras Maestro-Detalle
class ComprasMaestroDetalle {
private:
    int id_compra;
    int numero_orden;
    int id_proveedor;
    string proveedor_nombre;
    string fecha_orden;
    string fecha_ingreso;
    vector<DetalleCompra> detalles;
    double total;

    // FIX: usar localtime_s en lugar de localtime
    string ObtenerFechaActual() {
        time_t now = time(0);
        struct tm tstruct;
        localtime_s(&tstruct, &now);          // <-- corregido
        char buffer[20];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d", &tstruct);
        return string(buffer);
    }

    string ObtenerFechaHoraActual() {
        time_t t = time(nullptr);
        struct tm tstruct;
        localtime_s(&tstruct, &t);            // <-- corregido
        char buffer[30];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &tstruct);
        return string(buffer);
    }

public:
    ComprasMaestroDetalle() : id_compra(0), numero_orden(0), id_proveedor(0), total(0) {
        fecha_orden = ObtenerFechaActual();
        fecha_ingreso = ObtenerFechaHoraActual();
    }

    // ======= Setters del Maestro =======
    void SetIdCompra(int id) { id_compra = id; }
    void SetNumeroOrden(int numero) { numero_orden = numero; }
    void SetProveedor(int id, const string& nombre) { id_proveedor = id; proveedor_nombre = nombre; }
    void SetFechaOrden(const string& fecha) { fecha_orden = fecha; }

    // ======= Getters del Maestro =======
    int    GetIdCompra()        const { return id_compra; }
    int    GetNumeroOrden()     const { return numero_orden; }
    int    GetIdProveedor()     const { return id_proveedor; }
    string GetProveedorNombre() const { return proveedor_nombre; }
    string GetFechaOrden()      const { return fecha_orden; }
    string GetFechaIngreso()    const { return fecha_ingreso; }
    double GetTotal()           const { return total; }

    // ======= Metodos para detalles =======
    void AgregarDetalle(int id_producto, const string& nombre, int cantidad, double precio_unitario) {
        if (cantidad <= 0 || precio_unitario < 0) return;

        DetalleCompra detalle;
        // FIX: cast explícito de size_t a int
        detalle.id_detalle = (int)detalles.size() + 1;
        detalle.id_producto = id_producto;
        detalle.nombre_producto = nombre;
        detalle.cantidad = cantidad;
        detalle.precio_costo_unitario = precio_unitario;
        detalle.subtotal = cantidad * precio_unitario;

        detalles.push_back(detalle);
        CalcularTotal();
    }

    bool ModificarDetalle(int indice, int cantidad, double precio_unitario) {
        if (indice < 0 || indice >= (int)detalles.size()) return false;
        if (cantidad <= 0 || precio_unitario < 0) return false;

        detalles[indice].cantidad = cantidad;
        detalles[indice].precio_costo_unitario = precio_unitario;
        detalles[indice].subtotal = cantidad * precio_unitario;
        CalcularTotal();
        return true;
    }

    bool EliminarDetalle(int indice) {
        if (indice < 0 || indice >= (int)detalles.size()) return false;
        detalles.erase(detalles.begin() + indice);
        CalcularTotal();
        return true;
    }

    void CalcularTotal() {
        total = 0;
        for (const auto& d : detalles)
            total += d.subtotal;
    }

    const vector<DetalleCompra>& GetDetalles() const { return detalles; }
    int GetCantidadDetalles() const { return (int)detalles.size(); }

    void LimpiarDetalles() {
        detalles.clear();
        total = 0;
    }

    bool ValidarCompra() const {
        return id_compra > 0 && numero_orden > 0 && id_proveedor > 0 && !detalles.empty();
    }

    // ======= Mostrar resumen en pantalla =======
    void MostrarResumen() const {
        cout << "\n---------- RESUMEN DE COMPRA ----------\n";
        cout << "ID Compra  : " << id_compra << "\n";
        cout << "No. Orden  : " << numero_orden << "\n";
        cout << "Proveedor  : [" << id_proveedor << "] " << proveedor_nombre << "\n";
        cout << "Fecha      : " << fecha_orden << "\n";
        cout << "---------------------------------------\n";
        cout << "No. | Producto                | Cant | Costo   | Subtotal\n";
        cout << "----+-------------------------+------+---------+---------\n";
        for (int i = 0; i < (int)detalles.size(); i++) {
            const DetalleCompra& d = detalles[i];
            cout << " " << (i + 1) << "  | "
                << d.nombre_producto.substr(0, 23)
                << string(max(0, 23 - (int)d.nombre_producto.size()), ' ')
                << " | " << d.cantidad
                << "    | Q" << fixed << setprecision(2) << d.precio_costo_unitario
                << " | Q" << d.subtotal << "\n";
        }
        cout << "---------------------------------------\n";
        cout << "TOTAL: Q" << fixed << setprecision(2) << total << "\n";
        cout << "---------------------------------------\n";
    }

    // ======= Guardar en BD (INSERT maestro + detalles en transaccion) =======
    bool GuardarEnBD() {
        if (!ValidarCompra()) {
            cout << "Error: La compra no esta completa.\n";
            return false;
        }

        ConexionBD cn;
        cn.abrir_conexion();
        MYSQL* con = cn.get_conexion();
        if (!con) return false;

        mysql_query(con, "START TRANSACTION");

        // INSERT maestro
        string qMaestro = "INSERT INTO compras(id_compras, no_de_orden, id_proveedor, fecha_orden, fecha_ingreso) VALUES("
            + to_string(id_compra) + ","
            + to_string(numero_orden) + ","
            + to_string(id_proveedor) + ",'"
            + fecha_orden + "','"
            + fecha_ingreso + "')";

        if (mysql_query(con, qMaestro.c_str()) != 0) {
            cout << "Error al guardar compra: " << mysql_error(con) << "\n";
            mysql_query(con, "ROLLBACK");
            cn.cerrar_conexion();
            return false;
        }

        // INSERT detalles
        for (int i = 0; i < (int)detalles.size(); i++) {
            const DetalleCompra& d = detalles[i];
            long long id_det = (long long)id_compra * 1000 + (i + 1);

            string qDetalle = "INSERT INTO compras_detalle(id_compras_detalle, id_compra, id_producto, cantidad, precio_costo_unitario) VALUES("
                + to_string(id_det) + ","
                + to_string(id_compra) + ","
                + to_string(d.id_producto) + ","
                + to_string(d.cantidad) + ","
                + to_string(d.precio_costo_unitario) + ")";

            if (mysql_query(con, qDetalle.c_str()) != 0) {
                cout << "Error al guardar detalle " << (i + 1) << ": " << mysql_error(con) << "\n";
                mysql_query(con, "ROLLBACK");
                cn.cerrar_conexion();
                return false;
            }
        }

        mysql_query(con, "COMMIT");
        cout << "Compra guardada exitosamente.\n";
        cn.cerrar_conexion();
        return true;
    }

    // ======= Consultar compras existentes =======
    static void ConsultarCompras() {
        ConexionBD cn;
        cn.abrir_conexion();
        MYSQL* con = cn.get_conexion();
        if (!con) return;

        string q = "SELECT c.id_compras, c.no_de_orden, c.fecha_orden, "
            "p.proveedor, "
            "SUM(cd.cantidad * cd.precio_costo_unitario) AS total "
            "FROM compras c "
            "JOIN proveedores p ON c.id_proveedor = p.id_proveedor "
            "LEFT JOIN compras_detalle cd ON c.id_compras = cd.id_compra "
            "GROUP BY c.id_compras "
            "ORDER BY c.id_compras DESC";

        if (mysql_query(con, q.c_str()) == 0) {
            MYSQL_RES* res = mysql_store_result(con);
            MYSQL_ROW fila;
            cout << "\n===== COMPRAS REGISTRADAS =====\n";
            cout << "ID  | Orden | Fecha      | Proveedor            | Total\n";
            cout << "----+-------+------------+----------------------+-------\n";
            while ((fila = mysql_fetch_row(res))) {
                cout << fila[0] << " | " << fila[1]
                    << " | " << fila[2] << " | "
                    << string(fila[3]).substr(0, 20) << " | Q"
                    << (fila[4] ? fila[4] : "0.00") << "\n";
            }
            mysql_free_result(res);
        }
        else {
            cout << "Error: " << mysql_error(con) << "\n";
        }

        cn.cerrar_conexion();
    }

    // ======= Consultar detalle de una compra =======
    static void ConsultarDetalleCompra(int id_c) {
        ConexionBD cn;
        cn.abrir_conexion();
        MYSQL* con = cn.get_conexion();
        if (!con) return;

        string q = "SELECT p.productos, cd.cantidad, cd.precio_costo_unitario, "
            "(cd.cantidad * cd.precio_costo_unitario) AS subtotal "
            "FROM compras_detalle cd "
            "JOIN productos p ON cd.id_producto = p.idproductos "
            "WHERE cd.id_compra = " + to_string(id_c);

        if (mysql_query(con, q.c_str()) == 0) {
            MYSQL_RES* res = mysql_store_result(con);
            MYSQL_ROW fila;
            cout << "\n--- Detalle de Compra ID: " << id_c << " ---\n";
            while ((fila = mysql_fetch_row(res))) {
                cout << "Producto: " << fila[0]
                    << " | Cant: " << fila[1]
                    << " | Costo: Q" << fila[2]
                    << " | Subtotal: Q" << fila[3] << "\n";
            }
            mysql_free_result(res);
        }
        else {
            cout << "Error: " << mysql_error(con) << "\n";
        }

        cn.cerrar_conexion();
    }

    // ======= Eliminar compra y sus detalles =======
    static bool EliminarCompra(int id_c) {
        ConexionBD cn;
        cn.abrir_conexion();
        MYSQL* con = cn.get_conexion();
        if (!con) return false;

        mysql_query(con, "START TRANSACTION");

        string qDet = "DELETE FROM compras_detalle WHERE id_compra = " + to_string(id_c);
        string qMae = "DELETE FROM compras WHERE id_compras = " + to_string(id_c);

        if (mysql_query(con, qDet.c_str()) != 0 || mysql_query(con, qMae.c_str()) != 0) {
            cout << "Error al eliminar: " << mysql_error(con) << "\n";
            mysql_query(con, "ROLLBACK");
            cn.cerrar_conexion();
            return false;
        }

        mysql_query(con, "COMMIT");
        cout << "Compra eliminada exitosamente.\n";
        cn.cerrar_conexion();
        return true;
    }
};