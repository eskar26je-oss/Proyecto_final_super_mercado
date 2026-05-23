#pragma once

#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>

using namespace std;

// ================= CAMPOS VACIOS =================
inline bool campo_vacio(string texto) {

    return texto.empty();
}

// ================= SOLO LETRAS =================
inline bool solo_letras(string texto) {

    if (texto.empty()) {
        return false;
    }

    for (char c : texto) {

        if (!isalpha((unsigned char)c) && c != ' ') {

            return false;
        }
    }

    return true;
}

// ================= SOLO NUMEROS =================
inline bool solo_numeros(string texto) {

    if (texto.empty()) {

        return false;
    }

    for (char c : texto) {

        if (!isdigit((unsigned char)c)) {

            return false;
        }
    }

    return true;
}

// ================= DIRECCION =================
inline bool validar_direccion(string texto, int maximo = 100) {

    if (campo_vacio(texto)) {

        return false;
    }

    if (texto.length() > maximo) {

        return false;
    }

    for (char c : texto) {

        if (
            !isalnum((unsigned char)c) &&
            c != ' ' &&
            c != '-' &&
            c != '#' &&
            c != '.' &&
            c != ',' &&
            c != '/'
            ) {

            return false;
        }
    }

    return true;
}

// ================= SOLO DECIMALES =================
inline bool validar_decimal(string texto) {

    if (texto.empty()) {

        return false;
    }

    bool punto = false;
    bool numero = false;

    for (char c : texto) {

        if (c == '.') {

            if (punto) {

                return false;
            }

            punto = true;
        }
        else if (isdigit((unsigned char)c)) {

            numero = true;
        }
        else {

            return false;
        }
    }

    return numero;
}

// ================= LONGITUD EXACTA =================
inline bool validar_longitud(string texto, int longitud) {

    return texto.length() == longitud;
}

// ================= LONGITUD MAXIMA =================
inline bool validar_longitud_max(string texto, int longitud) {

    return texto.length() <= longitud;
}

// ================= GENERO =================
inline bool validar_genero(int genero) {

    return genero == 0 || genero == 1;
}

// ================= ENTERO POSITIVO =================
inline bool validar_entero_positivo(int numero) {

    return numero > 0;
}

// ================= DECIMAL POSITIVO =================
inline bool validar_decimal_positivo(float numero) {

    return numero > 0;
}

// ================= TELEFONO =================
inline bool validar_telefono_gt(string telefono) {

    if (!solo_numeros(telefono)) {

        return false;
    }

    return telefono.length() == 8;
}

// ================= CUI =================
inline bool validar_cui(string cui) {

    if (!solo_numeros(cui)) {

        return false;
    }

    return cui.length() == 13;
}

// ================= NIT =================
inline bool validar_nit(string nit) {

    if (campo_vacio(nit)) {

        return false;
    }

    if (nit.length() > 12) {

        return false;
    }

    for (char c : nit) {

        if (!isdigit((unsigned char)c) && c != '-') {

            return false;
        }
    }

    return true;
}

// ================= CORREO =================
inline bool validar_correo(string correo) {

    if (campo_vacio(correo)) {

        return false;
    }

    size_t arroba = correo.find('@');

    if (arroba == string::npos) {

        return false;
    }

    if (arroba != correo.rfind('@')) {

        return false;
    }

    size_t punto = correo.find('.', arroba);

    if (punto == string::npos) {

        return false;
    }

    if (arroba == 0 || punto == correo.length() - 1) {

        return false;
    }

    return true;
}

// ================= FECHA =================
inline bool validar_fecha(string fecha) {

    if (fecha.length() != 10) {

        return false;
    }

    if (fecha[4] != '-' || fecha[7] != '-') {

        return false;
    }

    for (int i = 0; i < fecha.length(); i++) {

        if (i == 4 || i == 7) {

            continue;
        }

        if (!isdigit((unsigned char)fecha[i])) {

            return false;
        }
    }

    int anio = stoi(fecha.substr(0, 4));
    int mes = stoi(fecha.substr(5, 2));
    int dia = stoi(fecha.substr(8, 2));

    if (anio < 1900 || anio > 2100) {

        return false;
    }

    if (mes < 1 || mes > 12) {

        return false;
    }

    int dias_mes[] = {
        31,28,31,30,31,30,
        31,31,30,31,30,31
    };

    bool bisiesto =
        (anio % 4 == 0 && anio % 100 != 0) ||
        (anio % 400 == 0);

    if (bisiesto && mes == 2) {

        dias_mes[1] = 29;
    }

    if (dia < 1 || dia > dias_mes[mes - 1]) {

        return false;
    }

    return true;
}

// ================= STOCK =================
inline bool validar_stock(int stock) {

    return stock >= 0;
}

// ================= PRECIOS =================
inline bool validar_precio(float precio) {

    return precio > 0;
}

// ================= VALIDAR ID =================
inline bool validar_id(int id) {

    return id > 0;
}

// ================= NOMBRES =================
inline bool validar_nombre(string texto, int maximo) {

    if (campo_vacio(texto)) {

        return false;
    }

    if (!validar_longitud_max(texto, maximo)) {

        return false;
    }

    for (char c : texto) {

        if (
            !isalpha((unsigned char)c) &&
            c != ' '
            ) {

            return false;
        }
    }

    return true;
}

// ================= TEXTO GENERAL =================
inline bool validar_texto_general(string texto, int maximo) {

    if (campo_vacio(texto)) {

        return false;
    }

    if (!validar_longitud_max(texto, maximo)) {

        return false;
    }

    for (char c : texto) {

        if (
            !isalnum((unsigned char)c) &&
            c != ' ' &&
            c != '.' &&
            c != ',' &&
            c != '-' &&
            c != '_' &&
            c != '(' &&
            c != ')'
            ) {

            return false;
        }
    }

    return true;
}

// ================= DESCRIPCION =================
inline bool validar_descripcion(string texto, int maximo) {

    if (campo_vacio(texto)) {

        return false;
    }

    if (!validar_longitud_max(texto, maximo)) {

        return false;
    }

    for (char c : texto) {

        if (
            !isalnum((unsigned char)c) &&
            c != ' ' &&
            c != '.' &&
            c != ',' &&
            c != '-' &&
            c != '(' &&
            c != ')' &&
            c != ':'
            ) {

            return false;
        }
    }

    return true;
}

// ================= IMAGEN =================
inline bool validar_imagen(string texto, int maximo) {

    if (campo_vacio(texto)) {

        return false;
    }

    if (!validar_longitud_max(texto, maximo)) {

        return false;
    }

    for (char c : texto) {

        if (
            !isalnum((unsigned char)c) &&
            c != '.' &&
            c != '_' &&
            c != '-'
            ) {

            return false;
        }
    }

    return true;
}
inline bool texto_valido_general(string texto, int maximo) {
    return validar_texto_general(texto, maximo);
}