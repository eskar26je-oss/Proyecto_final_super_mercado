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

// Estructura para detalles de venta
struct DetalleVenta {
    int id_detalle;
    int id_producto;
    string nombre_producto;
    int cantidad;
    double precio_unitario;
    double subtotal;

    DetalleVenta() : id_detalle(0), id_producto(0), cantidad(0),
                     precio_unitario(0), subtotal(0) {}
};

// Clase para gestionar Ventas Maestro-Detalle
class VentasMaestroDetalle {
private:
    int id_venta;
    int no_factura;
    char serie;
    string fecha_factura;
    string fecha_ingreso;
    int id_cliente;
    string cliente_nombre;
    int id_empleado;
    string empleado_nombre;
    vector<DetalleVenta> detalles;
    double total;

    string ObtenerFechaActual() {
        time_t now = time(0);
        struct tm tstruct = *localtime(&now);
        char buffer[20];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d", &tstruct);
        return string(buffer);
    }

    string ObtenerFechaHoraActual() {
        time_t now = time(0);
        struct tm tstruct = *localtime(&now);
        char buffer[30];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &tstruct);
        return string(buffer);
    }

public:
    VentasMaestroDetalle() : id_venta(0), no_factura(0), serie('A'),
                              id_cliente(0), id_empleado(0), total(0) {
        fecha_factura = ObtenerFechaActual();
        fecha_ingreso = ObtenerFechaHoraActual();
    }

    // ======= Setters del Maestro =======
    void SetIdVenta(int id)              { id_venta = id; }
    void SetNoFactura(int num)           { no_factura = num; }
    void SetSerie(char s)                { serie = s; }
    void SetFechaFactura(const string& f){ fecha_factura = f; }
    void SetCliente(int id, const string& nombre) {
        id_cliente = id;
        cliente_nombre = nombre;
    }
    void SetEmpleado(int id, const string& nombre) {
        id_empleado = id;
        empleado_nombre = nombre;
    }

    // ======= Getters del Maestro =======
    int    GetIdVenta()         const { return id_venta; }
    int    GetNoFactura()       const { return no_factura; }
    char   GetSerie()           const { return serie; }
    string GetFechaFactura()    const { return fecha_factura; }
    string GetFechaIngreso()    const { return fecha_ingreso; }
    int    GetIdCliente()       const { return id_cliente; }
    string GetClienteNombre()   const { return cliente_nombre; }
    int    GetIdEmpleado()      const { return id_empleado; }
    string GetEmpleadoNombre()  const { return empleado_nombre; }
    double GetTotal()           const { return total; }

    // ======= Metodos para detalles =======
    void AgregarDetalle(int id_producto, const string& nombre, int cantidad, double precio_unitario) {
        if (cantidad <= 0 || precio_unitario < 0) return;

        DetalleVenta detalle;
        detalle.id_detalle      = detalles.size() + 1;
        detalle.id_producto     = id_producto;
        detalle.nombre_producto = nombre;
        detalle.cantidad        = cantidad;
        detalle.precio_unitario = precio_unitario;
        detalle.subtotal        = cantidad * precio_unitario;

        detalles.push_back(detalle);
        CalcularTotal();
    }

    bool ModificarDetalle(int indice, int cantidad, double precio_unitario) {
        if (indice < 0 || indice >= (int)detalles.size()) return false;
        if (cantidad <= 0 || precio_unitario < 0) return false;

        detalles[indice].cantidad        = cantidad;
        detalles[indice].precio_unitario = precio_unitario;
        detalles[indice].subtotal        = cantidad * precio_unitario;
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

    const vector<DetalleVenta>& GetDetalles() const { return detalles; }
    int GetCantidadDetalles() const { return (int)detalles.size(); }

    void LimpiarDetalles() {
        detalles.clear();
        total = 0;
    }

    bool ValidarVenta() const {
        return id_venta > 0 && no_factura > 0 && id_cliente > 0
            && id_empleado > 0 && detalles.size() > 0;
    }

    // ======= Mostrar resumen de la venta en pantalla =======
    void MostrarResumen() const {
        cout << "\n---------- RESUMEN DE VENTA ----------\n";
        cout << "ID Venta   : " << id_venta      << "\n";
        cout << "No.Factura : " << no_factura     << "  Serie: " << serie << "\n";
        cout << "Fecha      : " << fecha_factura  << "\n";
        cout << "Cliente    : [" << id_cliente << "] " << cliente_nombre  << "\n";
        cout << "Empleado   : [" << id_empleado << "] " << empleado_nombre << "\n";
        cout << "--------------------------------------\n";
        cout << "No. | Producto                | Cant | Precio  | Subtotal\n";
        cout << "----+-------------------------+------+---------+---------\n";
        for (int i = 0; i < (int)detalles.size(); i++) {
            const DetalleVenta& d = detalles[i];
            cout << " " << (i + 1) << "  | "
                 << d.nombre_producto.substr(0, 23)
                 << string(max(0, 23 - (int)d.nombre_producto.size()), ' ')
                 << " | " << d.cantidad
                 << "    | Q" << fixed << setprecision(2) << d.precio_unitario
                 << " | Q" << d.subtotal << "\n";
        }
        cout << "--------------------------------------\n";
        cout << "TOTAL: Q" << fixed << setprecision(2) << total << "\n";
        cout << "--------------------------------------\n";
    }

    // ======= Guardar en BD (INSERT maestro + detalles en transaccion) =======
    bool GuardarEnBD() {
        if (!ValidarVenta()) {
            cout << "Error: La venta no esta completa.\n";
            return false;
        }

        ConexionBD cn;
        cn.abrir_conexion();
        MYSQL* con = cn.get_conexion();
        if (!con) return false;

        // Iniciar transaccion
        mysql_query(con, "START TRANSACTION");

        // INSERT maestro
        string qMaestro = "INSERT INTO ventas(id_ventas, no_factura, serie, fecha_factura, id_cliente, id_empleado, fecha_ingreso) VALUES("
            + to_string(id_venta) + ","
            + to_string(no_factura) + ",'"
            + serie + "','"
            + fecha_factura + "',"
            + to_string(id_cliente) + ","
            + to_string(id_empleado) + ",'"
            + fecha_ingreso + "')";

        if (mysql_query(con, qMaestro.c_str()) != 0) {
            cout << "Error al guardar venta: " << mysql_error(con) << "\n";
            mysql_query(con, "ROLLBACK");
            cn.cerrar_conexion();
            return false;
        }

        // INSERT detalles
        for (int i = 0; i < (int)detalles.size(); i++) {
            const DetalleVenta& d = detalles[i];
            // id_venta_detalle = id_venta * 1000 + i+1 (simple, evita colision)
            long long id_det = (long long)id_venta * 1000 + (i + 1);

            string qDetalle = "INSERT INTO venta_detalle(id_venta_detalle, id_venta, id_producto, cantidad, precio_unitario) VALUES("
                + to_string(id_det) + ","
                + to_string(id_venta) + ","
                + to_string(d.id_producto) + ","
                + to_string(d.cantidad) + ","
                + to_string(d.precio_unitario) + ")";

            if (mysql_query(con, qDetalle.c_str()) != 0) {
                cout << "Error al guardar detalle " << (i + 1) << ": " << mysql_error(con) << "\n";
                mysql_query(con, "ROLLBACK");
                cn.cerrar_conexion();
                return false;
            }
        }

        mysql_query(con, "COMMIT");
        cout << "Venta guardada exitosamente.\n";
        cn.cerrar_conexion();
        return true;
    }

    // ======= Consultar ventas existentes =======
    static void ConsultarVentas() {
        ConexionBD cn;
        cn.abrir_conexion();
        MYSQL* con = cn.get_conexion();
        if (!con) return;

        string q = "SELECT v.id_ventas, v.no_factura, v.serie, v.fecha_factura, "
                   "CONCAT(c.nombres,' ',c.apellidos), "
                   "CONCAT(e.nombres,' ',e.apellidos), "
                   "SUM(vd.cantidad * vd.precio_unitario) AS total "
                   "FROM ventas v "
                   "JOIN clientes c ON v.id_cliente = c.id_clientes "
                   "JOIN empleados e ON v.id_empleado = e.id_empleados "
                   "LEFT JOIN venta_detalle vd ON v.id_ventas = vd.id_venta "
                   "GROUP BY v.id_ventas "
                   "ORDER BY v.id_ventas DESC";

        if (mysql_query(con, q.c_str()) == 0) {
            MYSQL_RES* res = mysql_store_result(con);
            MYSQL_ROW fila;
            cout << "\n===== VENTAS REGISTRADAS =====\n";
            cout << "ID  | Factura | S | Fecha      | Cliente              | Empleado             | Total\n";
            cout << "----+---------+---+------------+----------------------+----------------------+-------\n";
            while ((fila = mysql_fetch_row(res))) {
                cout << fila[0] << " | " << fila[1] << "  | " << fila[2]
                     << " | " << fila[3] << " | "
                     << string(fila[4]).substr(0, 20) << " | "
                     << string(fila[5]).substr(0, 20) << " | Q"
                     << (fila[6] ? fila[6] : "0.00") << "\n";
            }
            mysql_free_result(res);
        } else {
            cout << "Error: " << mysql_error(con) << "\n";
        }

        cn.cerrar_conexion();
    }

    // ======= Consultar detalle de una venta =======
    static void ConsultarDetalleVenta(int id_v) {
        ConexionBD cn;
        cn.abrir_conexion();
        MYSQL* con = cn.get_conexion();
        if (!con) return;

        string q = "SELECT p.productos, vd.cantidad, vd.precio_unitario, "
                   "(vd.cantidad * vd.precio_unitario) AS subtotal "
                   "FROM venta_detalle vd "
                   "JOIN productos p ON vd.id_producto = p.idproductos "
                   "WHERE vd.id_venta = " + to_string(id_v);

        if (mysql_query(con, q.c_str()) == 0) {
            MYSQL_RES* res = mysql_store_result(con);
            MYSQL_ROW fila;
            cout << "\n--- Detalle de Venta ID: " << id_v << " ---\n";
            while ((fila = mysql_fetch_row(res))) {
                cout << "Producto: " << fila[0]
                     << " | Cant: " << fila[1]
                     << " | Precio: Q" << fila[2]
                     << " | Subtotal: Q" << fila[3] << "\n";
            }
            mysql_free_result(res);
        } else {
            cout << "Error: " << mysql_error(con) << "\n";
        }

        cn.cerrar_conexion();
    }

    // ======= Eliminar venta y sus detalles =======
    static bool EliminarVenta(int id_v) {
        ConexionBD cn;
        cn.abrir_conexion();
        MYSQL* con = cn.get_conexion();
        if (!con) return false;

        mysql_query(con, "START TRANSACTION");

        string qDet = "DELETE FROM venta_detalle WHERE id_venta = " + to_string(id_v);
        string qMae = "DELETE FROM ventas WHERE id_ventas = " + to_string(id_v);

        if (mysql_query(con, qDet.c_str()) != 0 || mysql_query(con, qMae.c_str()) != 0) {
            cout << "Error al eliminar: " << mysql_error(con) << "\n";
            mysql_query(con, "ROLLBACK");
            cn.cerrar_conexion();
            return false;
        }

        mysql_query(con, "COMMIT");
        cout << "Venta eliminada exitosamente.\n";
        cn.cerrar_conexion();
        return true;
    }
};
