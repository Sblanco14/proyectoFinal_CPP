#ifndef FUNCIONESCONFIG_H
#define FUNCIONESCONFIG_H

#include "umbral.h"
#include <cstring>
#include <cstdlib>
#include <string>

static inline std::string trimCfg(const std::string& s){
    size_t i=0, j=s.size();
    while (i<j && (s[i]==' '||s[i]=='\t')) ++i;
    while (j>i && (s[j-1]==' '||s[j-1]=='\t'||s[j-1]=='\r')) --j;
    return s.substr(i,j-i);
}

inline Umbral parseLineaConfiguracion(const char* linea) {
    Umbral u{}; 
    u.tipoSensor[0]='\0'; 
    u.min=0.0; 
    u.max=0.0;
    if (!linea) return u;

    std::string s(linea); s = trimCfg(s);
    if (s.empty() || s[0]=='#') return u;

    size_t p1 = s.find(','), p2 = (p1==std::string::npos)?std::string::npos:s.find(',', p1+1);
    std::string t   = (p1==std::string::npos)? s : s.substr(0,p1);
    std::string tmin= (p1==std::string::npos)? "" : s.substr(p1+1, (p2==std::string::npos? s.size():p2)-(p1+1));
    std::string tmax= (p2==std::string::npos)? "" : s.substr(p2+1);

    t   = trimCfg(t);  
    tmin=trimCfg(tmin);  
    tmax=trimCfg(tmax);

    std::strncpy(u.tipoSensor, t.c_str(), sizeof(u.tipoSensor)-1);
    u.tipoSensor[sizeof(u.tipoSensor)-1]='\0';
    u.min = std::atof(tmin.c_str());
    u.max = std::atof(tmax.c_str());
    return u;
}

#endif
