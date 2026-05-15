#define NOMINMAX
#include <iostream>
#include <limits>
#include <string>
#include "marca.h"
#include "productos.h"

using namespace std;

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

void menuMarcas() {

    int opcion;

    do {

        cout << "\n===== MARCAS =====" << endl;
        cout << "1. Crear marca" << endl;
        cout << "2. Ver marcas" << endl;
        cout << "3. Actualizar marca" << endl;
        cout << "4. Eliminar marca" << endl;
        cout << "0. Regresar" << endl;

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

void menuProductos() {

    int opcion;

    do {

        cout << "\n===== PRODUCTOS =====" << endl;
        cout << "1. Crear producto" << endl;
        cout << "2. Ver productos" << endl;
        cout << "3. Actualizar producto" << endl;
        cout << "4. Eliminar producto" << endl;
        cout << "0. Regresar" << endl;

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
            p.setId_marca(leerEntero("Nuevo ID marca: "));
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

int main() {

    int opcion;

    do {

        cout << "\n===== SISTEMA SUPERMERCADO =====" << endl;
        cout << "1. Marcas" << endl;
        cout << "2. Productos" << endl;
        cout << "0. Salir" << endl;

        opcion = leerEntero("Opcion: ");

        switch (opcion) {

        case 1:
            menuMarcas();
            break;

        case 2:
            menuProductos();
            break;
        }

    } while (opcion != 0);

    return 0;
}