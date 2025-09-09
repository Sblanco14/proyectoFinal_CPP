#ifndef MAQUINA_H
#define MAQUINA_H

#include "medicion.h"
#include <cstdint>

struct MaquinaUCI {
    uint8_t id;
    uint32_t numMediciones;
    Medicion* mediciones;
};

#endif