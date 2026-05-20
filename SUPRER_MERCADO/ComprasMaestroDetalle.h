#pragma once

#include <vector>
#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>

using namespace std;

// Estructura para detalles de compra
struct DetalleCompra {
    int id_detalle;
    int id_producto;
    string nombre_producto;
    int cantidad;
    double precio_costo_unitario;
    double subtotal;

    DetalleCompra() : id_detalle(0), id_producto(0), cantidad(0), 
                      precio_costo_unitario(0), subtotal(0) {}
};

// Clase para gestionar Compras Maestro-Detalle
class ComprasMaestroDetalle {
private:
    int id_compra;
    int numero_orden;
    int id_proveedor;
    string proveedor_nombre;
    string fecha_orden;
    string fecha_ingreso;
    vector<DetalleCompra> detalles;
    double total;

    string ObtenerFechaActual() {
        time_t now = time(0);
        struct tm tstruct = *localtime(&now);
        char buffer[20];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d", &tstruct);
        return string(buffer);
    }

    string ObtenerFechaHoraActual() {
        time_t now = time(0);
        struct tm tstruct = *localtime(&now);
        char buffer[30];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &tstruct);
        return string(buffer);
    }

public:
    ComprasMaestroDetalle() : id_compra(0), numero_orden(0), id_proveedor(0), total(0) {
        fecha_orden = ObtenerFechaActual();
        fecha_ingreso = ObtenerFechaHoraActual();
    }

    // Setters del Maestro
    void SetIdCompra(int id) { id_compra = id; }
    void SetNumeroOrden(int numero) { numero_orden = numero; }
    void SetProveedor(int id, const string& nombre) {
        id_proveedor = id;
        proveedor_nombre = nombre;
    }
    void SetFechaOrden(const string& fecha) { fecha_orden = fecha; }

    // Getters del Maestro
    int GetIdCompra() const { return id_compra; }
    int GetNumeroOrden() const { return numero_orden; }
    int GetIdProveedor() const { return id_proveedor; }
    string GetProveedorNombre() const { return proveedor_nombre; }
    string GetFechaOrden() const { return fecha_orden; }
    string GetFechaIngreso() const { return fecha_ingreso; }
    double GetTotal() const { return total; }

    // Métodos para detalles
    void AgregarDetalle(int id_producto, const string& nombre, int cantidad, double precio_unitario) {
        if (cantidad <= 0 || precio_unitario < 0) return;

        DetalleCompra detalle;
        detalle.id_detalle = detalles.size() + 1;
        detalle.id_producto = id_producto;
        detalle.nombre_producto = nombre;
        detalle.cantidad = cantidad;
        detalle.precio_costo_unitario = precio_unitario;
        detalle.subtotal = cantidad * precio_unitario;

        detalles.push_back(detalle);
        CalcularTotal();
    }

    bool ModificarDetalle(int indice, int cantidad, double precio_unitario) {
        if (indice < 0 || indice >= detalles.size()) return false;
        if (cantidad <= 0 || precio_unitario < 0) return false;

        detalles[indice].cantidad = cantidad;
        detalles[indice].precio_costo_unitario = precio_unitario;
        detalles[indice].subtotal = cantidad * precio_unitario;
        CalcularTotal();
        return true;
    }

    bool EliminarDetalle(int indice) {
        if (indice < 0 || indice >= detalles.size()) return false;

        detalles.erase(detalles.begin() + indice);
        CalcularTotal();
        return true;
    }

    void CalcularTotal() {
        total = 0;
        for (const auto& detalle : detalles) {
            total += detalle.subtotal;
        }
    }

    const vector<DetalleCompra>& GetDetalles() const { return detalles; }
    int GetCantidadDetalles() const { return detalles.size(); }

    void LimpiarDetalles() {
        detalles.clear();
        total = 0;
    }

    // Validar si la compra está completa
    bool ValidarCompra() const {
        return id_compra > 0 && numero_orden > 0 && id_proveedor > 0 && detalles.size() > 0;
    }
};
