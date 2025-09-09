#ifndef EXPORT_ECG_H
#define EXPORT_ECG_H

#include <fstream>
#include <cstring>
#include "sala.h"
#include "anomalias_ecg.h"

// Cuenta cuántas lecturas ECG tiene un paciente en una máquina
inline uint32_t contarECG(const MaquinaUCI& maq, const char* idPac){
    uint32_t c = 0;
    for (uint32_t k=0; k<maq.numMediciones; ++k){
        const Medicion& M = maq.mediciones[k];
        if (std::strcmp(M.idPaciente, idPac) != 0) continue;
        for (uint32_t j=0; j<M.numLecturas; ++j)
            if (M.lecturas[j].tipoSensor=='E') ++c;
    }
    return c;
}

// Evita duplicar paciente dentro de la MISMA máquina (sin STL)
inline bool yaVistoPacienteEnAnterior(const MaquinaUCI& maq, uint32_t hastaK, const char* idPac){
    for (uint32_t kk=0; kk<hastaK; ++kk)
        if (std::strcmp(maq.mediciones[kk].idPaciente, idPac) == 0) return true;
    return false;
}

// Exporta pacientes con ECG anómalo (por cada máquina donde sea anómalo) al binario
inline bool exportarPacientesECGAnomalos(const Sala& sala,
                                         const Umbral* U, int nU,
                                         const char* nombre = "pacientes_ecg_anomalos.dat"){
    std::ofstream out(nombre, std::ios::binary);
    if (!out) return false;

    for (uint32_t m=0; m<sala.numMaquinas; ++m){
        const MaquinaUCI& maq = sala.maquinas[m];

        for (uint32_t k=0; k<maq.numMediciones; ++k){
            const char* id = maq.mediciones[k].idPaciente;

            // Evitar duplicado del mismo paciente en la misma máquina
            if (yaVistoPacienteEnAnterior(maq, k, id)) continue;

            if (ecgAnomaloPacienteEnMaquina(maq, id, U, nU)){
                const uint32_t nECG = contarECG(maq, id);
                if (nECG == 0) continue;

                // --- Cabecera del bloque ---
                // ID Paciente (11 bytes exactos, sin '\0')
                char id11[11]; std::memset(id11, 0, 11);
                std::memcpy(id11, id, std::strlen(id) < 11 ? std::strlen(id) : 11);
                out.write(id11, 11);

                // Número de mediciones ECG (4 bytes)
                out.write(reinterpret_cast<const char*>(&nECG), sizeof(uint32_t));

                // --- Mediciones ECG ---
                for (uint32_t kk=0; kk<maq.numMediciones; ++kk){
                    const Medicion& M = maq.mediciones[kk];
                    if (std::strcmp(M.idPaciente, id) != 0) continue;

                    for (uint32_t j=0; j<M.numLecturas; ++j){
                        const Lectura& L = M.lecturas[j];
                        if (L.tipoSensor != 'E') continue;

                        // Fecha y hora (24 bytes exactos)
                        char f24[24]; std::memcpy(f24, M.fechaHora, 24);
                        out.write(f24, 24);

                        // Valor ECG (8 bytes)
                        out.write(reinterpret_cast<const char*>(&L.valor), sizeof(double));
                    }
                }
            }
        }
    }
    out.close();
    return true;
}

#endif
