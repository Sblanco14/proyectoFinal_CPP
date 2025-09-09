#include <iostream>
#include <cstring>
using namespace std;

#include "libs/cargarDatos.h"
#include "libs/reportes.h"
#include "libs/tamanos.h"
#include "libs/medicion.h"
#include "libs/lectura.h"
#include "libs/sala.h"
#include "libs/leerBSF.h"
#include "libs/reporteAnomalias.h"
#include "libs/anomalias_ecg.h"
#include "libs/export_ecg.h"

static void liberarMediciones(Medicion* arr, int n){
    if(!arr) return;
    for(int i=0;i<n;++i) delete[] arr[i].lecturas;
    delete[] arr;
}

int main(){
    cout << "=== Sistema UCI ===\n";
    int opcion=0; Umbral* umbrales=nullptr; 
    int nUmbrales=0;
    ArchivoPacientes AP{};
    Sala sala{}; sala.maquinas=nullptr;

    do{
        std::cout << "\nMenu:\n";
        std::cout << "1) Cargar configuracion y pacientes\n";
        std::cout << "2) Leer archivo BSF\n";
        std::cout << "3) Generar reporte de anomalias\n";
        std::cout << "4) Exportar pacientes con ECG anomalo (BIN)\n";
        std::cout << "0) Salir\n";
        std::cout << "Seleccione: ";
        std::cin >> opcion; cin.ignore(1024,'\n');

        if(opcion==1){
            delete[] umbrales; umbrales=nullptr;
            delete[] AP.pacientes; AP.pacientes=nullptr;
            nUmbrales = cargarUmbrales("data/configuracion.txt", umbrales);
            AP = cargarPacientes("data/pacientes.csv");
            cout << "Umbrales cargados: " << nUmbrales << "\n";
            cout << "Pacientes cargados: " << AP.numPacientes << "\n";
        }
        else if(opcion==2){
            if(sala.maquinas) liberarSala(sala);
            if(leerArchivoBSF("data/patient_readings_simulation 1.bsf", sala))
                std::cout<<"Archivo cargado correctamente.\n";
        }
        else if(opcion==3){
            if(sala.maquinas && umbrales){
                if(generarReporteAnomalias(sala, umbrales, nUmbrales))
                    std::cout<<"Reporte anomalias.txt generado.\n";
            } else {
                std::cout<<"Debe cargar configuracion y BSF primero.\n";
            }
        }
        else if(opcion==4){
            if(!sala.maquinas || !umbrales){
                std::cout<<"Cargue config (1) y BSF (2) primero.\n";
            } else {
                if (exportarPacientesECGAnomalos(sala, umbrales, nUmbrales))
                    std::cout<<"Archivo pacientes_ecg_anomalos.dat generado.\n";
                else
                    std::cout<<"No se pudo generar el binario (o no hay ECG anomalo).\n";
            }
        }
    } while(opcion!=0);

    delete[] umbrales;
    delete[] AP.pacientes;
    liberarSala(sala);
    return 0;
}
