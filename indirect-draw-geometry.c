#include "cge.h"
#include <assert.h>
#include <math.h>

///
/// Rect
///

// DONE
void cgeD_rect_bounds_wh (struct cge_State *s, int x, int y, int w, int h, double deg, CHAR c, WORD attrib) {
    assert(s != NULL);

    double cx = x + w / 2.0;
    double cy = y + h / 2.0;

    double x1 = x;
    double y1 = y;
    double x2 = x + w - 1;
    double y2 = y;
    double x3 = x + w - 1;
    double y3 = y + h - 1;
    double x4 = x;
    double y4 = y + h - 1;

    double rx1, ry1;
    double rx2, ry2;
    double rx3, ry3;
    double rx4, ry4;

    cgeM_rotate_xy(deg, x1, y1, cx, cy, &rx1, &ry1);
    cgeM_rotate_xy(deg, x2, y2, cx, cy, &rx2, &ry2);
    cgeM_rotate_xy(deg, x3, y3, cx, cy, &rx3, &ry3);
    cgeM_rotate_xy(deg, x4, y4, cx, cy, &rx4, &ry4);

    cgeD_line_xy(s, rx1, ry1, rx2, ry2, c, attrib);
    cgeD_line_xy(s, rx2, ry2, rx3, ry3, c, attrib);
    cgeD_line_xy(s, rx3, ry3, rx4, ry4, c, attrib);
    cgeD_line_xy(s, rx4, ry4, rx1, ry1, c, attrib);
}

// DONE
//# - (x1, y1) top-left
//# - (x2, y2) bot-right
inline void cgeD_rect_bounds_xy (struct cge_State *s, int x1, int y1, int x2, int y2, double deg, CHAR c, WORD attrib) {
    cgeD_rect_bounds_wh(s, x1, y1, x2 - x1 + 1, y2 - y1 + 1, deg, c, attrib);
}

///
/// Triangle
///

// DONE
void cgeD_triangle_wh (struct cge_State *s, int x, int y, int w, int h, double deg, CHAR c, WORD attrib) {
    int x1 = x;
    int y1 = y;
    int x2 = x + w/2 - 1;
    int y2 = y - h/2 - 1;
    int x3 = x + w - 1;
    int y3 = y;
    cgeD_triangle_xy(s, x1, y1, x2, y2, x3, y3, deg, c, attrib);
}

// DONE
static void rotate_triangle_vertices_xy (
    double deg,
    double   x1, double   y1, double   x2, double   y2, double   x3, double   y3,
    double *rx1, double *ry1, double *rx2, double *ry2, double *rx3, double *ry3
) {
    double cx = (x1 + x2 + x3) / 3.0;
    double cy = (y1 + y2 + y3) / 3.0;
    cgeM_rotate_xy(deg, x1, y1, cx, cy, rx1, ry1);
    cgeM_rotate_xy(deg, x2, y2, cx, cy, rx2, ry2);
    cgeM_rotate_xy(deg, x3, y3, cx, cy, rx3, ry3);
}

// DONE
//#
//#    2
//#   / \
//#  /   \
//# 1-----3
//#
static void rotate_triangle_vertices_wh (
    double deg,
    double x, double y, double w, double h,
    double *rx1, double *ry1, double *rx2, double *ry2, double *rx3, double *ry3
) {
    double x1 = x;
    double y1 = y;
    double x2 = x + w / 2.0 - 1;
    double y2 = y - h / 2.0 - 1;
    double x3 = x + w - 1;
    double y3 = y;
    rotate_triangle_vertices_xy(deg,
         x1,  y1,  x2,  y2,  x3,  y3,
        rx1, ry1, rx2, ry2, rx3, ry3  
    );
}

// DONE
void cgeD_triangle_bounds_wh (struct cge_State *s, int x, int y, int w, int h, double deg, CHAR c, WORD attrib) {
    double rx1, ry1;
    double rx2, ry2;
    double rx3, ry3;
    rotate_triangle_vertices_wh(deg,
        x, y, w, h,
        &rx1, &ry1, &rx2, &ry2, &rx3, &ry3
    );
    cgeD_line_xy(s, rx1, ry1, rx2, ry2, c, attrib);
    cgeD_line_xy(s, rx2, ry2, rx3, ry3, c, attrib);
    cgeD_line_xy(s, rx3, ry3, rx1, ry1, c, attrib);
}