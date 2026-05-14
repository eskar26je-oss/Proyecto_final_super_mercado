#define NOMINMAX
#include <iostream>
#include <limits>
#include <string>
#include "marca.h"
#include "productos.h"

using namespace std;

// --- FUNCIONES AUXILIARES PARA VALIDACIÓN ---

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
        cout << "Error: Ingrese un numero entero valido: ";
        limpiarBuffer();
    }
    limpiarBuffer();
    return valor;
}

double leerDecimal(string mensaje) {
    double valor;
    cout << mensaje;
    while (!(cin >> valor)) {
        cout << "Error: Ingrese un valor decimal valido: ";
        limpiarBuffer();
    }
    limpiarBuffer();
    return valor;
}

// --- MENÚ DE MARCAS ---

void menuMarcas() {
    int opcion;
    do {
        cout << "\n==============================" << endl;
        cout << "       GESTION DE MARCAS" << endl;
        cout << "==============================" << endl;
        cout << "1. Crear marca" << endl;
        cout << "2. Ver todas las marcas" << endl;
        cout << "3. Actualizar marca" << endl;
        cout << "4. Eliminar marca" << endl;
        cout << "0. Regresar al menu principal" << endl;

        opcion = leerEntero("Seleccione una opcion: ");

        switch (opcion) {
        case 1: {
            Marca m;
            m.setMarca(leerTexto("Nombre de la nueva marca: "));
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
            m.setId_marca(leerEntero("Ingrese el ID de la marca a modificar: "));
            m.setMarca(leerTexto("Ingrese el nuevo nombre para la marca: "));
            m.actualizar();
            break;
        }
        case 4: {
            Marca m;
            m.setId_marca(leerEntero("Ingrese el ID de la marca a eliminar: "));
            m.eliminar();
            break;
        }
        case 0:
            break;
        default:
            cout << "Opcion no valida." << endl;
        }
    } while (opcion != 0);
}

// --- MENÚ DE PRODUCTOS ---

void menuProductos() {
    int opcion;
    do {
        cout << "\n==============================" << endl;
        cout << "      GESTION DE PRODUCTOS" << endl;
        cout << "==============================" << endl;
        cout << "1. Registrar producto" << endl;
        cout << "2. Ver inventario" << endl;
        cout << "3. Actualizar producto" << endl;
        cout << "4. Eliminar producto" << endl;
        cout << "0. Regresar al menu principal" << endl;

        opcion = leerEntero("Seleccione una opcion: ");

        switch (opcion) {
        case 1: {
            Producto p;
            p.setProductos(leerTexto("Nombre del producto: "));
            p.setId_marca(leerEntero("ID de la Marca (debe existir): "));
            p.setDescripcion(leerTexto("Descripcion: "));
            p.setImagen(leerTexto("Nombre de imagen o URL: "));
            p.setPrecio_costo(leerDecimal("Precio Costo: "));
            p.setPrecio_venta(leerDecimal("Precio Venta: "));
            p.setExistencia(leerEntero("Cantidad en existencia: "));
            p.setFecha_ingreso(leerTexto("Fecha (AAAA-MM-DD HH:MM:SS) o dejar vacio: "));
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
            p.setIdproductos(leerEntero("Ingrese el ID del producto a actualizar: "));
            p.setProductos(leerTexto("Nuevo nombre: "));
            p.setId_marca(leerEntero("Nuevo ID de Marca: "));
            p.setDescripcion(leerTexto("Nueva descripcion: "));
            p.setImagen(leerTexto("Nueva imagen: "));
            p.setPrecio_costo(leerDecimal("Nuevo precio costo: "));
            p.setPrecio_venta(leerDecimal("Nuevo precio venta: "));
            p.setExistencia(leerEntero("Nueva existencia: "));
            p.setFecha_ingreso(leerTexto("Nueva fecha: "));
            p.actualizar();
            break;
        }
        case 4: {
            Producto p;
            p.setIdproductos(leerEntero("Ingrese el ID del producto a eliminar: "));
            p.eliminar();
            break;
        }
        case 0:
            break;
        default:
            cout << "Opcion no valida." << endl;
        }
    } while (opcion != 0);
}

// --- MAIN PRINCIPAL ---

int main() {
    int opcion;
    do {
        cout << "\n**********************************" << endl;
        cout << "     SISTEMA DE SUPERMERCADO" << endl;
        cout << "**********************************" << endl;
        cout << "1. Ir a Marcas" << endl;
        cout << "2. Ir a Productos" << endl;
        cout << "0. Salir del programa" << endl;

        opcion = leerEntero("Seleccione una opcion: ");

        switch (opcion) {
        case 1:
            menuMarcas();
            break;
        case 2:
            menuProductos();
            break;
        case 0:
            cout << "Cerrando sistema..." << endl;
            break;
        default:
            cout << "Opcion no valida." << endl;
        }
    } while (opcion != 0);

    return 0;
}