#ifndef REPORTES_H
#define REPORTES_H

#include <fstream>
#include <iostream>
#include <cstring>
#include "medicion.h"
#include "ordenar.h"
#include "anomalias.h"
#include "estadisticas.h"

inline bool generarReporteMedicionesPaciente(
    const char* idPaciente,
    Medicion* mediciones, int nMediciones,
    const Umbral* umbrales, int nUmbrales,
    char outFilePath[256] 
){
    if (!idPaciente || !mediciones || nMediciones<=0) return false;

    ordenarMedicionesPorFecha(mediciones, nMediciones);

    std::snprintf(outFilePath, 256, "mediciones_paciente_%s.txt", idPaciente);
    std::ofstream out(outFilePath);
    if(!out){ std::cerr<<"No se pudo crear "<<outFilePath<<"\n"; return false; }

    out << "Reporte de Mediciones - Paciente " << idPaciente << "\n";
    out << "========================================\n\n";

    StatsD stT, stE, stO; 
    StatsI stPS, stPD;
    init(stT); 
    init(stE); 
    init(stO); 
    init(stPS); 
    init(stPD);

    const char* secciones[4] = {"T","P","E","O"};
    for(int s=0;s<4;++s){
        const char* tipo = secciones[s];
        if(strcmp(tipo,"T")==0) out << "[ Temperatura ]\n";
        if(strcmp(tipo,"P")==0) out << "[ Presion Arterial ]\n";
        if(strcmp(tipo,"E")==0) out << "[ ECG ]\n";
        if(strcmp(tipo,"O")==0) out << "[ Oximetria ]\n";

        for(int i=0;i<nMediciones;++i){
            const Medicion& M = mediciones[i];
            for(uint32_t j=0; j<M.numLecturas; ++j){
                const Lectura& L = M.lecturas[j];
                if ( (strcmp(tipo,"T")==0 && L.tipoSensor=='T') ||
                     (strcmp(tipo,"P")==0 && L.tipoSensor=='P') ||
                     (strcmp(tipo,"E")==0 && L.tipoSensor=='E') ||
                     (strcmp(tipo,"O")==0 && L.tipoSensor=='O') ){
                    // Texto de la lectura
                    out << M.fechaHora << " - ";
                    if (L.tipoSensor=='P'){
                        bool an = esAnomalaNoECG(L, umbrales, nUmbrales);
                        out << "P_SIS=" << L.sistolica << "  P_DIA=" << L.diastolica
                            << "  [" << (an ? "ANOMALA" : "NORMAL") << "]\n";
                        acumular(stPS, L.sistolica);
                        acumular(stPD, L.diastolica);
                    } else if (L.tipoSensor=='T'){
                        bool an = esAnomalaNoECG(L, umbrales, nUmbrales);
                        out << "T=" << L.valor << " °C  [" << (an ? "ANOMALA" : "NORMAL") << "]\n";
                        acumular(stT, L.valor);
                    } else if (L.tipoSensor=='O'){
                        bool an = esAnomalaNoECG(L, umbrales, nUmbrales);
                        out << "O=" << L.valor << " %  [" << (an ? "ANOMALA" : "NORMAL") << "]\n";
                        acumular(stO, L.valor);
                    } else if (L.tipoSensor=='E'){
                        out << "ECG=" << L.valor << " mV\n"; 
                    }
                }
            }
        }

        if(strcmp(tipo,"T")==0 && stT.cnt){
            out << "  --> min: " << stT.min << "  max: " << stT.max << "  prom: " << promedio(stT) << "\n";
        }
        if(strcmp(tipo,"P")==0 && (stPS.cnt||stPD.cnt)){
            if(stPS.cnt) out << "  --> SIS min:" << stPS.min << " max:" << stPS.max << " prom:" << promedio(stPS) << "\n";
            if(stPD.cnt) out << "  --> DIA min:" << stPD.min << " max:" << stPD.max << " prom:" << promedio(stPD) << "\n";
        }
        if(strcmp(tipo,"E")==0 && stE.cnt){
            out << "  --> min: " << stE.min << "  max: " << stE.max << "  prom: " << promedio(stE) << "\n";
        }
        if(strcmp(tipo,"O")==0 && stO.cnt){
            out << "  --> min: " << stO.min << "  max: " << stO.max << "  prom: " << promedio(stO) << "\n";
        }

        out << "\n";
    }

    out.close();
    return true;
}

#endif
