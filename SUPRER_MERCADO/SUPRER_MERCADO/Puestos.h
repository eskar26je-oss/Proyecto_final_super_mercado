#pragma once
#include <mysql.h>
#include <iostream>
#include <string>

#include "validaciones.h"

using namespace std;

// ================= STRUCT =================
struct Puestos {

	int id_puestos;
	char puesto[50];
};

// ================= CREATE =================
inline void crear_puestos(MYSQL* conectar) {

	Puestos p;
	char respuesta;

	do {

		// ================= PUESTO =================
		do {

			cout << "Ingrese nombre del puesto: ";

			cin.getline(p.puesto, 50);

			if (cin.fail()) {

				cin.clear();
				cin.ignore(10000, '\n');

				cout << "Demasiados caracteres.\n";
				cout << "Intente nuevamente.\n";

				continue;
			}

			if (!validar_texto_general(p.puesto, 50)) {

				cout << "Puesto invalido.\n";
				cout << "Intente nuevamente.\n";
			}

		} while (!validar_texto_general(p.puesto, 50));

		// ================= QUERY =================
		string consulta =
			"INSERT INTO puestos(puesto) "
			"VALUES('"
			+ string(p.puesto) +
			"')";

		int estado =
			mysql_query(
				conectar,
				consulta.c_str()
			);

		if (estado == 0) {

			cout << "Puesto ingresado correctamente.\n";
		}
		else {

			cout << "Error: "
				<< mysql_error(conectar)
				<< endl;
		}

		// ================= CONTINUAR =================
		cout << "Desea ingresar otro puesto? (s/n): ";

		cin >> respuesta;

		cin.clear();
		cin.ignore(10000, '\n');

	} while (
		respuesta == 's' ||
		respuesta == 'S'
		);
}

// ================= READ =================
inline void mostrar_puestos(MYSQL* conectar) {

	MYSQL_ROW fila;
	MYSQL_RES* resultado;

	string consulta =
		"SELECT * FROM puestos";

	int estado =
		mysql_query(
			conectar,
			consulta.c_str()
		);

	if (estado != 0) {

		cout << "Error: "
			<< mysql_error(conectar)
			<< endl;

		return;
	}

	resultado =
		mysql_store_result(conectar);

	if (resultado == NULL) {

		cout << "No hay datos.\n";
		return;
	}

	cout << "\n===== PUESTOS =====\n";

	while (
		(fila = mysql_fetch_row(resultado))
		) {

		cout
			<< "ID: " << fila[0]
			<< endl;

		cout
			<< "Puesto: " << fila[1]
			<< endl;

		cout
			<< "-------------------\n";
	}

	mysql_free_result(resultado);
}

// ================= UPDATE =================
inline void actualizar_puestos(MYSQL* conectar) {

	Puestos p;

	// ================= ID =================
	do {

		cout << "Ingrese ID del puesto a modificar: ";

		cin >> p.id_puestos;

		if (cin.fail()) {

			cin.clear();
			cin.ignore(10000, '\n');

			cout << "ID invalido.\n";
			cout << "Intente nuevamente.\n";

			p.id_puestos = -1;

			continue;
		}

		cin.ignore(10000, '\n');

		if (!validar_id(p.id_puestos)) {

			cout << "ID invalido.\n";
			cout << "Intente nuevamente.\n";
		}

	} while (!validar_id(p.id_puestos));

	// ================= PUESTO =================
	do {

		cout << "Ingrese nuevo nombre del puesto: ";

		cin.getline(p.puesto, 50);

		if (cin.fail()) {

			cin.clear();
			cin.ignore(10000, '\n');

			cout << "Demasiados caracteres.\n";
			cout << "Intente nuevamente.\n";

			continue;
		}

		if (!validar_texto_general(p.puesto, 50)) {

			cout << "Puesto invalido.\n";
			cout << "Intente nuevamente.\n";
		}

	} while (!validar_texto_general(p.puesto, 50));

	// ================= QUERY =================
	string consulta =
		"UPDATE puestos SET puesto='"
		+ string(p.puesto)
		+ "' WHERE id_puestos="
		+ to_string(p.id_puestos);

	int estado =
		mysql_query(
			conectar,
			consulta.c_str()
		);

	if (estado == 0) {

		cout << "Puesto actualizado correctamente.\n";
	}
	else {

		cout << "Error: "
			<< mysql_error(conectar)
			<< endl;
	}
}

// ================= DELETE =================
inline void eliminar_puestos(MYSQL* conectar) {

	int id;

	do {

		cout << "Ingrese ID del puesto a eliminar: ";

		cin >> id;

		if (cin.fail()) {

			cin.clear();
			cin.ignore(10000, '\n');

			cout << "ID invalido.\n";
			cout << "Intente nuevamente.\n";

			id = -1;

			continue;
		}

		cin.ignore(10000, '\n');

		if (!validar_id(id)) {

			cout << "ID invalido.\n";
			cout << "Intente nuevamente.\n";
		}

	} while (!validar_id(id));

	// ================= QUERY =================
	string consulta =
		"DELETE FROM puestos "
		"WHERE id_puestos="
		+ to_string(id);

	int estado =
		mysql_query(
			conectar,
			consulta.c_str()
		);

	if (estado == 0) {

		cout << "Puesto eliminado correctamente.\n";
	}
	else {

		cout << "Error: "
			<< mysql_error(conectar)
			<< endl;
	}
}

// ================= MENU =================
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

		if (cin.fail()) {

			cin.clear();
			cin.ignore(10000, '\n');

			cout << "Opcion invalida.\n";

			opcion = 0;

			continue;
		}

		cin.ignore(10000, '\n');

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

		case 5:
			cout << "Saliendo...\n";
			break;

		default:
			cout << "Opcion invalida.\n";
		}

	} while (opcion != 5);
}