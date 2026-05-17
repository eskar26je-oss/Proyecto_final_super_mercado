#define NOMINMAX
#include <iostream>
#include <limits>
#include <string>
#include <mysql.h>

#include "puestos.h"
#include "empleados.h"
#include "marca.h"
#include "productos.h"

using namespace std;

// ================= LIMPIEZA Y LECTURA =================

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

// ================= CONEXION BD =================

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

    if (conectar) {
        cout << "Conexion exitosa\n";
    }
    else {
        cout << "Error en la conexion\n";
        exit(1);
    }
}

// ================= MENU PUESTOS =================

void menuPuestos(MYSQL* conectar) {

    int opcion;

    do {
        cout << "\n===== PUESTOS =====\n";
        cout << "1. Crear puesto\n";
        cout << "2. Ver puestos\n";
        cout << "3. Actualizar puesto\n";
        cout << "4. Eliminar puesto\n";
        cout << "0. Regresar\n";

        opcion = leerEntero("Opcion: ");

        switch (opcion) {
        case 1: crear_puestos(conectar); break;
        case 2: mostrar_puestos(conectar); break;
        case 3: actualizar_puestos(conectar); break;
        case 4: eliminar_puestos(conectar); break;
        }
    } while (opcion != 0);
}

// ================= MENU EMPLEADOS =================

void menuEmpleados(MYSQL* conectar) {

    int opcion;

    do {
        cout << "\n===== EMPLEADOS =====\n";
        cout << "1. Crear empleado\n";
        cout << "2. Ver empleados\n";
        cout << "3. Actualizar empleado\n";
        cout << "4. Eliminar empleado\n";
        cout << "0. Regresar\n";

        opcion = leerEntero("Opcion: ");

        switch (opcion) {
        case 1: crear_empleados(conectar); break;
        case 2: mostrar_empleados(conectar); break;
        case 3: actualizar_empleados(conectar); break;
        case 4: eliminar_empleados(conectar); break;
        }
    } while (opcion != 0);
}

// ================= MENU MARCAS =================

void menuMarcas() {

    int opcion;

    do {
        cout << "\n===== MARCAS =====\n";
        cout << "1. Crear marca\n";
        cout << "2. Ver marcas\n";
        cout << "3. Actualizar marca\n";
        cout << "4. Eliminar marca\n";
        cout << "0. Regresar\n";

        opcion = leerEntero("Opcion: ");

        switch (opcion) {
        case 1: {
            Marca m;
            m.setMarca(leerTexto("Nombre marca: "));
            m.crear();
            break;
        }
        case 2: {
            Marca m;
            m.leer();
            break;
        }
        case 3: {
            Marca m;
            m.setId_marca(leerEntero("ID marca: "));
            m.setMarca(leerTexto("Nuevo nombre: "));
            m.actualizar();
            break;
        }
        case 4: {
            Marca m;
            m.setId_marca(leerEntero("ID marca: "));
            m.eliminar();
            break;
        }
        }

    } while (opcion != 0);
}

// ================= MENU PRODUCTOS =================

void menuProductos() {

    int opcion;

    do {
        cout << "\n===== PRODUCTOS =====\n";
        cout << "1. Crear producto\n";
        cout << "2. Ver productos\n";
        cout << "3. Actualizar producto\n";
        cout << "4. Eliminar producto\n";
        cout << "0. Regresar\n";

        opcion = leerEntero("Opcion: ");

        switch (opcion) {
        case 1: {
            Producto p;
            p.setProductos(leerTexto("Nombre producto: "));
            p.setId_marca(leerEntero("ID marca: "));
            p.setDescripcion(leerTexto("Descripcion: "));
            p.setImagen(leerTexto("Imagen: "));
            p.setPrecio_costo(leerDecimal("Precio costo: "));
            p.setPrecio_venta(leerDecimal("Precio venta: "));
            p.setExistencia(leerEntero("Existencia: "));
            p.setFecha_ingreso(leerTexto("Fecha YYYY-MM-DD: "));
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
            p.setIdproductos(leerEntero("ID producto: "));
            p.setProductos(leerTexto("Nuevo producto: "));
            p.setId_marca(leerEntero("ID marca: "));
            p.setDescripcion(leerTexto("Descripcion: "));
            p.setImagen(leerTexto("Imagen: "));
            p.setPrecio_costo(leerDecimal("Precio costo: "));
            p.setPrecio_venta(leerDecimal("Precio venta: "));
            p.setExistencia(leerEntero("Existencia: "));
            p.setFecha_ingreso(leerTexto("Fecha: "));
            p.actualizar();
            break;
        }
        case 4: {
            Producto p;
            p.setIdproductos(leerEntero("ID producto: "));
            p.eliminar();
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
        cout << "0. Salir\n";

        opcion = leerEntero("Opcion: ");

        switch (opcion) {
        case 1: menuPuestos(conectar); break;
        case 2: menuEmpleados(conectar); break;
        case 3: menuMarcas(); break;
        case 4: menuProductos(); break;
        }

    } while (opcion != 0);

    mysql_close(conectar);

    return 0;
}