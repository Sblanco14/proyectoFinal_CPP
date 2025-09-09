#ifndef FUNCIONESFECHA_H
#define FUNCIONESFECHA_H

#include <cstdio>

inline bool compararFechas(const char* f1, const char* f2) {
    int d1, m1, a1, h1, min1, s1, ms1;
    int d2, m2, a2, h2, min2, s2, ms2;

    std::sscanf(f1, "%d/%d/%d %d:%d:%d.%d", &d1, &m1, &a1, &h1, &min1, &s1, &ms1);
    std::sscanf(f2, "%d/%d/%d %d:%d:%d.%d", &d2, &m2, &a2, &h2, &min2, &s2, &ms2);

    if (a1 != a2) return a1 > a2;
    if (m1 != m2) return m1 > m2;
    if (d1 != d2) return d1 > d2;
    if (h1 != h2) return h1 > h2;
    if (min1 != min2) return min1 > min2;
    if (s1 != s2) return s1 > s2;
    return ms1 > ms2;
}

#endif
