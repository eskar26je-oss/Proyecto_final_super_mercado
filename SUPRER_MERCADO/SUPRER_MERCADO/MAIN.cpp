#define NOMINMAX

#include <iostream>
#include <limits>
#include <string>
#include <mysql.h>
#include "CONEXION_BD.h"
#include "puestos.h"
#include "empleados.h"
#include "marca.h"
#include "productos.h"
#include "proveedores.h"
#include "clientes.h"
#include "validaciones.h"

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

int leerGenero(string mensaje) {
    string entrada;
    int valor;

    do {
        
        cout << mensaje;
        getline(cin, entrada);

        
        if (!entrada.empty() && (entrada == "0" || entrada == "1")) {
            valor = stoi(entrada); 
            if (validar_genero(valor)) {
                return valor;
            }
        }

        
        cout << "Ingrese un numero valido (1=M 0=F)\n";

    } while (true);
}

// ================= VALIDACIONES =================

template <typename Func>
string leerTextoValidado(
    string mensaje,
    Func funcion,
    string error
) {

    string valor;

    do {

        valor = leerTexto(mensaje);

        if (funcion(valor)) {

            return valor;
        }

        cout << error << endl;

    } while (true);
}

int leerEnteroPositivo(string mensaje) {

    int valor;

    do {

        valor = leerEntero(mensaje);

        if (validar_entero_positivo(valor)) {

            return valor;
        }

        cout << "Ingrese un numero positivo\n";

    } while (true);
}

double leerDecimalPositivo(string mensaje) {

    double valor;

    do {

        valor = leerDecimal(mensaje);

        if (valor > 0) {

            return valor;
        }

        cout << "Ingrese un decimal positivo\n";

    } while (true);
}

// ================= BD =================

