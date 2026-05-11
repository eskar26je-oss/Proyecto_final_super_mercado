#include <iostream>
#include "Conexion_BD.h"
using namespace std;
int main()
{
ConexionBD cn = ConexionBD();
cn.abrir_conexion();
if (cn.get_conexion()) {
	cout << "Conexion Exitosa" << endl;
}
else {
	cout << "Conexion Fallida XXXXXXXXXXXXXXXXx" << endl;
}
	cn.cerrar_conexion();

}