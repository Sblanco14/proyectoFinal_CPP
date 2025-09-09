#ifndef ANOMALIAS_ECG_H
#define ANOMALIAS_ECG_H

#include <cstring>
#include <cmath>
#include "sala.h"
#include "umbral.h"

// Busca el umbral de ECG ("E") en el arreglo de umbrales
inline const Umbral* buscarUmbralE(const Umbral* U, int n){
    for (int i=0; i<n; ++i) {
        if (std::strcmp(U[i].tipoSensor, "E") == 0) return &U[i];
    }
    return nullptr;
}

// Calcula min y max de ECG para un paciente dentro de una máquina
inline bool minMaxECGEnMaquina(const MaquinaUCI& maq, const char* idPac,
                               double& mn, double& mx){
    bool hay=false;
    mn = 1e300; mx = -1e300;
    for (uint32_t k=0; k<maq.numMediciones; ++k){
        const Medicion& M = maq.mediciones[k];
        if (std::strcmp(M.idPaciente, idPac) != 0) continue;
        for (uint32_t j=0; j<M.numLecturas; ++j){
            const Lectura& L = M.lecturas[j];
            if (L.tipoSensor=='E'){
                if (L.valor < mn) mn = L.valor;
                if (L.valor > mx) mx = L.valor;
                hay = true;
            }
        }
    }
    return hay;
}

// ¿Paciente tiene ECG anómalo en esta máquina?
inline bool ecgAnomaloPacienteEnMaquina(const MaquinaUCI& maq, const char* idPac,
                                        const Umbral* U, int nU){
    const Umbral* e = buscarUmbralE(U, nU);
    if (!e) return false;
    double mn, mx;
    if (!minMaxECGEnMaquina(maq, idPac, mn, mx)) return false;
    const double lhs = std::fabs(mn) + std::fabs(mx);
    const double rhs = std::fabs(e->min) + std::fabs(e->max);
    return lhs > rhs;
}

#endif
