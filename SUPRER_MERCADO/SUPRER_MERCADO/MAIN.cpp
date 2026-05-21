#define NOMINMAX
#include <iostream>
#include <limits>
#include <string>
#include <mysql.h>

#include "puestos.h"
#include "empleados.h"
#include "marca.h"
#include "productos.h"
#include "proveedores.h"
#include "clientes.h"
#include "VentasMaestroDetalle.h"
#include "ComprasMaestroDetalle.h"

using namespace std;

// ================= LIMPIEZA =================

void limpiarBuffer() {
    cin.clear();
    cin.ignore((numeric_limits<streamsize>::max)(), '\n');
}

string leerTexto(string mensaje) {
    string valor;
    cout << mensaje;
    getline(cin, valor);
    return valor;
}

int leerEntero(string mensaje) {
    int valor;
    cout << mensaje;
    while (!(cin >> valor)) {
        cout << "Ingrese un numero valido: ";
        limpiarBuffer();
    }
    limpiarBuffer();
    return valor;
}

double leerDecimal(string mensaje) {
    double valor;
    cout << mensaje;
    while (!(cin >> valor)) {
        cout << "Ingrese un decimal valido: ";
        limpiarBuffer();
    }
    limpiarBuffer();
    return valor;
}

// ================= BD =================

void conectar_bd(MYSQL*& conectar) {
    conectar = mysql_init(0);
    conectar = mysql_real_connect(
        conectar,
        "localhost",
        "root",
        "MySQL2026#",
        "super_mercado_profin",
        3306, NULL, 0
    );
    if (conectar) cout << "Conexion exitosa\n";
    else { cout << "Error en la conexion\n"; exit(1); }
}

// ================= MENUS ORIGINALES =================

void menuPuestos(MYSQL* conectar) {
    int opcion;
    do {
        cout << "\n===== PUESTOS =====\n";
        cout << "1. Crear\n2. Ver\n3. Actualizar\n4. Eliminar\n0. Regresar\n";
        opcion = leerEntero("Opcion: ");
        switch (opcion) {
        case 1: crear_puestos(conectar);      break;
        case 2: mostrar_puestos(conectar);    break;
        case 3: actualizar_puestos(conectar); break;
        case 4: eliminar_puestos(conectar);   break;
        }
    } while (opcion != 0);
}

void menuEmpleados(MYSQL* conectar) {
    int opcion;
    do {
        cout << "\n===== EMPLEADOS =====\n";
        cout << "1. Crear\n2. Ver\n3. Actualizar\n4. Eliminar\n0. Regresar\n";
        opcion = leerEntero("Opcion: ");
        switch (opcion) {
        case 1: crear_empleados(conectar);      break;
        case 2: mostrar_empleados(conectar);    break;
        case 3: actualizar_empleados(conectar); break;
        case 4: eliminar_empleados(conectar);   break;
        }
    } while (opcion != 0);
}

void menuMarcas() {
    int opcion;
    Marca m;
    do {
        cout << "\n===== MARCAS =====\n";
        cout << "1. Crear\n2. Ver\n3. Actualizar\n4. Eliminar\n0. Regresar\n";
        opcion = leerEntero("Opcion: ");
        switch (opcion) {
        case 1: m.setMarca(leerTexto("Marca: ")); m.crear(); break;
        case 2: m.leer(); break;
        case 3:
            m.setId_marca(leerEntero("ID: "));
            m.setMarca(leerTexto("Nuevo nombre: "));
            m.actualizar(); break;
        case 4: m.setId_marca(leerEntero("ID: ")); m.eliminar(); break;
        }
    } while (opcion != 0);
}

