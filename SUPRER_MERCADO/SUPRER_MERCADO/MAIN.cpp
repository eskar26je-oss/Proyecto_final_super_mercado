#define NOMINMAX

#include <iostream>
#include <limits>
#include <string>
#include <iomanip>
#include <ctime>
#include <mysql.h>

#include "CONEXION_BD.h"
#include "puestos.h"
#include "empleados.h"
#include "marca.h"
#include "productos.h"
#include "proveedores.h"
#include "clientes.h"
#include "validaciones.h"
#include "ComprasMaestroDetalle.h"
#include "ventas.h"

using namespace std;

// ================= UTILIDADES DE ENTRADA =================

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

// ================= ENTRADA CON VALIDACION =================

template <typename Func>
string leerTextoValidado(string mensaje, Func funcion, string error) {
    string valor;
    do {
        valor = leerTexto(mensaje);
        if (funcion(valor)) return valor;
        cout << error << "\n";
    } while (true);
}

int leerEnteroPositivo(string mensaje) {
    int valor;
    do {
        valor = leerEntero(mensaje);
        if (validar_entero_positivo(valor)) return valor;
        cout << "Ingrese un numero positivo\n";
    } while (true);
}

double leerDecimalPositivo(string mensaje) {
    double valor;
    do {
        valor = leerDecimal(mensaje);
        if (valor > 0) return valor;
        cout << "Ingrese un decimal positivo\n";
    } while (true);
}

// ================= PUESTOS =================

void menuPuestos(MYSQL* conectar) {
    int opcion;
    do {
        cout << "\n===== PUESTOS =====\n";
        cout << "1. Crear\n2. Ver\n3. Actualizar\n4. Eliminar\n0. Regresar\n";
        opcion = leerEntero("Opcion: ");
        switch (opcion) {

        case 1: {
            string puesto = leerTextoValidado(
                "Puesto: ",
                [](string t) { return validar_nombre(t, 50); },
                "Nombre invalido"
            );
            crear_puestos(conectar, puesto);
            break;
        }

        case 2:
            mostrar_puestos(conectar);
            break;

        case 3: {
            int id = leerEnteroPositivo("ID: ");
            string puesto = leerTextoValidado(
                "Nuevo puesto: ",
                [](string t) { return validar_nombre(t, 50); },
                "Nombre invalido"
            );
            actualizar_puestos(conectar, id, puesto);
            break;
        }

        case 4: {
            int id = leerEnteroPositivo("ID: ");
            eliminar_puestos(conectar, id);
            break;
        }

        }
    } while (opcion != 0);
}

// ================= EMPLEADOS =================

void menuEmpleados(MYSQL* conectar) {
    int opcion;
    do {
        cout << "\n===== EMPLEADOS =====\n";
        cout << "1. Crear\n2. Ver\n3. Actualizar\n4. Eliminar\n0. Regresar\n";
        opcion = leerEntero("Opcion: ");
        switch (opcion) {

        case 1: {
            string nombres = leerTextoValidado(
                "Nombres: ",
                [](string t) { return validar_nombre(t, 50); },
                "Nombre invalido"
            );
            string apellidos = leerTextoValidado(
                "Apellidos: ",
                [](string t) { return validar_nombre(t, 50); },
                "Apellido invalido"
            );
            string direccion = leerTextoValidado(
                "Direccion: ",
                [](string t) { return validar_direccion(t, 100); },
                "Direccion invalida"
            );
            string telefono = leerTextoValidado(
                "Telefono: ",
                validar_telefono_gt,
                "Telefono invalido"
            );
            string cui = leerTextoValidado(
                "CUI: ",
                validar_cui,
                "CUI invalido"
            );
            int genero = leerEntero("Genero (1=M 0=F): ");
            string fecha_nacimiento = leerTextoValidado(
                "Fecha nacimiento (YYYY-MM-DD): ",
                validar_fecha,
                "Fecha invalida"
            );
            mostrar_puestos(conectar);
            int id_puesto = leerEnteroPositivo("ID Puesto: ");
            string fecha_inicio = leerTextoValidado(
                "Fecha inicio labores (YYYY-MM-DD): ",
                validar_fecha,
                "Fecha invalida"
            );
            string fecha_ingreso = leerTextoValidado(
                "Fecha ingreso (YYYY-MM-DD): ",
                validar_fecha,
                "Fecha invalida"
            );
            crear_empleados(
                conectar, nombres, apellidos, direccion,
                telefono, cui, genero, fecha_nacimiento,
                id_puesto, fecha_inicio, fecha_ingreso
            );
            break;
        }

        case 2:
            mostrar_empleados(conectar);
            break;

        case 3: {
            int id = leerEnteroPositivo("ID: ");
            string nombres = leerTextoValidado(
                "Nuevo nombre: ",
                [](string t) { return validar_nombre(t, 50); },
                "Nombre invalido"
            );
            actualizar_empleados(conectar, id, nombres);
            break;
        }

        case 4: {
            int id = leerEnteroPositivo("ID: ");
            eliminar_empleados(conectar, id);
            break;
        }

        }
    } while (opcion != 0);
}

