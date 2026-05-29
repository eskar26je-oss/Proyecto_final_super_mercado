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
inline void crear_empleados(
	MYSQL* conectar,
	string nombres,
	string apellidos,
	string direccion,
	string telefono,
	string cui,
	int genero,
	string fecha_nacimiento,
	int id_puesto,
	string fecha_inicio,
	string fecha_ingreso
) {

	string consulta =
		"INSERT INTO empleados(nombres, apellidos, direccion, telefono, cui, genero, "
		"fecha_nacimiento, id_puesto, fecha_inicio_de_labores, fecha_ingreso) VALUES('";

	consulta += nombres + "','";
	consulta += apellidos + "','";
	consulta += direccion + "','";
	consulta += telefono + "','";
	consulta += cui + "',";
	consulta += to_string(genero) + ",'";
	consulta += fecha_nacimiento + "',";
	consulta += to_string(id_puesto) + ",'";
	consulta += fecha_inicio + "','";
	consulta += fecha_ingreso + "')";

	int estado = mysql_query(conectar, consulta.c_str());

	if (estado == 0)
		cout << "Empleado agregado\n";
	else
		cout << "Error: " << mysql_error(conectar) << endl;
}

// ================= READ =================
inline void mostrar_empleados(MYSQL* conectar) {

	MYSQL_ROW fila;
	MYSQL_RES* resultado;

	string consulta =
		"SELECT e.id_empleados, e.nombres, e.apellidos, p.puesto "
		"FROM empleados e "
		"INNER JOIN puestos p "
		"ON e.id_puesto = p.id_puestos";

	mysql_query(conectar, consulta.c_str());

	resultado = mysql_store_result(conectar);

	while ((fila = mysql_fetch_row(resultado))) {

		cout << "ID: " << fila[0]
			<< " | Nombre: " << fila[1] << " " << fila[2]
			<< " | Puesto: " << fila[3] << endl;
	}

	mysql_free_result(resultado);
}

// ================= UPDATE =================
inline void actualizar_empleados(
	MYSQL* conectar,
	int id_empleados,
	string nombres
) {

	string consulta =
		"UPDATE empleados SET nombres='" +
		nombres +
		"' WHERE id_empleados=" +
		to_string(id_empleados);

	int estado = mysql_query(conectar, consulta.c_str());

	if (estado == 0)
		cout << "Actualizado\n";
	else
		cout << "Error: " << mysql_error(conectar) << endl;
}	
	
// ================= DELETE =================
inline void eliminar_empleados(
	MYSQL* conectar,
	int id
) {

	string consulta =
		"DELETE FROM empleados WHERE id_empleados=" +
		to_string(id);

	int estado = mysql_query(conectar, consulta.c_str());

	if (estado == 0)
		cout << "Eliminado\n";
	else
		cout << "Error: " << mysql_error(conectar) << endl;
}