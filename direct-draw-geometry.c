#include "cge.h"
#include <assert.h>
#include <math.h>
#include <stdlib.h>

// DONE
inline void cgeD_xy (struct cge_State *s, int x, int y, CHAR c, WORD attrib) {
    assert(s != NULL);
    CGE__ASSERT_XY(s, x, y);
    cgeD_rect_wh(s, x, y, 1, 1, 0.0, c, attrib);
}

///
/// Line
///

// FIX
//# to be optimized
void cgeD_line_xy (struct cge_State *s, int x1, int y1, int x2, int y2, CHAR c, WORD attrib) {
    assert(s != NULL);

    /*
    if (
        (x1 == x2) ||
        (y1 == y2)
    ) {
        cge__WIN_draw_rect_xy(s, x1, y1, x2, y2, c, attrib);
        return;
    }
    */

    double len = cgeM__maxint(
        abs(x2 - x1),
        abs(y2 - y1)
    );
    double dx = (x2 - x1) / len;
    double dy = (y2 - y1) / len;
    double x = x1;
    double y = y1;

    for (int i = 0; i <= (int)len; i++) {
        int rx = round(x);
        int ry = round(y);

        if (rx >= 0)
        if (rx < s->w)
        if (ry >= 0)
        if (ry < s->h) {
            cgeD_xy(s, rx, ry, c, attrib);
        }

        x += dx;
        y += dy;
    }
}

///
/// Triangle
///

// DONE
static double triangle_area (int x1, int y1, int x2, int y2, int x3, int y3) {
    return fabs( (x1*(y2-y3) + x2*(y3-y1) + x3*(y1-y2)) / 2.0 );
}

// DONE
void cgeD_triangle_xy (struct cge_State *s, int x1, int y1, int x2, int y2, int x3, int y3, double deg, CHAR c, WORD attrib) {
    assert(s != NULL);

    if (deg != 0.0) {
        double cx = (x1 + x2 + x3) / 3.0;
        double cy = (y1 + y2 + y3) / 3.0;
        cgeM_rotate_xy_int(deg, x1, y1, cx, cy, &x1, &y1);
        cgeM_rotate_xy_int(deg, x2, y2, cx, cy, &x2, &y2);
        cgeM_rotate_xy_int(deg, x3, y3, cx, cy, &x3, &y3);
    }

    int min_x = cgeM__clampx(s, cgeM__minint3(x1, x2, x3));
    int max_x = cgeM__clampx(s, cgeM__maxint3(x1, x2, x3));
    int min_y = cgeM__clampy(s, cgeM__minint3(y1, y2, y3));
    int max_y = cgeM__clampy(s, cgeM__maxint3(y1, y2, y3));

    SMALL_RECT rect = cge__WIN_canvas_rect(s, min_x, min_y, max_x, max_y);
    COORD sz = cge__WIN_rect_to_sz(&rect);
    CHAR_INFO* char_info = cge__WIN_canvas_alloc(sz);
    cge__WIN_canvas_read(s, char_info, &rect);

    for (int x = min_x; x <= max_x; x++)
    for (int y = min_y; y <= max_y; y++) {
        float area_ABC = triangle_area(x1, y1, x2, y2, x3, y3);
        float area_PBC = triangle_area(x,  y,  x2, y2, x3, y3);
        float area_PAC = triangle_area(x1, y1, x,  y,  x3, y3);
        float area_PAB = triangle_area(x1, y1, x2, y2, x,  y);
        if (area_ABC == area_PBC + area_PAC + area_PAB) { // is point inside triangle
            CHAR_INFO *ci = cge__WIN_canvas_at(s, char_info, &rect, x, y);
            ci->Char.AsciiChar = c;
            ci->Attributes = attrib;
        }
    }

    cge__WIN_canvas_draw(s, char_info, &rect);
    free(char_info);
}

///
/// Rect
///