void menuProductos() {
    int opcion;
    do {
        cout << "\n===== PRODUCTOS =====\n";
        cout << "1. Crear\n2. Ver\n3. Actualizar\n4. Eliminar\n0. Regresar\n";
        opcion = leerEntero("Opcion: ");
        switch (opcion) {
        case 1: {
            Producto p;
            p.setProductos(leerTexto("Producto: "));
            p.setId_marca(leerEntero("Marca ID: "));
            p.setDescripcion(leerTexto("Descripcion: "));
            p.setImagen(leerTexto("Imagen: "));
            p.setPrecio_costo(leerDecimal("Costo: "));
            p.setPrecio_venta(leerDecimal("Venta: "));
            p.setExistencia(leerEntero("Existencia: "));
            p.setFecha_ingreso(leerTexto("Fecha: "));
            p.crear(); break;
        }
        case 2: { Producto p; p.leer(); break; }
        case 3: {
            Producto p;
            p.setIdproductos(leerEntero("ID: "));
            p.setProductos(leerTexto("Nuevo: "));
            p.setId_marca(leerEntero("Marca ID: "));
            p.setDescripcion(leerTexto("Desc: "));
            p.setImagen(leerTexto("Img: "));
            p.setPrecio_costo(leerDecimal("Costo: "));
            p.setPrecio_venta(leerDecimal("Venta: "));
            p.setExistencia(leerEntero("Existencia: "));
            p.setFecha_ingreso(leerTexto("Fecha: "));
            p.actualizar(); break;
        }
        case 4: { Producto p; p.setIdproductos(leerEntero("ID: ")); p.eliminar(); break; }
        }
    } while (opcion != 0);
}

void menuProveedores(MYSQL* conectar) {
    int opcion;
    Proveedor p;
    do {
        cout << "\n===== PROVEEDORES =====\n";
        cout << "1. Crear\n2. Ver\n3. Actualizar\n4. Eliminar\n0. Regresar\n";
        opcion = leerEntero("Opcion: ");
        switch (opcion) {
        case 1: {
            string prov = leerTexto("Proveedor: ");
            string nit  = leerTexto("NIT: ");
            string dir  = leerTexto("Direccion: ");
            string tel  = leerTexto("Telefono: ");
            Proveedor tempP(0, prov, nit, dir, tel);
            tempP.crear(); break;
        }
        case 2: p.leer(); break;
        case 3: {
            int id = leerEntero("ID: ");
            Proveedor tempP(id, leerTexto("Proveedor: "), leerTexto("NIT: "),
                            leerTexto("Direccion: "), leerTexto("Telefono: "));
            tempP.actualizar(); break;
        }
        case 4: {
            int id = leerEntero("ID: ");
            Proveedor tempP(id, "", "", "", "");
            tempP.eliminar(); break;
        }
        }
    } while (opcion != 0);
}

void menuClientes() {
    int opcion;
    Cliente c;
    do {
        cout << "\n===== CLIENTES =====\n";
        cout << "1. Crear\n2. Ver\n3. Actualizar\n4. Eliminar\n0. Regresar\n";
        opcion = leerEntero("Opcion: ");
        switch (opcion) {
        case 1: {
            c = Cliente(0, leerTexto("Nombres: "), leerTexto("Apellidos: "),
                        leerTexto("NIT: "), leerEntero("Genero (1=M 0=F): "),
                        leerTexto("Telefono: "), leerTexto("Correo: "), "");
            c.crear(); break;
        }
        case 2: c.leer(); break;
        case 3: {
            int id = leerEntero("ID: ");
            c = Cliente(id, leerTexto("Nombres: "), leerTexto("Apellidos: "),
                        leerTexto("NIT: "), leerEntero("Genero: "),
                        leerTexto("Telefono: "), leerTexto("Correo: "), "");
            c.actualizar(); break;
        }
        case 4: {
            int id = leerEntero("ID: ");
            c = Cliente(id, "", "", "", 1, "", "", "");
            c.eliminar(); break;
        }
        }
    } while (opcion != 0);
}

// ================= MENU VENTAS MAESTRO-DETALLE =================

