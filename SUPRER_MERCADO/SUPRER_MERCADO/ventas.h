#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <mysql.h>
#include <ctime>
#include <cstdlib>
#include <iomanip>
#include <sstream>
#include <fstream>
#include "validaciones.h"

using namespace std;

// ================= FECHA =================

inline string obtener_fecha() {
    time_t tiempo = time(0);
    tm hoy;
    localtime_s(&hoy, &tiempo);
    char fecha[11];
    strftime(fecha, sizeof(fecha), "%Y-%m-%d", &hoy);
    return fecha;
}

// ================= FECHA Y HORA =================

inline string obtener_fecha_hora() {
    time_t tiempo = time(0);
    tm hoy;
    localtime_s(&hoy, &tiempo);
    char fechaHora[20];
    strftime(fechaHora, sizeof(fechaHora), "%Y-%m-%d %H:%M:%S", &hoy);
    return fechaHora;
}

// ================= GENERAR NUMERO FACTURA =================

inline int generar_factura() { return rand() % 9000 + 1000; }

// ================= GENERAR SERIE =================

inline string generar_serie() { return "A"; }

// ================= PROXIMO ID VENTA =================

inline int obtener_proximo_id_venta(MYSQL* conectar) {
    mysql_query(conectar, "SELECT IFNULL(MAX(id_ventas), 0) + 1 FROM ventas");
    MYSQL_RES* r = mysql_store_result(conectar);
    MYSQL_ROW f = mysql_fetch_row(r);
    int id = atoi(f[0]);
    mysql_free_result(r);
    return id;
}

// ================= PROXIMO ID DETALLE =================

inline long long obtener_proximo_id_detalle(MYSQL* conectar) {
    mysql_query(conectar, "SELECT IFNULL(MAX(id_venta_detalle), 0) + 1 FROM venta_detalle");
    MYSQL_RES* r = mysql_store_result(conectar);
    MYSQL_ROW f = mysql_fetch_row(r);
    long long id = atoll(f[0]);
    mysql_free_result(r);
    return id;
}

// ================= STRUCTS =================

struct DetalleVenta {
    int id_producto = 0;
    string nombre_producto = "";
    int cantidad = 0;
    double precio = 0.0;
    double subtotal = 0.0;
};

struct Venta {
    int id_venta = 0;
    int no_factura = 0;
    string serie = "";
    string fecha_factura = "";
    int id_cliente = 0;
    string nombre_cliente = "";
    string nit_cliente = "";
    int id_empleado = 0;
    string nombre_empleado = "";
    string fecha_ingreso = "";
    double total = 0.0;
};

// ================= CONSTRUIR LINEAS FACTURA =================

inline vector<string> construir_factura(Venta venta, vector<DetalleVenta> detalle) {

    vector<string> lineas;

    const int ANCHO = 54;

    auto centrar = [&](string texto) -> string {
        int espacios = (ANCHO - 2 - (int)texto.length()) / 2;
        if (espacios < 0) espacios = 0;
        string linea = "| ";
        for (int i = 0; i < espacios; i++) linea += " ";
        linea += texto;
        int restantes = ANCHO - 2 - espacios - (int)texto.length();
        for (int i = 0; i < restantes; i++) linea += " ";
        linea += " |";
        return linea;
        };

    auto separador = [&]() -> string {
        string s = "+";
        for (int i = 0; i < ANCHO; i++) s += "-";
        s += "+";
        return s;
        };

    auto fila_tabla = [&](string cod, string prod, string cant, string precio, string sub) -> string {
        if (prod.length() > 17) prod = prod.substr(0, 17);
        ostringstream ss;
        ss << "| "
            << left << setw(5) << cod
            << left << setw(18) << prod
            << right << setw(5) << cant
            << right << setw(10) << precio
            << right << setw(10) << sub
            << " |";
        return ss.str();
        };

    ostringstream num_fac;
    num_fac << venta.serie << "-" << setw(4) << setfill('0') << venta.no_factura;

    ostringstream total_str;
    total_str << "Q" << fixed << setprecision(2) << venta.total;

    lineas.push_back(separador());
    lineas.push_back(centrar(""));
    lineas.push_back(centrar("SUPERMERCADO PROFIN"));
    lineas.push_back(centrar("Factura de Venta"));
    lineas.push_back(centrar(""));
    lineas.push_back(separador());
    lineas.push_back(centrar("Factura: " + num_fac.str() + "   Fecha: " + venta.fecha_factura));
    lineas.push_back(centrar("Cliente: " + venta.nombre_cliente));
    lineas.push_back(centrar("NIT    : " + venta.nit_cliente));
    lineas.push_back(centrar("Cajero : " + venta.nombre_empleado));
    lineas.push_back(centrar(""));
    lineas.push_back(separador());
    lineas.push_back(fila_tabla("COD", "PRODUCTO", "CANT", "PRECIO", "SUBTOTAL"));
    lineas.push_back(separador());

    for (int i = 0; i < (int)detalle.size(); i++) {
        ostringstream precio_str, sub_str;
        precio_str << "Q" << fixed << setprecision(2) << detalle[i].precio;
        sub_str << "Q" << fixed << setprecision(2) << detalle[i].subtotal;

        lineas.push_back(fila_tabla(
            to_string(detalle[i].id_producto),
            detalle[i].nombre_producto,
            to_string(detalle[i].cantidad),
            precio_str.str(),
            sub_str.str()
        ));
    }

    lineas.push_back(separador());
    lineas.push_back(centrar("TOTAL:  " + total_str.str()));
    lineas.push_back(centrar(""));
    lineas.push_back(separador());
    lineas.push_back("");
    lineas.push_back("        ** Gracias por su compra **");
    lineas.push_back("           Vuelva pronto");
    lineas.push_back("");

    return lineas;
}

