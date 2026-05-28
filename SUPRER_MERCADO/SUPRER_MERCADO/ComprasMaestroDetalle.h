#pragma once

#include <vector>
#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <mysql.h>
#include "CONEXION_BD.h"

using namespace std;

// ================= ESTRUCTURA DETALLE =================

struct DetalleCompra {
    int         id_detalle;
    int         id_producto;
    string      nombre_producto;
    string      marca;
    int         cantidad;
    double      precio_costo_unitario;
    double      subtotal;

    DetalleCompra() : id_detalle(0), id_producto(0),
        cantidad(0), precio_costo_unitario(0), subtotal(0) {}
};

// ================= CLASE COMPRAS MAESTRO-DETALLE =================

class ComprasMaestroDetalle {

private:
    int    id_compra;
    int    no_de_orden;
    int    id_proveedor;
    string nombre_proveedor;
    string nit_proveedor;
    string fecha_orden;
    string fecha_ingreso;
    double total;
    vector<DetalleCompra> detalles;

    // ---- helpers de fecha ----
    string obtenerFecha() {
        time_t now = time(0);
        struct tm t;
        localtime_s(&t, &now);
        char buf[11];
        strftime(buf, sizeof(buf), "%Y-%m-%d", &t);
        return string(buf);
    }

    string obtenerFechaHora() {
        time_t now = time(0);
        struct tm t;
        localtime_s(&t, &now);
        char buf[20];
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &t);
        return string(buf);
    }

    // ---- auto ID desde BD ----
    int obtenerProximoIdCompra(MYSQL* con) {
        mysql_query(con, "SELECT IFNULL(MAX(id_compras), 0) + 1 FROM compras");
        MYSQL_RES* r = mysql_store_result(con);
        MYSQL_ROW  f = mysql_fetch_row(r);
        int id = atoi(f[0]);
        mysql_free_result(r);
        return id;
    }

    long long obtenerProximoIdDetalle(MYSQL* con) {
        mysql_query(con, "SELECT IFNULL(MAX(id_compras_detalle), 0) + 1 FROM compras_detalle");
        MYSQL_RES* r = mysql_store_result(con);
        MYSQL_ROW  f = mysql_fetch_row(r);
        long long id = atoll(f[0]);
        mysql_free_result(r);
        return id;
    }

    // ---- calculo total ----
    void calcularTotal() {
        total = 0;
        for (const auto& d : detalles)
            total += d.subtotal;
    }

    // ---- construir lineas del recibo ----
    vector<string> construirRecibo() {

        const int ANCHO = 54;
        vector<string> lineas;

        auto centrar = [&](string texto) -> string {
            int esp = (ANCHO - 2 - (int)texto.length()) / 2;
            if (esp < 0) esp = 0;
            string l = "| ";
            for (int i = 0; i < esp; i++) l += " ";
            l += texto;
            int rest = ANCHO - 2 - esp - (int)texto.length();
            for (int i = 0; i < rest; i++) l += " ";
            l += " |";
            return l;
        };

        auto separador = [&]() -> string {
            string s = "+";
            for (int i = 0; i < ANCHO; i++) s += "-";
            s += "+";
            return s;
        };

        auto filaTabla = [&](string cod, string prod,
            string cant, string costo, string sub) -> string {
                if (prod.length() > 17) prod = prod.substr(0, 17);
                ostringstream ss;
                ss << "| "
                    << left  << setw(5)  << cod
                    << left  << setw(18) << prod
                    << right << setw(5)  << cant
                    << right << setw(10) << costo
                    << right << setw(10) << sub
                    << " |";
                return ss.str();
        };

        ostringstream num_ord;
        num_ord << "ORD-" << setw(4) << setfill('0') << no_de_orden;

        ostringstream total_str;
        total_str << "Q" << fixed << setprecision(2) << total;

        lineas.push_back(separador());
        lineas.push_back(centrar(""));
        lineas.push_back(centrar("SUPERMERCADO PROFIN"));
        lineas.push_back(centrar("Orden de Compra"));
        lineas.push_back(centrar(""));
        lineas.push_back(separador());
        lineas.push_back(centrar("Orden     : " + num_ord.str() + "   Fecha: " + fecha_orden));
        lineas.push_back(centrar("Proveedor : " + nombre_proveedor));
        lineas.push_back(centrar("NIT       : " + nit_proveedor));
        lineas.push_back(centrar(""));
        lineas.push_back(separador());
        lineas.push_back(filaTabla("COD", "PRODUCTO", "CANT", "COSTO", "SUBTOTAL"));
        lineas.push_back(separador());

        for (const auto& d : detalles) {
            ostringstream cs, ss2;
            cs  << "Q" << fixed << setprecision(2) << d.precio_costo_unitario;
            ss2 << "Q" << fixed << setprecision(2) << d.subtotal;
            lineas.push_back(filaTabla(
                to_string(d.id_producto),
                d.nombre_producto,
                to_string(d.cantidad),
                cs.str(),
                ss2.str()
            ));
        }

        lineas.push_back(separador());
        lineas.push_back(centrar("TOTAL:  " + total_str.str()));
        lineas.push_back(centrar(""));
        lineas.push_back(separador());
        lineas.push_back("");
        lineas.push_back("        ** Compra registrada exitosamente **");
        lineas.push_back("           Gracias por su servicio");
        lineas.push_back("");

        return lineas;
    }

    // ---- imprimir recibo en consola ----
    void imprimirReciboConsola() {
        system("cls");
        vector<string> lineas = construirRecibo();
        for (const auto& l : lineas)
            cout << l << "\n";
    }

    // ---- guardar recibo en .txt y abrir notepad ----
    void guardarEImprimirRecibo() {
        string archivo = "compra_ORD-" + to_string(no_de_orden) + ".txt";
        ofstream f(archivo);
        if (!f.is_open()) {
            cout << "\nERROR: No se pudo crear el archivo.\n";
            return;
        }
        vector<string> lineas = construirRecibo();
        for (const auto& l : lineas)
            f << l << "\n";
        f.close();

        cout << "\nArchivo guardado: " << archivo << "\n";
        cout << "Abriendo Bloc de Notas...\n";
        cout << "Presiona Ctrl+P dentro del Bloc de Notas para imprimir.\n\n";
        string cmd = "notepad " + archivo;
        system(cmd.c_str());
    }