void menuVentas() {
    int opcion;
    do {
        cout << "\n===== VENTAS =====\n";
        cout << "1. Nueva Venta\n";
        cout << "2. Ver todas las Ventas\n";
        cout << "3. Ver Detalle de una Venta\n";
        cout << "4. Eliminar Venta\n";
        cout << "0. Regresar\n";
        opcion = leerEntero("Opcion: ");

        switch (opcion) {

        case 1: {
            // ---- ENCABEZADO (MAESTRO) ----
            VentasMaestroDetalle venta;

            cout << "\n--- NUEVA VENTA ---\n";
            venta.SetIdVenta(leerEntero("ID de la venta     : "));
            venta.SetNoFactura(leerEntero("No. de factura     : "));

            string serieStr = leerTexto("Serie (letra, ej. A): ");
            venta.SetSerie(serieStr.empty() ? 'A' : serieStr[0]);

            string fechaF = leerTexto("Fecha factura (YYYY-MM-DD, Enter=hoy): ");
            if (!fechaF.empty()) venta.SetFechaFactura(fechaF);

            int idCli = leerEntero("ID del cliente     : ");
            string nomCli = leerTexto("Nombre del cliente : ");
            venta.SetCliente(idCli, nomCli);

            int idEmp = leerEntero("ID del empleado    : ");
            string nomEmp = leerTexto("Nombre del empleado: ");
            venta.SetEmpleado(idEmp, nomEmp);

            // ---- DETALLE ----
            int opDet;
            do {
                cout << "\n--- DETALLE DE VENTA ---\n";
                cout << "1. Agregar producto\n";
                cout << "2. Modificar linea\n";
                cout << "3. Eliminar linea\n";
                cout << "4. Ver resumen\n";
                cout << "5. Guardar venta\n";
                cout << "0. Cancelar\n";
                opDet = leerEntero("Opcion: ");

                switch (opDet) {
                case 1: {
                    int    idProd  = leerEntero("ID producto   : ");
                    string nomProd = leerTexto("Nombre producto: ");
                    int    cant    = leerEntero("Cantidad       : ");
                    double precio  = leerDecimal("Precio unitario: ");
                    venta.AgregarDetalle(idProd, nomProd, cant, precio);
                    cout << "Producto agregado. Total actual: Q"
                         << fixed << setprecision(2) << venta.GetTotal() << "\n";
                    break;
                }
                case 2: {
                    venta.MostrarResumen();
                    int idx   = leerEntero("Numero de linea a modificar: ") - 1;
                    int cant  = leerEntero("Nueva cantidad : ");
                    double pr = leerDecimal("Nuevo precio   : ");
                    if (venta.ModificarDetalle(idx, cant, pr))
                        cout << "Linea modificada.\n";
                    else
                        cout << "Indice invalido.\n";
                    break;
                }
                case 3: {
                    venta.MostrarResumen();
                    int idx = leerEntero("Numero de linea a eliminar: ") - 1;
                    if (venta.EliminarDetalle(idx))
                        cout << "Linea eliminada.\n";
                    else
                        cout << "Indice invalido.\n";
                    break;
                }
                case 4:
                    venta.MostrarResumen();
                    break;
                case 5:
                    venta.MostrarResumen();
                    venta.GuardarEnBD();
                    opDet = 0; // salir del submenú
                    break;
                }
            } while (opDet != 0);
            break;
        }

        case 2:
            VentasMaestroDetalle::ConsultarVentas();
            break;

        case 3: {
            int id = leerEntero("ID de la venta: ");
            VentasMaestroDetalle::ConsultarDetalleVenta(id);
            break;
        }

        case 4: {
            int id = leerEntero("ID de la venta a eliminar: ");
            VentasMaestroDetalle::EliminarVenta(id);
            break;
        }
        }
    } while (opcion != 0);
}

// ================= MENU COMPRAS MAESTRO-DETALLE =================