// ================= IMPRIMIR FACTURA EN CONSOLA =================

inline void imprimir_factura_consola(Venta venta, vector<DetalleVenta> detalle) {

    system("cls");

    vector<string> lineas = construir_factura(venta, detalle);

    for (int i = 0; i < (int)lineas.size(); i++) {
        cout << lineas[i] << "\n";
    }
}

// ================= GUARDAR FACTURA EN TXT Y ABRIR NOTEPAD =================

inline void guardar_e_imprimir(Venta venta, vector<DetalleVenta> detalle) {

    string nombre_archivo = "factura_"
        + venta.serie
        + "-"
        + to_string(venta.no_factura)
        + ".txt";

    ofstream archivo(nombre_archivo);

    if (!archivo.is_open()) {
        cout << "\nERROR: No se pudo crear el archivo.\n";
        return;
    }

    vector<string> lineas = construir_factura(venta, detalle);

    for (int i = 0; i < (int)lineas.size(); i++) {
        archivo << lineas[i] << "\n";
    }

    archivo.close();

    cout << "\nArchivo guardado: " << nombre_archivo << "\n";
    cout << "Abriendo Bloc de Notas...\n";
    cout << "Presiona Ctrl+P dentro del Bloc de Notas para imprimir.\n\n";

    string comando = "notepad " + nombre_archivo;
    system(comando.c_str());
}

// ================= NUEVA VENTA =================