void conectar_bd(MYSQL*& conectar) {

    conectar = mysql_init(0);

    conectar = mysql_real_connect(
        conectar,
        "localhost",
        "root",
        "root08",
        "super_mercado",
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
                [](string t) {
                    return validar_nombre(t, 50);
                },
                "Nombre invalido, debe tener entre 2 y 50 caracteres(letras) "
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
                [](string t) {
                    return validar_nombre(t, 50);
                },
                "Nombre invalido, debe tener entre 2 y 50 caracteres(letras) "
            );

            actualizar_puestos(
                conectar,
                id,
                puesto
            );

            break;
        }

        case 4: {

            int id = leerEnteroPositivo("ID: ");

            eliminar_puestos(
                conectar,
                id
            );

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
                [](string t) {
                    return validar_nombre(t, 50);
                },
                "Nombre invalido, debe tener entre 2 y 50 caracteres(letras) "
            );

            string apellidos = leerTextoValidado(
                "Apellidos: ",
                [](string t) {
                    return validar_nombre(t, 50);
                },
                "Apellido invalido, debe tener entre 2 y 50 caracteres(letras) "
            );

            string direccion = leerTextoValidado(
                "Direccion: ",
                [](string t) {
                    return validar_direccion(t, 100);
                },
                "Direccion invalida, por favor ingrese una direccion valida "
            );

            string telefono = leerTextoValidado(
                "Telefono: ",
                validar_telefono_gt,
                "Telefono invalido, debe tener 8 caracteres numericos "
            );

            string cui = leerTextoValidado(
                "CUI: ",
                validar_cui,
                "CUI invalido, debe tener 13 caracteres numericos "
            );

            int genero = leerGenero(
                "Genero (1=M 0=F): "
            );

            string fecha_nacimiento = leerTextoValidado(
                "Fecha nacimiento: ",
                validar_fecha,
                "Fecha invalida, debe tener el formato AAAA-MM-DD"
            );

            mostrar_puestos(conectar);

            int id_puesto = leerEnteroPositivo(
                "ID Puesto: "
            );

            string fecha_inicio = leerTextoValidado(
                "Fecha inicio labores: ",
                validar_fecha,
                "Fecha invalida, debe tener el formato AAAA-MM-DD"
            );

            string fecha_ingreso = leerTextoValidado(
                "Fecha ingreso: ",
                validar_fecha,
                "Fecha invalida, debe tener el formato AAAA-MM-DD"
            );

            crear_empleados(
                conectar,
                nombres,
                apellidos,
                direccion,
                telefono,
                cui,
                genero,
                fecha_nacimiento,
                id_puesto,
                fecha_inicio,
                fecha_ingreso
            );

            break;
        }

        case 2:

            mostrar_empleados(conectar);
            break;

        case 3: {
            mostrar_empleados(conectar);

            int id = leerEnteroPositivo("Ingrese el ID del empleado a modificar: ");
            int op_sub;

            do {
                cout << "\n--- Que campo desea modificar? ---\n";
                cout << "1. Nombres\n";
                cout << "2. Apellidos\n";
                cout << "3. Direccion\n";
                cout << "4. Telefono\n";
                cout << "5. CUI\n";
                cout << "6. Genero\n";
                cout << "7. Fecha de Nacimiento\n";
                cout << "8. ID Puesto\n";
                cout << "9. Fecha Inicio de Labores\n";
                cout << "0. Regresar al menu anterior\n";

                op_sub = leerEntero("Seleccione una opcion: ");
                limpiarBuffer(); 

                string consulta = "";
                string campo = "";
                string nuevo_valor = "";
                bool es_texto = true;

                switch (op_sub) {
                case 1:
                    nuevo_valor = leerTextoValidado("Nuevos Nombres: ", [](string t) { return validar_nombre(t, 50); }, "Nombre invalido");
                    campo = "nombres";
                    break;
                case 2:
                    nuevo_valor = leerTextoValidado("Nuevos Apellidos: ", [](string t) { return validar_nombre(t, 50); }, "Apellido invalido");
                    campo = "apellidos";
                    break;
                case 3:
                    nuevo_valor = leerTextoValidado("Nueva Direccion: ", [](string t) { return validar_direccion(t, 100); }, "Direccion invalida");
                    campo = "direccion";
                    break;
                case 4:
                    nuevo_valor = leerTextoValidado("Nuevo Telefono: ", validar_telefono_gt, "Telefono invalido");
                    campo = "telefono";
                    break;
                case 5:
                    nuevo_valor = leerTextoValidado("Nuevo CUI: ", validar_cui, "CUI invalido");
                    campo = "cui";
                    break;
                case 6: {
                    int gen = leerGenero("Nuevo Genero (1=M 0=F): ");
                    nuevo_valor = to_string(gen);
                    campo = "genero";
                    es_texto = false;
                    break;
                }
                case 7:
                    nuevo_valor = leerTextoValidado("Nueva Fecha Nacimiento (YYYY-MM-DD): ", validar_fecha, "Fecha invalida");
                    campo = "fecha_nacimiento";
                    break;
                case 8: {
                    mostrar_puestos(conectar);
                    int puesto = leerEnteroPositivo("Nuevo ID Puesto: ");
                    nuevo_valor = to_string(puesto);
                    campo = "id_puesto";
                    es_texto = false;
                    break;
                }
                case 9:
                    nuevo_valor = leerTextoValidado("Nueva Fecha Inicio Labores (YYYY-MM-DD): ", validar_fecha, "Fecha invalida");
                    campo = "fecha_inicio_de_labores";
                    break;
                case 0:
                    cout << "Regresando...\n";
                    break;
                default:
                    cout << "Opcion no valida.\n";
                    break;
                }

                
                if (op_sub >= 1 && op_sub <= 9) {
                    if (es_texto) {
                        consulta = "UPDATE empleados SET " + campo + " = '" + nuevo_valor + "' WHERE id_empleados = " + to_string(id);
                    }
                    else {
                        consulta = "UPDATE empleados SET " + campo + " = " + nuevo_valor + " WHERE id_empleados = " + to_string(id);
                    }

                    
                    int estado = mysql_query(conectar, consulta.c_str());

                    if (estado == 0) {
                        cout << "Campo " << campo << " actualizado exitosamente\n";
                    }
                    else {
                        cout << "Error al actualizar en la BD: " << mysql_error(conectar) << endl;
                    }
                }

            } while (op_sub != 0);

            break;
        }

        case 4: {

            int id = leerEnteroPositivo("ID: ");

            eliminar_empleados(
                conectar,
                id
            );

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

            m.setMarca(
                leerTextoValidado(
                    "Marca: ",
                    [](string t) {
                        return validar_nombre(t, 50);
                    },
                    "Solo se permiten letras, entre 2 y 50 caracteres"
                )
            );

            m.crear();
            break;

        case 2:

            m.leer();
            break;

        case 3:

            m.setId_marca(leerEnteroPositivo("ID: "));

            m.setMarca(
                leerTextoValidado(
                    "Nuevo nombre: ",
                    [](string t) {
                        return validar_nombre(t, 50);
                    },
                    "Solo se permiten letras, entre 2 y 50 caracteres"
                )
            );

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

            p.setProductos(
                leerTextoValidado(
                    "Producto: ",
                    [](string t) {
                        return validar_nombre(t, 50);
                    },
                    "Solo se permiten letras, entre 2 y 50 caracteres"
                )
            );

            p.setId_marca(
                leerEnteroPositivo("Marca ID: ")
            );

            p.setDescripcion(
                leerTextoValidado(
                    "Descripcion: ",
                    [](string t) {
                        return validar_descripcion(t, 200);
                    },
                    "Descripcion invalida, por favor ingrese una descripcion valida"
                )
            );

            p.setImagen(
                leerTextoValidado(
                    "Imagen: ",
                    [](string t) {
                        return validar_imagen(t, 100);
                    },
                    "Imagen invalida, por favor ingrese una imagen valida"
                )
            );

            p.setPrecio_costo(
                leerDecimalPositivo("Costo: ")
            );

            p.setPrecio_venta(
                leerDecimalPositivo("Venta: ")
            );

            p.setExistencia(
                leerEnteroPositivo("Existencia: ")
            );

            p.setFecha_ingreso(
                leerTextoValidado(
                    "Fecha: ",
                    validar_fecha,
                    "Formato YYYY-MM-DD"
                )
            );

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

            p.setIdproductos(
                leerEnteroPositivo("ID: ")
            );

            p.setProductos(
                leerTextoValidado(
                    "Nuevo: ",
                    [](string t) {
                        return validar_nombre(t, 50);
                    },
                    "Solo se permiten letras, entre 2 y 50 caracteres"
                )
            );

            p.setId_marca(
                leerEnteroPositivo("Marca ID: ")
            );

            p.setDescripcion(
                leerTextoValidado(
                    "Desc: ",
                    [](string t) {
                        return validar_descripcion(t, 200);
                    },
                    "Descripcion invalida, por favor ingrese una descripcion valida"
                )
            );

            p.setImagen(
                leerTextoValidado(
                    "Img: ",
                    [](string t) {
                        return validar_imagen(t, 100);
                    },
                    "Imagen invalida, por favor ingrese una imagen valida"
                )
            );

            p.setPrecio_costo(
                leerDecimalPositivo("Costo: ")
            );

            p.setPrecio_venta(
                leerDecimalPositivo("Venta: ")
            );

            p.setExistencia(
                leerEnteroPositivo("Existencia: ")
            );

            p.setFecha_ingreso(
                leerTextoValidado(
                    "Fecha: ",
                    validar_fecha,
                    "Formato YYYY-MM-DD"
                )
            );

            p.actualizar();
            break;
        }

        case 4: {

            Producto p;

            p.setIdproductos(
                leerEnteroPositivo("ID: ")
            );

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
                [](string t) {
                    return validar_nombre(t, 50);
                },
                "Solo se permiten letras, entre 2 y 50 caracteres"
            );

            string nit = leerTextoValidado(
                "NIT: ",
                validar_nit,
                "NIT invalido, por favor ingrese un NIT valido"
            );

            string dir = leerTextoValidado(
                "Direccion: ",
                [](string t) {
                    return validar_direccion(t, 100);
                },
                "Direccion invalida, por favor ingrese una direccion valida"
            );

            string tel = leerTextoValidado(
                "Telefono: ",
                validar_telefono_gt,
                "Telefono invalido, por favor ingrese un telefono valido"
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
                [](string t) {
                    return validar_nombre(t, 50);
                },
                "Solo se permiten letras, entre 2 y 50 caracteres"
            );

            string nit = leerTextoValidado(
                "NIT: ",
                validar_nit,
                "NIT invalido, por favor ingrese un NIT valido"
            );

            string dir = leerTextoValidado(
                "Direccion: ",
                [](string t) {
                    return validar_direccion(t, 100);
                },
                "Direccion invalida, por favor ingrese una direccion valida"
            );

            string tel = leerTextoValidado(
                "Telefono: ",
                validar_telefono_gt,
                "Telefono invalido, por favor ingrese un telefono valido"
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

            limpiarBuffer();

            string nom = leerTextoValidado(
                "Nombres: ",
                [](string t) {
                    return validar_nombre(t, 50);
                },
                "Solo se permiten letras, entre 2 y 50 caracteres"
            );

            string ape = leerTextoValidado(
                "Apellidos: ",
                [](string t) {
                    return validar_nombre(t, 50);
                },
                "Solo se permiten letras, entre 2 y 50 caracteres"
            );

            string nit = leerTextoValidado(
                "NIT: ",
                validar_nit,
                "NIT invalido, por favor ingrese un NIT valido"
            );


            limpiarBuffer();

            int gen = leerGenero("Genero (1=M 0=F): ");

            string tel = leerTextoValidado(
                "Telefono: ",
                validar_telefono_gt,
                "Telefono invalido, por favor ingrese un telefono valido"
            );

            string correo = leerTextoValidado(
                "Correo: ",
                validar_correo,
                "Correo invalido, por favor ingrese un correo valido "
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
            limpiarBuffer(); 

            string nom = leerTextoValidado(
                "Nombres: ",
                [](string t) {
                    return validar_nombre(t, 50);
                },
                "Solo se permiten letras, entre 2 y 50 caracteres"
            );

            string ape = leerTextoValidado(
                "Apellidos: ",
                [](string t) {
                    return validar_nombre(t, 50);
                },
                "Solo se permiten letras, entre 2 y 50 caracteres"
            );

            string nit = leerTextoValidado(
                "NIT: ",
                validar_nit,
                "NIT invalido, por favor ingrese un NIT valido"
            );

            
            limpiarBuffer();

            int gen = leerGenero("Genero (1=M 0=F): ");

            string tel = leerTextoValidado(
                "Telefono: ",
                validar_telefono_gt,
                "Telefono invalido, por favor ingrese un telefono valido"
            );

            string correo = leerTextoValidado(
                "Correo: ",
                validar_correo,
                "Correo invalido, por favor ingrese un correo valido "
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

// ================= MAIN =================

int main() {

    MYSQL* conectar;
    conectar_bd(conectar);

    int opcion;

    do {

        cout << "\n===== SISTEMA SUPERMERCADO =====\n";
        cout << "1. Puestos\n2. Empleados\n3. Marcas\n4. Productos\n5. Proveedores\n6. Clientes\n0. Salir\n";

        opcion = leerEntero("Opcion: ");

        switch (opcion) {

        case 1:
            menuPuestos(conectar);
            break;

        case 2:
            menuEmpleados(conectar);
            break;

        case 3:
            menuMarcas();
            break;

        case 4:
            menuProductos();
            break;

        case 5:
            menuProveedores(conectar);
            break;

        case 6:
            menuClientes();
            break;
        }

    } while (opcion != 0);

    mysql_close(conectar);

    return 0;
}