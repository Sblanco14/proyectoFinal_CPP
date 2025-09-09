#ifndef CARGARDATOS_H
#define CARGARDATOS_H

#include "umbral.h"
#include "paciente.h"
#include "archivo_pacientes.h"
#include "funcionesConfig.h"
#include "funcionesPacientes.h"
#include <fstream>
#include <iostream>
#include <string>

inline int cargarUmbrales(const char* nombreArchivo, Umbral*& umbrales) {
    umbrales = nullptr;
    int count = 0, cap = 8;
    Umbral* temp = new Umbral[cap];

    std::ifstream in(nombreArchivo);
    if (!in) {
        std::cerr << "No se pudo abrir archivo de configuracion: " << nombreArchivo << "\n";
        delete[] temp;
        return 0;
    }
    std::string linea;
    while (std::getline(in, linea)) {
        if (!linea.empty() && linea[0] == '#') continue;
        bool vacia = true;
        for (char c : linea) if (c!=' ' && c!='\t' && c!='\r') { vacia=false; break; }
        if (vacia) continue;

        Umbral u = parseLineaConfiguracion(linea.c_str());
        if (u.tipoSensor[0] == '\0') continue;

        if (count == cap) {
            cap *= 2;
            Umbral* nuevo = new Umbral[cap];
            for (int i=0;i<count;++i) nuevo[i]=temp[i];
            delete[] temp; temp=nuevo;
        }
        temp[count++] = u;
    }
    in.close();
    if (count==0) { delete[] temp; return 0; }
    umbrales = new Umbral[count];
    for (int i=0;i<count;++i) umbrales[i]=temp[i];
    delete[] temp;
    return count;
}

inline ArchivoPacientes cargarPacientes(const char* nombreArchivo) {
    ArchivoPacientes ap{}; ap.numPacientes=0; ap.pacientes=nullptr;

    std::ifstream in(nombreArchivo);
    if (!in) { std::cerr<<"No se pudo abrir archivo de pacientes: "<<nombreArchivo<<"\n"; return ap; }

    int count=0, cap=8;
    Paciente* temp = new Paciente[cap];
    std::string linea;
    while (std::getline(in, linea)) {
        if (!linea.empty() && linea[0]=='#') continue;
        bool vacia=true; for (char c:linea) if (c!=' '&&c!='\t'&&c!='\r'){vacia=false;break;}
        if (vacia) continue;

        Paciente p = parseLineaPaciente(linea.c_str());
        if (p.id[0]=='\0') continue;

        if (count==cap){ cap*=2; Paciente* nuevo=new Paciente[cap]; for(int i=0;i<count;++i) nuevo[i]=temp[i]; delete[] temp; temp=nuevo; }
        temp[count++] = p;
    }
    in.close();
    if (count==0){ delete[] temp; return ap; }

    ap.numPacientes=count; ap.pacientes=new Paciente[count];
    for (int i=0;i<count;++i) ap.pacientes[i]=temp[i];
    delete[] temp;
    return ap;
}

#endif
