#ifndef ANOMALIAS_H
#define ANOMALIAS_H

#include "lectura.h"
#include "umbral.h"
#include <cstring>

inline const Umbral* buscarUmbral(const Umbral* u, int n, const char* tipo){
    for(int i=0;i<n;++i) if(std::strcmp(u[i].tipoSensor, tipo)==0) return &u[i];
    return nullptr;
}

inline bool esAnomalaNoECG(const Lectura& L, const Umbral* umbrales, int nUmbrales){
    if (L.tipoSensor=='T'){
        const Umbral* ut = buscarUmbral(umbrales, nUmbrales, "T");
        if(!ut) return false; 
        return (L.valor < ut->min) || (L.valor > ut->max);
    } else if (L.tipoSensor=='O'){
        const Umbral* uo = buscarUmbral(umbrales, nUmbrales, "O");
        if(!uo) return false;
        return (L.valor < uo->min) || (L.valor > uo->max);
    } else if (L.tipoSensor=='P'){
        const Umbral* us = buscarUmbral(umbrales, nUmbrales, "P_SIS");
        const Umbral* ud = buscarUmbral(umbrales, nUmbrales, "P_DIA");
        bool as = (us && (L.sistolica < us->min || L.sistolica > us->max));
        bool ad = (ud && (L.diastolica < ud->min || L.diastolica > ud->max));
        return as || ad;
    }
    return false; 
}

#endif
