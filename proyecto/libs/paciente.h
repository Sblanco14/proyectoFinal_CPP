#ifndef PACIENTE_H
#define PACIENTE_H

#include "tamanos.h"

struct Paciente {
    char id[ID_PACIENTE_LEN];
    char tipoDocumento[4];
    char documento[16];
    char nombres[40];
    char apellidos[40];
    char fechaNacimiento[16];
    char telefono[20];
    char email[50];
    char tipoSangre[4];
    char entidadSalud[20];
    char medicinaPrepagada[20];
};

#endif