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
#include "ventas.h"

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
        3306,
        NULL,
        0
    );

    if (conectar) cout << "Conexion exitosa\n";
    else {
        cout << "Error en la conexion\n";
        exit(1);
    }
}

// ================= MENUS =================

void menuPuestos(MYSQL* conectar) {
    int opcion;

    do {
        cout << "\n===== PUESTOS =====\n";
        cout << "1. Crear\n2. Ver\n3. Actualizar\n4. Eliminar\n0. Regresar\n";

        opcion = leerEntero("Opcion: ");

        switch (opcion) {
        case 1: crear_puestos(conectar); break;
        case 2: mostrar_puestos(conectar); break;
        case 3: actualizar_puestos(conectar); break;
        case 4: eliminar_puestos(conectar); break;
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
        case 1: crear_empleados(conectar); break;
        case 2: mostrar_empleados(conectar); break;
        case 3: actualizar_empleados(conectar); break;
        case 4: eliminar_empleados(conectar); break;
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
        case 1:
            m.setMarca(leerTexto("Marca: "));
            m.crear();
            break;
        case 2:
            m.leer();
            break;
        case 3:
            m.setId_marca(leerEntero("ID: "));
            m.setMarca(leerTexto("Nuevo nombre: "));
            m.actualizar();
            break;
        case 4:
            m.setId_marca(leerEntero("ID: "));
            m.eliminar();
            break;
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
            p.crear();
            break;
        }
        case 2: {
            Producto p;
            p.leer();
            break;
        }
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
            p.actualizar();
            break;
        }
        case 4: {
            Producto p;
            p.setIdproductos(leerEntero("ID: "));
            p.eliminar();
            break;
        }
        }
    } while (opcion != 0);
}

// ================= PROVEEDORES (ARREGLADO) =================

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
            string nit = leerTexto("NIT: ");
            string dir = leerTexto("Direccion: ");
            string tel = leerTexto("Telefono: ");

            {
                Proveedor tempP(0, prov, nit, dir, tel);
                tempP.crear();
            }
            break;
        }

        case 2:
            p.leer();
            break;

        case 3: {
            int id = leerEntero("ID: ");
            string prov = leerTexto("Proveedor: ");
            string nit = leerTexto("NIT: ");
            string dir = leerTexto("Direccion: ");
            string tel = leerTexto("Telefono: ");

            {
                Proveedor tempP(id, prov, nit, dir, tel);
                tempP.actualizar();
            }
            break;
        }

        case 4: {
            int id = leerEntero("ID: ");
            {
                Proveedor tempP(id, "", "", "", "");
                tempP.eliminar();
            }
            break;
        }
        }

    } while (opcion != 0);
}

// ================= CLIENTES =================

void menuClientes() {

    int opcion;
    Cliente c;

    do {
        cout << "\n===== CLIENTES =====\n";
        cout << "1. Crear\n2. Ver\n3. Actualizar\n4. Eliminar\n0. Regresar\n";

        opcion = leerEntero("Opcion: ");

        switch (opcion) {

        case 1: {
            string nom = leerTexto("Nombres: ");
            string ape = leerTexto("Apellidos: ");
            string nit = leerTexto("NIT: ");
            bool gen = leerEntero("Genero (1=M 0=F): ");
            string tel = leerTexto("Telefono: ");
            string correo = leerTexto("Correo: ");

            c = Cliente(0, nom, ape, nit, gen, tel, correo, "");
            c.crear();
            break;
        }

        case 2:
            c.leer();
            break;

        case 3: {
            int id = leerEntero("ID: ");
            string nom = leerTexto("Nombres: ");
            string ape = leerTexto("Apellidos: ");
            string nit = leerTexto("NIT: ");
            bool gen = leerEntero("Genero: ");
            string tel = leerTexto("Telefono: ");
            string correo = leerTexto("Correo: ");

            c = Cliente(id, nom, ape, nit, gen, tel, correo, "");
            c.actualizar();
            break;
        }

        case 4: {
            int id = leerEntero("ID: ");
            c = Cliente(id, "", "", "", 1, "", "", "");
            c.eliminar();
            break;
        }
        }

    } while (opcion != 0);
}

// ================= MAIN =================

int main() {

    srand((unsigned int)time(0));//esta linea se agrego por tema de facturacion para no repetir el mismo numero de factura.

    MYSQL* conectar;
    conectar_bd(conectar);

    int opcion;

    do {
        cout << "\n===== SISTEMA SUPERMERCADO =====\n";
        cout << "  --- Catalogos ---\n";
        cout << "  1. Puestos\n";
        cout << "  2. Empleados\n";
        cout << "  3. Marcas\n";
        cout << "  4. Productos\n";
        cout << "  5. Proveedores\n";
        cout << "  6. Clientes\n";
        cout << "  --- Maestro-Detalle ---\n";
        cout << "  7. Ventas y Ventas Detalle\n";
        cout << "  0. Salir\n";

        opcion = leerEntero("Opcion: ");

        switch (opcion) {
        case 1: menuPuestos(conectar); break;
        case 2: menuEmpleados(conectar); break;
        case 3: menuMarcas(); break;
        case 4: menuProductos(); break;
        case 5: menuProveedores(conectar); break;
        case 6: menuClientes(); break;
        case 7: menu_ventas(conectar); break;
            
        }

    } while (opcion != 0);

    mysql_close(conectar);
    return 0;
}