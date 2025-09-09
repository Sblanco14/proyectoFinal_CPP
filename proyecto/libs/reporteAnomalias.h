#ifndef REPORTEANOMALIAS_H
#define REPORTEANOMALIAS_H

#include <fstream>
#include <iostream>
#include "sala.h"
#include "anomalias.h"
#include "ordenar.h"

inline bool generarReporteAnomalias(const Sala& sala, const Umbral* umbrales, int nUmbrales, const char* nombreArchivo = "anomalias.txt") {
    std::ofstream out(nombreArchivo);
    if (!out) {
        std::cerr << "No se pudo crear " << nombreArchivo << "\n";
        return false;
    }
    out << "==== REPORTE DE ANOMALIAS ====\n\n";

    for (int m=0; m<sala.numMaquinas; ++m) {
        const MaquinaUCI& maq = sala.maquinas[m];
        for (uint32_t k=0; k<maq.numMediciones; ++k) {
            const Medicion& med = maq.mediciones[k];
            for (uint32_t j=0; j<med.numLecturas; ++j) {
                const Lectura& L = med.lecturas[j];
                if (L.tipoSensor!='E' && esAnomalaNoECG(L, umbrales, nUmbrales)) {
                    out << "Paciente " << med.idPaciente
                        << " | Maquina " << (int)maq.id
                        << " | " << med.fechaHora << " | ";
                    if (L.tipoSensor=='T')
                        out << "Temperatura=" << L.valor;
                    else if (L.tipoSensor=='O')
                        out << "Oximetria=" << L.valor;
                    else if (L.tipoSensor=='P')
                        out << "Presion SIS=" << L.sistolica << " DIA=" << L.diastolica;
                    out << " -> ANOMALIA\n";
                }
            }
        }
    }

    out.close();
    return true;
}

#endif