// FIX
void cgeD_rect_wh (struct cge_State *s, int x, int y, int w, int h, double deg, CHAR c, WORD attrib) {
    assert(s != NULL);

    int x1 = x;
    int y1 = y;
    int x2 = x + w - 1;
    int y2 = y + h - 1;
    int x3 = x + w - 1;
    int y3 = y;
    int x4 = x;
    int y4 = y + h - 1;

    if (deg != 0.0) {
        cgeD_quad_xy(s, x1, y1, x3, y3, x2, y2, x4, y4, deg, c, attrib);
        return;
    }

    x1 = cgeM__clampx(s, cgeM__minint(x1, x2));
    y1 = cgeM__clampy(s, cgeM__minint(y1, y2));
    x2 = cgeM__clampx(s, cgeM__maxint(x1, x2));
    y2 = cgeM__clampy(s, cgeM__maxint(y1, y2));

    SMALL_RECT rect = cge__WIN_canvas_rect(s, x1, y1, x2, y2);
    COORD sz = cge__WIN_rect_to_sz(&rect);
    CHAR_INFO *char_info = cge__WIN_canvas_alloc(sz);

    for (int i = 0; i < abs(sz.X * sz.Y); i++) {
        char_info[i].Char.AsciiChar = c;
        char_info[i].Attributes = attrib;
    }

    cge__WIN_canvas_draw(s, char_info, &rect);
    free(char_info);
}

///
/// Quad
///

// DONE
static inline float quad_is_left(int x1, int y1, int x2, int y2, int x3, int y3) {
    return ( (x2 - x1) * (y3 - y1) - (x3 - x1) * (y2 - y1) );
}

// DONE
static BOOL point_is_in_quad (int x, int y, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) {
    if (quad_is_left(x1, y1, x2, y2, x, y) > 0)
    if (quad_is_left(x2, y2, x3, y3, x, y) > 0)
    if (quad_is_left(x3, y3, x4, y4, x, y) > 0)
    if (quad_is_left(x4, y4, x1, y1, x, y) > 0)
        return TRUE;
    return FALSE;
}

// FIX
//# optimize for (deg == 0)
void cgeD_quad_xy (struct cge_State *s, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, double deg, CHAR c, WORD attrib) {
    assert(s != NULL);
    
    if (deg != 0.0) {
        double cx = (x1 + x2 + x3 + x4) / 4.0;
        double cy = (y1 + y2 + y3 + y4) / 4.0;
        cgeM_rotate_xy_int(deg, x1, y1, cx, cy, &x1, &y1);
        cgeM_rotate_xy_int(deg, x2, y2, cx, cy, &x2, &y2);
        cgeM_rotate_xy_int(deg, x3, y3, cx, cy, &x3, &y3);
        cgeM_rotate_xy_int(deg, x4, y4, cx, cy, &x4, &y4);
    }

    int min_x = cgeM__clampx(s, cgeM__minint4(x1, x2, x3, x4));
    int max_x = cgeM__clampx(s, cgeM__maxint4(x1, x2, x3, x4));
    int min_y = cgeM__clampy(s, cgeM__minint4(y1, y2, y3, y4));
    int max_y = cgeM__clampy(s, cgeM__maxint4(y1, y2, y3, y4));

    SMALL_RECT rect = cge__WIN_canvas_rect(s, min_x, min_y, max_x, max_y);
    COORD sz = cge__WIN_rect_to_sz(&rect);
    CHAR_INFO* char_info = cge__WIN_canvas_alloc(sz);
    cge__WIN_canvas_read(s, char_info, &rect);

    for (int x = min_x; x <= max_x; x++)
    for (int y = min_y; y <= max_y; y++) {
        if (point_is_in_quad(x, y, x1, y1, x2, y2, x3, y3, x4, y4)) {
            CHAR_INFO *ci = cge__WIN_canvas_at(s, char_info, &rect, x, y);
            ci->Char.AsciiChar = c;
            ci->Attributes = attrib;
        }
    }

    cge__WIN_canvas_draw(s, char_info, &rect);
    free(char_info);
}