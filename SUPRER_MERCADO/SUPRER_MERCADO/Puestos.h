#pragma once
#include <mysql.h>
#include <iostream>
#include <string>

using namespace std;

struct Puestos {
	int id_puestos;
	char puesto[50];
};

// CREATE
inline void crear_puestos(MYSQL* conectar) {

	Puestos p;
	char respuesta;

	do {

		cout << "Ingrese nombre del puesto: ";
		cin.getline(p.puesto, 50);

		string consulta =
			"INSERT INTO puestos(puesto) VALUES('" +
			string(p.puesto) + "')";

		int estado = mysql_query(conectar, consulta.c_str());

		if (estado == 0)
			cout << "Puesto ingresado correctamente\n";
		else
			cout << "Error: " << mysql_error(conectar) << endl;

		cout << "Desea ingresar otro puesto? (s/n): ";
		cin >> respuesta;
		cin.ignore();

	} while (respuesta == 's' || respuesta == 'S');
}

// READ
inline void mostrar_puestos(MYSQL* conectar) {

	MYSQL_ROW fila;
	MYSQL_RES* resultado;

	string consulta = "SELECT * FROM puestos";

	mysql_query(conectar, consulta.c_str());

	resultado = mysql_store_result(conectar);

	cout << "\n===== PUESTOS =====\n";

	while ((fila = mysql_fetch_row(resultado))) {

		cout << "ID: " << fila[0] << endl;
		cout << "Puesto: " << fila[1] << endl;
		cout << "-------------------\n";
	}

	mysql_free_result(resultado);
}

// UPDATE
inline void actualizar_puestos(MYSQL* conectar) {

	Puestos p;

	cout << "Ingrese ID del puesto a modificar: ";
	cin >> p.id_puestos;
	cin.ignore();

	cout << "Ingrese nuevo nombre del puesto: ";
	cin.getline(p.puesto, 50);

	string consulta =
		"UPDATE puestos SET puesto = '" +
		string(p.puesto) +
		"' WHERE id_puestos = " +
		to_string(p.id_puestos);

	int estado = mysql_query(conectar, consulta.c_str());

	if (estado == 0)
		cout << "Puesto actualizado correctamente\n";
	else
		cout << "Error: " << mysql_error(conectar) << endl;
}

// DELETE
inline void eliminar_puestos(MYSQL* conectar) {

	int id;

	cout << "Ingrese ID del puesto a eliminar: ";
	cin >> id;

	string consulta =
		"DELETE FROM puestos WHERE id_puestos = " +
		to_string(id);

	int estado = mysql_query(conectar, consulta.c_str());

	if (estado == 0)
		cout << "Puesto eliminado correctamente\n";
	else
		cout << "Error: " << mysql_error(conectar) << endl;
}

// MENU
inline void menu_puestos(MYSQL* conectar) {

	int opcion;

	do {

		cout << "\n===== CRUD PUESTOS =====\n";
		cout << "1. Crear Puesto\n";
		cout << "2. Mostrar Puestos\n";
		cout << "3. Actualizar Puesto\n";
		cout << "4. Eliminar Puesto\n";
		cout << "5. Salir\n";
		cout << "Seleccione una opcion: ";

		cin >> opcion;
		cin.ignore();

		switch (opcion) {

		case 1:
			crear_puestos(conectar);
			break;

		case 2:
			mostrar_puestos(conectar);
			break;

		case 3:
			actualizar_puestos(conectar);
			break;

		case 4:
			eliminar_puestos(conectar);
			break;
		}

	} while (opcion != 5);
}