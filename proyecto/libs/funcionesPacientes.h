#ifndef FUNCIONESPACIENTES_H
#define FUNCIONESPACIENTES_H

#include "paciente.h"
#include <cstring>
#include <string>

static inline std::string trimP(const std::string& s) {
    size_t i = 0, j = s.size();
    while (i < j && (s[i] == ' ' || s[i] == '\t')) ++i;
    while (j > i && (s[j-1] == ' ' || s[j-1] == '\t' || s[j-1] == '\r')) --j;
    return s.substr(i, j - i);
}

inline void copiarCampo(char* dest, size_t destSize, const std::string& src) {
    std::string t = trimP(src);
    std::strncpy(dest, t.c_str(), destSize - 1);
    dest[destSize - 1] = '\0';
}

inline Paciente parseLineaPaciente(const char* linea) {
    Paciente p{};
 
    p.id[0] = p.tipoDocumento[0] = p.documento[0] = p.nombres[0] = p.apellidos[0] = '\0';
    p.fechaNacimiento[0] = p.telefono[0] = p.email[0] = p.tipoSangre[0] = p.entidadSalud[0] = p.medicinaPrepagada[0] = '\0';

    if (linea == nullptr) return p;
    std::string s(linea);
    s = trimP(s);
    if (s.empty() || s[0] == '#') return p;

    std::string campos[11];
    int n = 0;
    size_t start = 0;
    while (n < 11 && start <= s.size()) {
        size_t pos = s.find(';', start);
        if (pos == std::string::npos) {
            campos[n++] = s.substr(start);
            break;
        } else {
            campos[n++] = s.substr(start, pos - start);
            start = pos + 1;
        }
    }
    while (n < 11) campos[n++] = "";

    copiarCampo(p.id, sizeof(p.id), campos[0]);
    copiarCampo(p.tipoDocumento, sizeof(p.tipoDocumento), campos[1]);
    copiarCampo(p.documento, sizeof(p.documento), campos[2]);
    copiarCampo(p.nombres, sizeof(p.nombres), campos[3]);
    copiarCampo(p.apellidos, sizeof(p.apellidos), campos[4]);
    copiarCampo(p.fechaNacimiento, sizeof(p.fechaNacimiento), campos[5]);
    copiarCampo(p.telefono, sizeof(p.telefono), campos[6]);
    copiarCampo(p.email, sizeof(p.email), campos[7]);
    copiarCampo(p.tipoSangre, sizeof(p.tipoSangre), campos[8]);
    copiarCampo(p.entidadSalud, sizeof(p.entidadSalud), campos[9]);
    copiarCampo(p.medicinaPrepagada, sizeof(p.medicinaPrepagada), campos[10]);

    return p;
}

#endif
