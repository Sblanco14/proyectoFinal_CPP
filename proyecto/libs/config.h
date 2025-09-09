#ifndef CONFIG_H
#define CONFIG_H

#include <cstdint>
#include "tamanos.h"

struct Config {
    char rutaDatosBSF[RUTA_LEN];
    char rutaPacientesTxt[RUTA_LEN];
    char rutaReporteTxt[RUTA_LEN];

    double tempMinValida;
    double tempMaxValida;
    double ecgMinValido;
    double ecgMaxValido;
    int32_t oxiMinValido;
    int32_t oxiMaxValido;

    uint8_t activarLogsBinarios;
    uint8_t activarReporteDetallado;
};


#endif