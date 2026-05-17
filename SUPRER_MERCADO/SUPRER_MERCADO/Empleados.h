#pragma once
#include <mysql.h>
#include <iostream>
#include <string>

using namespace std;


void mostrar_puestos(MYSQL* conectar);

struct Empleados {

	int id_empleados;
	char nombres[60];
	char apellidos[60];
	char direccion[80];
	char telefono[9];
	char cui[14];
	int genero;
	char fecha_nacimiento[11];
	int id_puesto;
	char fecha_inicio_de_labores[11];
	char fecha_ingreso[11];
};

// ================= CREATE =================
inline void crear_empleados(MYSQL* conectar) {

	Empleados empleados;
	char respuesta;

	do {

		cout << "Ingrese nombres: ";
		cin.getline(empleados.nombres, 60);

		cout << "Ingrese apellidos: ";
		cin.getline(empleados.apellidos, 60);

		cout << "Ingrese direccion: ";
		cin.getline(empleados.direccion, 80);

		cout << "Ingrese telefono: ";
		cin.getline(empleados.telefono, 9);

		cout << "Ingrese CUI: ";
		cin.getline(empleados.cui, 14);

		cout << "Genero (0/1): ";
		cin >> empleados.genero;
		cin.ignore();

		cout << "Fecha nacimiento: ";
		cin.getline(empleados.fecha_nacimiento, 11);

		mostrar_puestos(conectar);

		cout << "ID puesto: ";
		cin >> empleados.id_puesto;
		cin.ignore();

		cout << "Fecha inicio: ";
		cin.getline(empleados.fecha_inicio_de_labores, 11);

		cout << "Fecha ingreso: ";
		cin.getline(empleados.fecha_ingreso, 11);

		string consulta =
			"INSERT INTO empleados(nombres, apellidos, direccion, telefono, cui, genero, "
			"fecha_nacimiento, id_puesto, fecha_inicio_de_labores, fecha_ingreso) VALUES('";

		consulta += string(empleados.nombres) + "','";
		consulta += string(empleados.apellidos) + "','";
		consulta += string(empleados.direccion) + "','";
		consulta += string(empleados.telefono) + "','";
		consulta += string(empleados.cui) + "',";
		consulta += to_string(empleados.genero) + ",'";
		consulta += string(empleados.fecha_nacimiento) + "',";
		consulta += to_string(empleados.id_puesto) + ",'";
		consulta += string(empleados.fecha_inicio_de_labores) + "','";
		consulta += string(empleados.fecha_ingreso) + "')";

		mysql_query(conectar, consulta.c_str());

		cout << "Empleado agregado\n";

		cout << "Otro? (s/n): ";
		cin >> respuesta;
		cin.ignore();

	} while (respuesta == 's' || respuesta == 'S');
}

// ================= READ =================
inline void mostrar_empleados(MYSQL* conectar) {

	MYSQL_ROW fila;
	MYSQL_RES* resultado;

	string consulta =
		"SELECT e.id_empleados, e.nombres, e.apellidos, p.puesto "
		"FROM empleados e INNER JOIN puestos p ON e.id_puesto = p.id_puestos";

	mysql_query(conectar, consulta.c_str());

	resultado = mysql_store_result(conectar);

	while ((fila = mysql_fetch_row(resultado))) {

		cout << "ID: " << fila[0]
			<< " | Nombre: " << fila[1] << " " << fila[2]
			<< " | Puesto: " << fila[3] << endl;
	}
}

// ================= UPDATE =================
inline void actualizar_empleados(MYSQL* conectar) {

	Empleados e;

	cout << "ID: ";
	cin >> e.id_empleados;
	cin.ignore();

	cout << "Nuevo nombre: ";
	cin.getline(e.nombres, 60);

	string consulta =
		"UPDATE empleados SET nombres='" + string(e.nombres) +
		"' WHERE id_empleados=" + to_string(e.id_empleados);

	mysql_query(conectar, consulta.c_str());

	cout << "Actualizado\n";
}

// ================= DELETE =================
inline void eliminar_empleados(MYSQL* conectar) {

	int id;

	cout << "ID eliminar: ";
	cin >> id;

	string consulta =
		"DELETE FROM empleados WHERE id_empleados=" + to_string(id);

	mysql_query(conectar, consulta.c_str());

	cout << "Eliminado\n";
}

// ================= MENU =================
inline void menu_empleados(MYSQL* conectar) {

	int opcion;

	do {

		cout << "\n===== EMPLEADOS =====\n";
		cout << "1. Crear\n2. Mostrar\n3. Actualizar\n4. Eliminar\n5. Salir\n";
		cout << "Opcion: ";
		cin >> opcion;
		cin.ignore();

		switch (opcion) {

		case 1: crear_empleados(conectar); break;
		case 2: mostrar_empleados(conectar); break;
		case 3: actualizar_empleados(conectar); break;
		case 4: eliminar_empleados(conectar); break;
		}

	} while (opcion != 5);
}