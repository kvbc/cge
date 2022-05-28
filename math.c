#include "cge.h"
#include <math.h>

inline double cgeM_rad (double deg) {
    return deg * (CGE_PI / 180.0);
}

inline double cgeM_deg (double rad) {
    return rad * (180.0 / CGE_PI);
}

void cgeM_rotate_xy (double deg, double x, double y, double cx, double cy, double *rx, double *ry) {
    double rad = cgeM_rad(deg);
    x -= cx;
    y -= cy;
    *rx = cx + x * cos(rad) - y * sin(rad);
    *ry = cy + x * sin(rad) + y * cos(rad);
}

inline void cgeM_rotate_xy_int (double deg, double x, double y, double cx, double cy, int *rx, int *ry) {
    double rx_, ry_;
    cgeM_rotate_xy(deg, x, y, cx, cy, &rx_, &ry_);
    *rx = rx_;
    *ry = ry_;
}

inline int cgeM__minint  (int a, int b)               { return a < b ? a : b; }
inline int cgeM__maxint  (int a, int b)               { return a > b ? a : b; }
inline int cgeM__minint3 (int a, int b, int c)        { return cgeM__minint(cgeM__minint(a, b), c); }
inline int cgeM__maxint3 (int a, int b, int c)        { return cgeM__maxint(cgeM__maxint(a, b), c); }
inline int cgeM__minint4 (int a, int b, int c, int d) { return cgeM__minint(cgeM__minint3(a, b, c), d); }
inline int cgeM__maxint4 (int a, int b, int c, int d) { return cgeM__maxint(cgeM__maxint3(a, b, c), d); }

inline int cgeM__clampx (struct cge_State *s, int x) {
    return cgeM__minint(cgeM__maxint(0, x), s->w - 1);
}

inline int cgeM__clampy (struct cge_State *s, int y) {
    return cgeM__minint(cgeM__maxint(0, y), s->h - 1); 
}