inline void nueva_venta(MYSQL* conectar) {

    Venta venta;
    vector<DetalleVenta> detalle;
    char continuar;

    system("cls");

    cout << "+--------------------------------------+\n";
    cout << "|           NUEVA VENTA                |\n";
    cout << "+--------------------------------------+\n\n";

    // SELECCIONAR CAJERO
    MYSQL_RES* res_cajeros;
    MYSQL_ROW  fil_cajero;

    mysql_query(conectar,
        "SELECT id_empleados, nombres, apellidos FROM empleados ORDER BY id_empleados");
    res_cajeros = mysql_store_result(conectar);

    cout << "\n+--------------------------------------+\n";
    cout << "|         SELECCIONAR CAJERO           |\n";
    cout << "+--------------------------------------+\n";

    while ((fil_cajero = mysql_fetch_row(res_cajeros))) {
        cout << "  [" << fil_cajero[0] << "] "
            << fil_cajero[1] << " "
            << fil_cajero[2] << "\n";
    }

    mysql_free_result(res_cajeros);

    cout << "\nIngrese ID del cajero: ";
    cin >> venta.id_empleado;
    cin.ignore();

    // VERIFICAR QUE EL CAJERO EXISTE
    MYSQL_RES* res_ver;
    MYSQL_ROW  fil_ver;

    string q_ver =
        "SELECT nombres, apellidos FROM empleados WHERE id_empleados = "
        + to_string(venta.id_empleado);

    mysql_query(conectar, q_ver.c_str());
    res_ver = mysql_store_result(conectar);
    fil_ver = mysql_fetch_row(res_ver);

    if (fil_ver == NULL) {
        cout << "\nERROR: Cajero no encontrado. Operacion cancelada.\n";
        mysql_free_result(res_ver);
        system("pause");
        return;
    }

    venta.nombre_empleado = string(fil_ver[0]) + " " + string(fil_ver[1]);
    mysql_free_result(res_ver);

    cout << "\nCajero: " << venta.nombre_empleado << "\n";

    // NIT
    string nit;
    bool nit_valido = false;
    do {
        cout << "\nIngrese NIT Cliente (o CF): ";
        getline(cin, nit);
        nit_valido = validar_nit(nit);
        if (!nit_valido) cout << "NIT invalido. Intente de nuevo.\n";
    } while (!nit_valido);

    // BUSCAR CLIENTE
    MYSQL_ROW fila_cliente;
    MYSQL_RES* resultado_cliente;

    string consulta_cliente =
        "SELECT id_clientes, nombres, apellidos, nit FROM clientes WHERE nit = '" + nit + "'";

    mysql_query(conectar, consulta_cliente.c_str());
    resultado_cliente = mysql_store_result(conectar);
    fila_cliente = mysql_fetch_row(resultado_cliente);

    if (fila_cliente != NULL) {
        venta.id_cliente = atoi(fila_cliente[0]);
        venta.nombre_cliente = string(fila_cliente[1]) + " " + string(fila_cliente[2]);
        venta.nit_cliente = fila_cliente[3];
        cout << "\nCliente encontrado: " << venta.nombre_cliente << "\n";
    }
    else {
        cout << "\nCliente no encontrado. Registrando...\n\n";

        string nombres, apellidos, telefono, correo;

        do { cout << "Nombres   : "; getline(cin, nombres); } while (!validar_nombre(nombres, 50));
        do { cout << "Apellidos : "; getline(cin, apellidos); } while (!validar_nombre(apellidos, 50));
        do { cout << "Telefono  : "; getline(cin, telefono); } while (!validar_telefono_gt(telefono));
        do { cout << "Correo    : "; getline(cin, correo); } while (!validar_correo(correo));

        string insertar_cliente =
            "INSERT INTO clientes(nombres,apellidos,nit,genero,telefono,correo_electronico) VALUES('"
            + nombres + "','" + apellidos + "','" + nit + "',1,'" + telefono + "','" + correo + "')";

        if (mysql_query(conectar, insertar_cliente.c_str())) {
            cout << "\nERROR al registrar cliente: " << mysql_error(conectar) << "\n";
            mysql_free_result(resultado_cliente);
            system("pause");
            return;
        }

        venta.id_cliente = (int)mysql_insert_id(conectar);
        venta.nombre_cliente = nombres + " " + apellidos;
        venta.nit_cliente = nit;
        cout << "\nCliente registrado correctamente.\n";
    }

    mysql_free_result(resultado_cliente);

    // DATOS FACTURA
    venta.id_venta = obtener_proximo_id_venta(conectar);
    venta.fecha_factura = obtener_fecha();
    venta.fecha_ingreso = obtener_fecha_hora();
    venta.no_factura = generar_factura();
    venta.serie = generar_serie();
    venta.total = 0;

    cout << "\n+--------------------------------------+\n";
    cout << "  Fecha   : " << venta.fecha_factura << "\n";
    cout << "  Factura : " << venta.serie << "-" << venta.no_factura << "\n";
    cout << "  Cliente : " << venta.nombre_cliente << "\n";
    cout << "+--------------------------------------+\n";

    // AGREGAR PRODUCTOS
    do {
        DetalleVenta dv;

        cout << "\nCodigo Producto: ";
        cin >> dv.id_producto;
        cin.ignore();

        MYSQL_ROW fila;
        MYSQL_RES* resultado;

        string consulta =
            "SELECT p.idproductos, p.productos, m.marca, p.precio_venta, p.existencia "
            "FROM productos p INNER JOIN marcas m ON p.id_marca = m.id_marca "
            "WHERE p.idproductos = " + to_string(dv.id_producto);

        mysql_query(conectar, consulta.c_str());
        resultado = mysql_store_result(conectar);
        fila = mysql_fetch_row(resultado);

        if (fila != NULL) {
            dv.id_producto = atoi(fila[0]);
            dv.nombre_producto = fila[1];
            string marca = fila[2];
            dv.precio = atof(fila[3]);
            int existencia = atoi(fila[4]);

            cout << "\n  ID      : " << dv.id_producto << "\n";
            cout << "  Producto: " << dv.nombre_producto << "\n";
            cout << "  Marca   : " << marca << "\n";
            cout << "  Precio  : Q" << fixed << setprecision(2) << dv.precio << "\n";
            cout << "  Stock   : " << existencia << "\n";

            cout << "\nCantidad: ";
            cin >> dv.cantidad;
            cin.ignore();

            if (dv.cantidad <= 0) {
                cout << "\nERROR: Cantidad invalida.\n";
            }
            else if (dv.cantidad > existencia) {
                cout << "\nERROR: Stock insuficiente. Disponible: " << existencia << "\n";
            }
            else {
                dv.subtotal = dv.cantidad * dv.precio;
                venta.total += dv.subtotal;
                detalle.push_back(dv);
                cout << "\nProducto agregado. Total: Q"
                    << fixed << setprecision(2) << venta.total << "\n";
            }
        }
        else {
            cout << "\nERROR: Producto no encontrado.\n";
        }

        mysql_free_result(resultado);

        cout << "\nAgregar otro producto? (s/n): ";
        cin >> continuar;
        cin.ignore();

    } while (continuar == 's' || continuar == 'S');

    if (detalle.empty()) {
        cout << "\nERROR: No hay productos. Operacion cancelada.\n";
        system("pause");
        return;
    }

    // TRANSACCION
    mysql_query(conectar, "START TRANSACTION");

    // GUARDAR VENTA
    string insertar_venta =
        "INSERT INTO ventas(id_ventas,no_factura,serie,fecha_factura,id_cliente,id_empleado,fecha_ingreso) VALUES("
        + to_string(venta.id_venta) + ","
        + to_string(venta.no_factura) + ",'"
        + venta.serie + "','"
        + venta.fecha_factura + "',"
        + to_string(venta.id_cliente) + ","
        + to_string(venta.id_empleado) + ",'"
        + venta.fecha_ingreso + "')";

    if (mysql_query(conectar, insertar_venta.c_str())) {
        cout << "\nERROR al guardar venta: " << mysql_error(conectar) << "\n";
        mysql_query(conectar, "ROLLBACK");
        system("pause");
        return;
    }

    // GUARDAR DETALLE
    long long id_detalle = obtener_proximo_id_detalle(conectar);
    bool error_detalle = false;

    for (int i = 0; i < (int)detalle.size(); i++) {

        string insertar_detalle =
            "INSERT INTO venta_detalle(id_venta_detalle,id_venta,id_producto,cantidad,precio_unitario) VALUES("
            + to_string(id_detalle) + ","
            + to_string(venta.id_venta) + ","
            + to_string(detalle[i].id_producto) + ","
            + to_string(detalle[i].cantidad) + ","
            + to_string(detalle[i].precio) + ")";

        if (mysql_query(conectar, insertar_detalle.c_str())) {
            cout << "\nERROR en detalle: " << mysql_error(conectar) << "\n";
            error_detalle = true;
            break;
        }

        id_detalle++;

        string actualizar_stock =
            "UPDATE productos SET existencia = existencia - "
            + to_string(detalle[i].cantidad)
            + " WHERE idproductos = "
            + to_string(detalle[i].id_producto);

        if (mysql_query(conectar, actualizar_stock.c_str())) {
            cout << "\nERROR al actualizar stock: " << mysql_error(conectar) << "\n";
            error_detalle = true;
            break;
        }
    }

    if (error_detalle) {
        mysql_query(conectar, "ROLLBACK");
        cout << "\nVenta revertida. Intente de nuevo.\n";
        system("pause");
        return;
    }

    mysql_query(conectar, "COMMIT");

    // MOSTRAR FACTURA EN CONSOLA
    imprimir_factura_consola(venta, detalle);

    // GENERAR E IMPRIMIR AUTOMATICAMENTE
    guardar_e_imprimir(venta, detalle);
    cout << "En el Bloc de Notas presiona Ctrl+P para imprimir.\n";

    system("pause");
}

// ================= MENU VENTAS =================

inline void menu_ventas(MYSQL* conectar) {

    int opcion;

    do {
        system("cls");
        cout << "\n===== VENTAS =====\n";
        cout << "1. Nueva Venta\n";
        cout << "0. Regresar\n";
        cout << "Opcion: ";
        cin >> opcion;
        cin.ignore();

        switch (opcion) {
        case 1:
            nueva_venta(conectar);
            break;
        }
    } while (opcion != 0);
}