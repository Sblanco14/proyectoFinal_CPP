#ifndef ORDENAR_H
#define ORDENAR_H

#include "medicion.h"
#include "funcionesFecha.h"
#include <cstring>

inline void ordenarMedicionesPorFecha(Medicion* arr, int n){
    for(int i=1;i<n;++i){
        Medicion key = arr[i];
        int j = i-1;
        while(j>=0 && compararFechas(arr[j].fechaHora, key.fechaHora)){
            arr[j+1] = arr[j];
            --j;
        }
        arr[j+1] = key;
    }
}

#endif