// ================= MARCAS =================

void menuMarcas() {
    int opcion;
    Marca m;
    do {
        cout << "\n===== MARCAS =====\n";
        cout << "1. Crear\n2. Ver\n3. Actualizar\n4. Eliminar\n0. Regresar\n";
        opcion = leerEntero("Opcion: ");
        switch (opcion) {

        case 1:
            m.setMarca(leerTextoValidado(
                "Marca: ",
                [](string t) { return validar_nombre(t, 50); },
                "Solo letras"
            ));
            m.crear();
            break;

        case 2:
            m.leer();
            break;

        case 3:
            m.setId_marca(leerEnteroPositivo("ID: "));
            m.setMarca(leerTextoValidado(
                "Nuevo nombre: ",
                [](string t) { return validar_nombre(t, 50); },
                "Solo letras"
            ));
            m.actualizar();
            break;

        case 4:
            m.setId_marca(leerEnteroPositivo("ID: "));
            m.eliminar();
            break;
        }
    } while (opcion != 0);
}

// ================= PRODUCTOS =================

void menuProductos() {
    int opcion;
    do {
        cout << "\n===== PRODUCTOS =====\n";
        cout << "1. Crear\n2. Ver\n3. Actualizar\n4. Eliminar\n0. Regresar\n";
        opcion = leerEntero("Opcion: ");
        switch (opcion) {

        case 1: {
            Producto p;
            p.setProductos(leerTextoValidado(
                "Producto: ",
                [](string t) { return validar_nombre(t, 50); },
                "Solo letras"
            ));
            p.setId_marca(leerEnteroPositivo("Marca ID: "));
            p.setDescripcion(leerTextoValidado(
                "Descripcion: ",
                [](string t) { return validar_descripcion(t, 200); },
                "Descripcion invalida"
            ));
            p.setImagen(leerTextoValidado(
                "Imagen: ",
                [](string t) { return validar_imagen(t, 100); },
                "Imagen invalida"
            ));
            p.setPrecio_costo(leerDecimalPositivo("Costo: "));
            p.setPrecio_venta(leerDecimalPositivo("Venta: "));
            p.setExistencia(leerEnteroPositivo("Existencia: "));
            p.setFecha_ingreso(leerTextoValidado(
                "Fecha (YYYY-MM-DD): ",
                validar_fecha,
                "Formato YYYY-MM-DD"
            ));
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
            p.setIdproductos(leerEnteroPositivo("ID: "));
            p.setProductos(leerTextoValidado(
                "Nuevo nombre: ",
                [](string t) { return validar_nombre(t, 50); },
                "Solo letras"
            ));
            p.setId_marca(leerEnteroPositivo("Marca ID: "));
            p.setDescripcion(leerTextoValidado(
                "Desc: ",
                [](string t) { return validar_descripcion(t, 200); },
                "Descripcion invalida"
            ));
            p.setImagen(leerTextoValidado(
                "Img: ",
                [](string t) { return validar_imagen(t, 100); },
                "Imagen invalida"
            ));
            p.setPrecio_costo(leerDecimalPositivo("Costo: "));
            p.setPrecio_venta(leerDecimalPositivo("Venta: "));
            p.setExistencia(leerEnteroPositivo("Existencia: "));
            p.setFecha_ingreso(leerTextoValidado(
                "Fecha (YYYY-MM-DD): ",
                validar_fecha,
                "Formato YYYY-MM-DD"
            ));
            p.actualizar();
            break;
        }

        case 4: {
            Producto p;
            p.setIdproductos(leerEnteroPositivo("ID: "));
            p.eliminar();
            break;
        }

        }
    } while (opcion != 0);
}

// ================= PROVEEDORES =================

