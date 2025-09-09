#ifndef LECTURA_H
#define LECTURA_H

#include <cstdint>

struct Lectura {
    char tipoSensor;
    double valor;
    int32_t sistolica;
    int32_t diastolica;
};

#endif