#ifndef MEDICION_H
#define MEDICION_H

#include "lectura.h"
#include "tamanos.h"
#include <cstdint>

struct Medicion {
    char idPaciente[ID_PACIENTE_LEN];
    char fechaHora[FECHA_HORA_LEN];
    uint32_t numLecturas;

    Lectura* lecturas;
};

#endif