void menuProveedores(MYSQL* conectar) {
    int opcion;
    Proveedor p;
    do {
        cout << "\n===== PROVEEDORES =====\n";
        cout << "1. Crear\n2. Ver\n3. Actualizar\n4. Eliminar\n0. Regresar\n";
        opcion = leerEntero("Opcion: ");
        switch (opcion) {

        case 1: {
            string prov = leerTextoValidado(
                "Proveedor: ",
                [](string t) { return validar_nombre(t, 50); },
                "Solo letras"
            );
            string nit = leerTextoValidado(
                "NIT: ",
                validar_nit,
                "NIT invalido"
            );
            string dir = leerTextoValidado(
                "Direccion: ",
                [](string t) { return validar_direccion(t, 100); },
                "Direccion invalida"
            );
            string tel = leerTextoValidado(
                "Telefono: ",
                validar_telefono_gt,
                "Telefono invalido"
            );
            Proveedor tempP(0, prov, nit, dir, tel);
            tempP.crear();
            break;
        }

        case 2:
            p.leer();
            break;

        case 3: {
            int id = leerEnteroPositivo("ID: ");
            string prov = leerTextoValidado(
                "Proveedor: ",
                [](string t) { return validar_nombre(t, 50); },
                "Solo letras"
            );
            string nit = leerTextoValidado(
                "NIT: ",
                validar_nit,
                "NIT invalido"
            );
            string dir = leerTextoValidado(
                "Direccion: ",
                [](string t) { return validar_direccion(t, 100); },
                "Direccion invalida"
            );
            string tel = leerTextoValidado(
                "Telefono: ",
                validar_telefono_gt,
                "Telefono invalido"
            );
            Proveedor tempP(id, prov, nit, dir, tel);
            tempP.actualizar();
            break;
        }

        case 4: {
            int id = leerEnteroPositivo("ID: ");
            Proveedor tempP(id, "", "", "", "");
            tempP.eliminar();
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
            string nom = leerTextoValidado(
                "Nombres: ",
                [](string t) { return validar_nombre(t, 50); },
                "Solo letras"
            );
            string ape = leerTextoValidado(
                "Apellidos: ",
                [](string t) { return validar_nombre(t, 50); },
                "Solo letras"
            );
            string nit = leerTextoValidado(
                "NIT: ",
                validar_nit,
                "NIT invalido"
            );
            int gen = leerEntero("Genero (1=M 0=F): ");
            string tel = leerTextoValidado(
                "Telefono: ",
                validar_telefono_gt,
                "Telefono invalido"
            );
            string correo = leerTextoValidado(
                "Correo: ",
                validar_correo,
                "Correo invalido"
            );
            c = Cliente(0, nom, ape, nit, gen, tel, correo, "");
            c.crear();
            break;
        }

        case 2:
            c.leer();
            break;

        case 3: {
            int id = leerEnteroPositivo("ID: ");
            string nom = leerTextoValidado(
                "Nombres: ",
                [](string t) { return validar_nombre(t, 50); },
                "Solo letras"
            );
            string ape = leerTextoValidado(
                "Apellidos: ",
                [](string t) { return validar_nombre(t, 50); },
                "Solo letras"
            );
            string nit = leerTextoValidado(
                "NIT: ",
                validar_nit,
                "NIT invalido"
            );
            int gen = leerEntero("Genero: ");
            string tel = leerTextoValidado(
                "Telefono: ",
                validar_telefono_gt,
                "Telefono invalido"
            );
            string correo = leerTextoValidado(
                "Correo: ",
                validar_correo,
                "Correo invalido"
            );
            c = Cliente(id, nom, ape, nit, gen, tel, correo, "");
            c.actualizar();
            break;
        }

        case 4: {
            int id = leerEnteroPositivo("ID: ");
            c = Cliente(id, "", "", "", 1, "", "", "");
            c.eliminar();
            break;
        }

        }
    } while (opcion != 0);
}

// ================= COMPRAS =================

void menuCompras(MYSQL* conectar) {
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
            ComprasMaestroDetalle compra;
            compra.nuevaCompra(conectar);
            break;
        }

        case 2:
            ComprasMaestroDetalle::consultarCompras(conectar);
            break;

        case 3: {
            int id = leerEnteroPositivo("ID de la compra: ");
            ComprasMaestroDetalle::consultarDetalle(conectar, id);
            break;
        }

        case 4: {
            int id = leerEnteroPositivo("ID compra a eliminar: ");
            ComprasMaestroDetalle::eliminarCompra(conectar, id);
            break;
        }

        }
    } while (opcion != 0);
}

// ================= MAIN =================

int main() {

    srand((unsigned int)time(0));

    ConexionBD cn;
    cn.abrir_conexion();
    MYSQL* conectar = cn.get_conexion();

    if (!conectar) {
        cout << "No se pudo establecer la conexion. Saliendo.\n";
        return 1;
    }

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
        cout << "  8. Compras\n";
        cout << "  0. Salir\n";
        opcion = leerEntero("Opcion: ");

        switch (opcion) {
        case 1: menuPuestos(conectar);     break;
        case 2: menuEmpleados(conectar);   break;
        case 3: menuMarcas();              break;
        case 4: menuProductos();           break;
        case 5: menuProveedores(conectar); break;
        case 6: menuClientes();            break;
        case 7: menu_ventas(conectar);     break;
        case 8: menuCompras(conectar);     break;
        }
    } while (opcion != 0);

    cn.cerrar_conexion();
    return 0;
}