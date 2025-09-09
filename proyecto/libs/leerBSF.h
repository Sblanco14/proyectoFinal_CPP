#ifndef LEERBSF_H
#define LEERBSF_H

#include <fstream>
#include <iostream>
#include <cstdint>
#include <cstring>
#include "sala.h"
#include "tamanos.h"

inline bool leerArchivoBSF(const char* nombre, Sala& sala) {
    std::ifstream in(nombre, std::ios::binary);
    if (!in) {
        std::cerr << "No se pudo abrir archivo: " << nombre << "\n";
        return false;
    }

    in.read(reinterpret_cast<char*>(&sala.idSala), sizeof(uint8_t));
    in.read(reinterpret_cast<char*>(&sala.numMaquinas), sizeof(uint8_t));
    sala.maquinas = new MaquinaUCI[sala.numMaquinas];

    for (int m=0; m<sala.numMaquinas; ++m) {
        in.read(reinterpret_cast<char*>(&sala.maquinas[m].id), sizeof(uint8_t));
        in.read(reinterpret_cast<char*>(&sala.maquinas[m].numMediciones), sizeof(uint32_t));
        
        sala.maquinas[m].mediciones = new Medicion[sala.maquinas[m].numMediciones];

        for (uint32_t k=0; k<sala.maquinas[m].numMediciones; ++k) {
            Medicion& med = sala.maquinas[m].mediciones[k];
            in.read(med.idPaciente, 11);  
            med.idPaciente[11] = '\0';
            in.read(med.fechaHora, 24);  
            med.fechaHora[24] = '\0';
            in.read(reinterpret_cast<char*>(&med.numLecturas), sizeof(uint32_t));
            
            if (!in || med.numLecturas > 10000) { 
                std::cerr << "numLecturas invalido: " << med.numLecturas << "\n";
                return false;
            }

            med.lecturas = new Lectura[med.numLecturas];

            for (uint32_t j=0; j<med.numLecturas; ++j) {
                Lectura& L = med.lecturas[j];
                in.read(&L.tipoSensor, 1);
                if (L.tipoSensor=='T' || L.tipoSensor=='E' || L.tipoSensor=='O') {
                    in.read(reinterpret_cast<char*>(&L.valor), sizeof(double));
                    L.sistolica = L.diastolica = 0;
                } else if (L.tipoSensor=='P') {
                    in.read(reinterpret_cast<char*>(&L.sistolica), sizeof(int));
                    in.read(reinterpret_cast<char*>(&L.diastolica), sizeof(int));
                    L.valor = 0.0;
                }
            }
        }
    }

    in.close();
    return true;
}

inline void liberarSala(Sala& sala) {
    if (!sala.maquinas) return;
    for (int m=0; m<sala.numMaquinas; ++m) {
        MaquinaUCI& maq = sala.maquinas[m];
        for (uint32_t k=0; k<maq.numMediciones; ++k) {
            delete[] maq.mediciones[k].lecturas;
        }
        delete[] maq.mediciones;
    }
    delete[] sala.maquinas;
    sala.maquinas = nullptr;
}

#endif
