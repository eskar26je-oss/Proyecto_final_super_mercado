#pragma once
#include <mysql.h>
#include <iostream>
#include <string>

using namespace std;

struct Puestos {

	int id_puestos;
	char puesto[50];
};

// ================= CREATE =================
inline void crear_puestos(
	MYSQL* conectar,
	string puesto
) {

	string consulta =
		"INSERT INTO puestos(puesto) VALUES('" +
		puesto + "')";

	int estado = mysql_query(conectar, consulta.c_str());

	if (estado == 0)
		cout << "Puesto ingresado correctamente\n";
	else
		cout << "Error: " << mysql_error(conectar) << endl;
}

// ================= READ =================
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

// ================= UPDATE =================
inline void actualizar_puestos(
	MYSQL* conectar,
	int id_puestos,
	string puesto
) {

	string consulta =
		"UPDATE puestos SET puesto = '" +
		puesto +
		"' WHERE id_puestos = " +
		to_string(id_puestos);

	int estado = mysql_query(conectar, consulta.c_str());

	if (estado == 0)
		cout << "Puesto actualizado correctamente\n";
	else
		cout << "Error: " << mysql_error(conectar) << endl;
}

// ================= DELETE =================
inline void eliminar_puestos(
	MYSQL* conectar,
	int id
) {

	string consulta =
		"DELETE FROM puestos WHERE id_puestos = " +
		to_string(id);

	int estado = mysql_query(conectar, consulta.c_str());

	if (estado == 0)
		cout << "Puesto eliminado correctamente\n";
	else
		cout << "Error: " << mysql_error(conectar) << endl;
}