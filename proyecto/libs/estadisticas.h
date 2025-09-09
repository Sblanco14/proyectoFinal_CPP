#ifndef ESTADISTICAS_H
#define ESTADISTICAS_H

#include "lectura.h"
#include <cfloat>

struct StatsD { double min, max, sum; int cnt; };
struct StatsI { int min, max, sum; int cnt; };

inline void init(StatsD& s){ s.min= DBL_MAX; s.max= -DBL_MAX; s.sum=0.0; s.cnt=0; }
inline void init(StatsI& s){ s.min= 2147483647; s.max= -2147483647-1; s.sum=0; s.cnt=0; }

inline void acumular(StatsD& s, double v){ if(v<s.min) s.min=v; if(v>s.max) s.max=v; s.sum+=v; s.cnt++; }
inline void acumular(StatsI& s, int v){ if(v<s.min) s.min=v; if(v>s.max) s.max=v; s.sum+=v; s.cnt++; }

inline double promedio(const StatsD& s){ return s.cnt? (s.sum/s.cnt):0.0; }
inline double promedio(const StatsI& s){ return s.cnt? ((double)s.sum/s.cnt):0.0; }

#endif