public:

    ComprasMaestroDetalle() : id_compra(0), no_de_orden(0),
        id_proveedor(0), total(0) {
        fecha_orden   = obtenerFecha();
        fecha_ingreso = obtenerFechaHora();
    }

    double GetTotal() const { return total; }

    // ================= NUEVA COMPRA =================

    void nuevaCompra(MYSQL* conectar) {

        detalles.clear();
        total = 0;

        system("cls");
        cout << "+--------------------------------------+\n";
        cout << "|           NUEVA COMPRA               |\n";
        cout << "+--------------------------------------+\n\n";

        // ---- listar proveedores ----
        MYSQL_RES* res_prov;
        MYSQL_ROW  fil_prov;

        mysql_query(conectar,
            "SELECT id_proveedor, proveedor, nit FROM proveedores ORDER BY id_proveedor");
        res_prov = mysql_store_result(conectar);

        cout << "\n+--------------------------------------+\n";
        cout << "|         SELECCIONAR PROVEEDOR        |\n";
        cout << "+--------------------------------------+\n";

        while ((fil_prov = mysql_fetch_row(res_prov))) {
            cout << "  [" << fil_prov[0] << "] "
                << fil_prov[1]
                << "  NIT: " << fil_prov[2] << "\n";
        }
        mysql_free_result(res_prov);

        cout << "\nIngrese ID del proveedor: ";
        cin >> id_proveedor;
        cin.ignore();

        // ---- verificar proveedor ----
        MYSQL_RES* res_ver;
        MYSQL_ROW  fil_ver;

        string q_ver =
            "SELECT proveedor, nit FROM proveedores WHERE id_proveedor = "
            + to_string(id_proveedor);

        mysql_query(conectar, q_ver.c_str());
        res_ver = mysql_store_result(conectar);
        fil_ver = mysql_fetch_row(res_ver);

        if (fil_ver == NULL) {
            cout << "\nERROR: Proveedor no encontrado. Operacion cancelada.\n";
            mysql_free_result(res_ver);
            system("pause");
            return;
        }

        nombre_proveedor = string(fil_ver[0]);
        nit_proveedor    = string(fil_ver[1]);
        mysql_free_result(res_ver);

        cout << "\nProveedor: " << nombre_proveedor << "\n";

        // ---- datos auto ----
        id_compra     = obtenerProximoIdCompra(conectar);
        no_de_orden   = rand() % 9000 + 1000;
        fecha_orden   = obtenerFecha();
        fecha_ingreso = obtenerFechaHora();

        cout << "\n+--------------------------------------+\n";
        cout << "  Fecha     : " << fecha_orden << "\n";
        cout << "  Orden     : ORD-" << no_de_orden << "\n";
        cout << "  Proveedor : " << nombre_proveedor << "\n";
        cout << "+--------------------------------------+\n";

        // ---- agregar productos ----
        char continuar;
        do {
            DetalleCompra dc;

            cout << "\nCodigo Producto: ";
            cin >> dc.id_producto;
            cin.ignore();

            MYSQL_RES* res_prod;
            MYSQL_ROW  fil_prod;

            string q_prod =
                "SELECT p.idproductos, p.productos, m.marca, p.precio_costo "
                "FROM productos p INNER JOIN marcas m ON p.id_marca = m.id_marca "
                "WHERE p.idproductos = " + to_string(dc.id_producto);

            mysql_query(conectar, q_prod.c_str());
            res_prod = mysql_store_result(conectar);
            fil_prod = mysql_fetch_row(res_prod);

            if (fil_prod != NULL) {
                dc.id_producto          = atoi(fil_prod[0]);
                dc.nombre_producto      = string(fil_prod[1]);
                dc.marca                = string(fil_prod[2]);
                dc.precio_costo_unitario = atof(fil_prod[3]);

                cout << "\n  ID      : " << dc.id_producto << "\n";
                cout << "  Producto: " << dc.nombre_producto << "\n";
                cout << "  Marca   : " << dc.marca << "\n";
                cout << "  Costo   : Q" << fixed << setprecision(2)
                    << dc.precio_costo_unitario << "\n";

                cout << "\nCantidad: ";
                cin >> dc.cantidad;
                cin.ignore();

                if (dc.cantidad <= 0) {
                    cout << "\nERROR: Cantidad invalida. Debe ser mayor a 0.\n";
                }
                else {
                    dc.subtotal = dc.cantidad * dc.precio_costo_unitario;
                    calcularTotal();
                    detalles.push_back(dc);
                    // recalcular despues de push
                    calcularTotal();
                    cout << "\nProducto agregado. Total acumulado: Q"
                        << fixed << setprecision(2) << total << "\n";
                }
            }
            else {
                cout << "\nERROR: Producto no encontrado.\n";
            }

            mysql_free_result(res_prod);

            cout << "\nAgregar otro producto? (s/n): ";
            cin >> continuar;
            cin.ignore();

        } while (continuar == 's' || continuar == 'S');

        if (detalles.empty()) {
            cout << "\nERROR: No hay productos. Operacion cancelada.\n";
            system("pause");
            return;
        }

        // ---- transaccion ----
        mysql_query(conectar, "START TRANSACTION");

        // INSERT maestro
        string q_maestro =
            "INSERT INTO compras(id_compras, no_de_orden, id_proveedor, fecha_orden, fecha_ingreso) VALUES("
            + to_string(id_compra)   + ","
            + to_string(no_de_orden) + ","
            + to_string(id_proveedor) + ",'"
            + fecha_orden  + "','"
            + fecha_ingreso + "')";

        if (mysql_query(conectar, q_maestro.c_str())) {
            cout << "\nERROR al guardar compra: " << mysql_error(conectar) << "\n";
            mysql_query(conectar, "ROLLBACK");
            system("pause");
            return;
        }

        // INSERT detalles + actualizar stock
        long long id_det   = obtenerProximoIdDetalle(conectar);
        bool      hay_error = false;

        for (int i = 0; i < (int)detalles.size(); i++) {
            const DetalleCompra& d = detalles[i];

            string q_det =
                "INSERT INTO compras_detalle(id_compras_detalle, id_compra, id_producto, cantidad, precio_costo_unitario) VALUES("
                + to_string(id_det)           + ","
                + to_string(id_compra)        + ","
                + to_string(d.id_producto)    + ","
                + to_string(d.cantidad)       + ","
                + to_string(d.precio_costo_unitario) + ")";

            if (mysql_query(conectar, q_det.c_str())) {
                cout << "\nERROR en detalle: " << mysql_error(conectar) << "\n";
                hay_error = true;
                break;
            }

            id_det++;

            // compra = entrada => sumar stock
            string q_stock =
                "UPDATE productos SET existencia = existencia + "
                + to_string(d.cantidad)
                + " WHERE idproductos = "
                + to_string(d.id_producto);

            if (mysql_query(conectar, q_stock.c_str())) {
                cout << "\nERROR al actualizar stock: " << mysql_error(conectar) << "\n";
                hay_error = true;
                break;
            }
        }

        if (hay_error) {
            mysql_query(conectar, "ROLLBACK");
            cout << "\nCompra revertida. Intente de nuevo.\n";
            system("pause");
            return;
        }

        mysql_query(conectar, "COMMIT");

        // ---- imprimir recibo ----
        imprimirReciboConsola();
        guardarEImprimirRecibo();
        cout << "En el Bloc de Notas presiona Ctrl+P para imprimir.\n";

        system("pause");
    }

    // ================= VER TODAS LAS COMPRAS =================

    static void consultarCompras(MYSQL* conectar) {
        string q =
            "SELECT c.id_compras, c.no_de_orden, c.fecha_orden, "
            "p.proveedor, "
            "IFNULL(SUM(cd.cantidad * cd.precio_costo_unitario), 0) AS total "
            "FROM compras c "
            "JOIN proveedores p ON c.id_proveedor = p.id_proveedor "
            "LEFT JOIN compras_detalle cd ON c.id_compras = cd.id_compra "
            "GROUP BY c.id_compras "
            "ORDER BY c.id_compras DESC";

        if (mysql_query(conectar, q.c_str()) != 0) {
            cout << "Error: " << mysql_error(conectar) << "\n";
            return;
        }

        MYSQL_RES* res = mysql_store_result(conectar);
        MYSQL_ROW  fila;

        cout << "\n===== COMPRAS REGISTRADAS =====\n";
        cout << left
            << setw(5)  << "ID"
            << setw(8)  << "Orden"
            << setw(13) << "Fecha"
            << setw(22) << "Proveedor"
            << "Total\n";
        cout << string(60, '-') << "\n";

        while ((fila = mysql_fetch_row(res))) {
            cout << left
                << setw(5)  << fila[0]
                << setw(8)  << fila[1]
                << setw(13) << fila[2]
                << setw(22) << string(fila[3]).substr(0, 20)
                << "Q" << (fila[4] ? fila[4] : "0.00") << "\n";
        }

        mysql_free_result(res);
        system("pause");
    }

    // ================= VER DETALLE DE UNA COMPRA =================

    static void consultarDetalle(MYSQL* conectar, int id_c) {
        string q =
            "SELECT p.idproductos, p.productos, cd.cantidad, "
            "cd.precio_costo_unitario, "
            "(cd.cantidad * cd.precio_costo_unitario) AS subtotal "
            "FROM compras_detalle cd "
            "JOIN productos p ON cd.id_producto = p.idproductos "
            "WHERE cd.id_compra = " + to_string(id_c);

        if (mysql_query(conectar, q.c_str()) != 0) {
            cout << "Error: " << mysql_error(conectar) << "\n";
            return;
        }

        MYSQL_RES* res = mysql_store_result(conectar);
        MYSQL_ROW  fila;

        cout << "\n--- Detalle de Compra ID: " << id_c << " ---\n";
        cout << left
            << setw(6)  << "COD"
            << setw(22) << "Producto"
            << setw(7)  << "Cant"
            << setw(10) << "Costo"
            << "Subtotal\n";
        cout << string(55, '-') << "\n";

        while ((fila = mysql_fetch_row(res))) {
            cout << left
                << setw(6)  << fila[0]
                << setw(22) << string(fila[1]).substr(0, 20)
                << setw(7)  << fila[2]
                << setw(10) << (string("Q") + fila[3])
                << "Q" << fila[4] << "\n";
        }

        mysql_free_result(res);
        system("pause");
    }

    // ================= ELIMINAR COMPRA =================

    static void eliminarCompra(MYSQL* conectar, int id_c) {
        mysql_query(conectar, "START TRANSACTION");

        // primero revertir stock
        string q_stock =
            "UPDATE productos p "
            "JOIN compras_detalle cd ON p.idproductos = cd.id_producto "
            "SET p.existencia = p.existencia - cd.cantidad "
            "WHERE cd.id_compra = " + to_string(id_c);

        if (mysql_query(conectar, q_stock.c_str()) != 0) {
            cout << "Error al revertir stock: " << mysql_error(conectar) << "\n";
            mysql_query(conectar, "ROLLBACK");
            system("pause");
            return;
        }

        string q_det = "DELETE FROM compras_detalle WHERE id_compra = " + to_string(id_c);
        string q_mae = "DELETE FROM compras WHERE id_compras = " + to_string(id_c);

        if (mysql_query(conectar, q_det.c_str()) != 0 ||
            mysql_query(conectar, q_mae.c_str()) != 0) {
            cout << "Error al eliminar: " << mysql_error(conectar) << "\n";
            mysql_query(conectar, "ROLLBACK");
            system("pause");
            return;
        }

        mysql_query(conectar, "COMMIT");
        cout << "\nCompra eliminada y stock revertido exitosamente.\n";
        system("pause");
    }
};