void menuCompras() {
    int opcion;
    do {
        cout << "\n===== COMPRAS =====\n";
        cout << "1. Nueva Compra\n";
        cout << "2. Ver todas las Compras\n";
        cout << "3. Ver Detalle de una Compra\n";
        cout << "4. Eliminar Compra\n";
        cout << "0. Regresar\n";
        opcion = leerEntero("Opcion: ");

        switch (opcion) {

        case 1: {
            // ---- ENCABEZADO (MAESTRO) ----
            ComprasMaestroDetalle compra;

            cout << "\n--- NUEVA COMPRA ---\n";
            compra.SetIdCompra(leerEntero("ID de la compra     : "));
            compra.SetNumeroOrden(leerEntero("No. de orden        : "));

            string fechaO = leerTexto("Fecha orden (YYYY-MM-DD, Enter=hoy): ");
            if (!fechaO.empty()) compra.SetFechaOrden(fechaO);

            int idProv = leerEntero("ID del proveedor    : ");
            string nomProv = leerTexto("Nombre del proveedor: ");
            compra.SetProveedor(idProv, nomProv);

            // ---- DETALLE ----
            int opDet;
            do {
                cout << "\n--- DETALLE DE COMPRA ---\n";
                cout << "1. Agregar producto\n";
                cout << "2. Modificar linea\n";
                cout << "3. Eliminar linea\n";
                cout << "4. Ver resumen\n";
                cout << "5. Guardar compra\n";
                cout << "0. Cancelar\n";
                opDet = leerEntero("Opcion: ");

                switch (opDet) {
                case 1: {
                    int    idProd  = leerEntero("ID producto   : ");
                    string nomProd = leerTexto("Nombre producto: ");
                    int    cant    = leerEntero("Cantidad       : ");
                    double costo   = leerDecimal("Precio de costo: ");
                    compra.AgregarDetalle(idProd, nomProd, cant, costo);
                    cout << "Producto agregado. Total actual: Q"
                         << fixed << setprecision(2) << compra.GetTotal() << "\n";
                    break;
                }
                case 2: {
                    compra.MostrarResumen();
                    int idx   = leerEntero("Numero de linea a modificar: ") - 1;
                    int cant  = leerEntero("Nueva cantidad : ");
                    double co = leerDecimal("Nuevo costo    : ");
                    if (compra.ModificarDetalle(idx, cant, co))
                        cout << "Linea modificada.\n";
                    else
                        cout << "Indice invalido.\n";
                    break;
                }
                case 3: {
                    compra.MostrarResumen();
                    int idx = leerEntero("Numero de linea a eliminar: ") - 1;
                    if (compra.EliminarDetalle(idx))
                        cout << "Linea eliminada.\n";
                    else
                        cout << "Indice invalido.\n";
                    break;
                }
                case 4:
                    compra.MostrarResumen();
                    break;
                case 5:
                    compra.MostrarResumen();
                    compra.GuardarEnBD();
                    opDet = 0;
                    break;
                }
            } while (opDet != 0);
            break;
        }

        case 2:
            ComprasMaestroDetalle::ConsultarCompras();
            break;

        case 3: {
            int id = leerEntero("ID de la compra: ");
            ComprasMaestroDetalle::ConsultarDetalleCompra(id);
            break;
        }

        case 4: {
            int id = leerEntero("ID de la compra a eliminar: ");
            ComprasMaestroDetalle::EliminarCompra(id);
            break;
        }
        }
    } while (opcion != 0);
}

// ================= MAIN =================

int main() {

    MYSQL* conectar;
    conectar_bd(conectar);

    int opcion;

    do {
        cout << "\n===== SISTEMA SUPERMERCADO =====\n";
        cout << "1. Puestos\n";
        cout << "2. Empleados\n";
        cout << "3. Marcas\n";
        cout << "4. Productos\n";
        cout << "5. Proveedores\n";
        cout << "6. Clientes\n";
        cout << "7. Ventas\n";
        cout << "8. Compras\n";
        cout << "0. Salir\n";

        opcion = leerEntero("Opcion: ");

        switch (opcion) {
        case 1: menuPuestos(conectar);   break;
        case 2: menuEmpleados(conectar); break;
        case 3: menuMarcas();            break;
        case 4: menuProductos();         break;
        case 5: menuProveedores(conectar); break;
        case 6: menuClientes();          break;
        case 7: menuVentas();            break;
        case 8: menuCompras();           break;
        }

    } while (opcion != 0);

    mysql_close(conectar);
    return 0;
}
