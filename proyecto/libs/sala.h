#ifndef SALA_H
#define SALA_H

#include "maquina.h"
#include <cstdint>

struct Sala {
    uint8_t idSala;
    uint8_t numMaquinas;
    MaquinaUCI* maquinas;
};

#endif