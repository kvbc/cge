#ifndef CGE_H
#define CGE_H

#include <windows.h>
#include <assert.h>

///
/// Defines
///

#define CGE_PI 3.1415926535

#define CGE__ASSERT_X(s_, x_) (void)(   \
            (assert((x_) >= 0)),        \
            (assert((x_) < (s_)->w))    \
        )

#define CGE__ASSERT_Y(s_, y_) (void)(   \
            (assert((y_) >= 0)),        \
            (assert((y_) < (s_)->h))    \
        )

#define CGE__ASSERT_XY(s_, x_, y_) (void)(  \
            CGE__ASSERT_X((s_), (x_)),      \
            CGE__ASSERT_Y((s_), (y_))       \
        )

#define CGE__ASSERT_XXYY(s_, x1_, y1_, x2_, y2_) (void)(    \
            (CGE__ASSERT_XY((s_), (x1_), (y1_))),           \
            (CGE__ASSERT_XY((s_), (x2_), (y2_)))            \
        )

#define CGE_FG_BLACK       (0)
#define CGE_FG_RED         (FOREGROUND_RED)
#define CGE_FG_GREEN       (FOREGROUND_GREEN)
#define CGE_FG_BLUE        (FOREGROUND_BLUE)
#define CGE_FG_GOLD        (FOREGROUND_RED | FOREGROUND_GREEN)
#define CGE_FG_AQUA        (FOREGROUND_GREEN | FOREGROUND_BLUE)
#define CGE_FG_PURPLE      (FOREGROUND_BLUE | FOREGROUND_RED)
#define CGE_FG_WHITE       (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)
#define CGE_FG_GRAY        (FOREGROUND_INTENSITY)
#define CGE_FG_LIGHTRED    (FOREGROUND_INTENSITY | CGE_FG_RED)
#define CGE_FG_LIGHTGREEN  (FOREGROUND_INTENSITY | CGE_FG_GREEN)
#define CGE_FG_LIGHTBLUE   (FOREGROUND_INTENSITY | CGE_FG_BLUE)
#define CGE_FG_LIGHTAQUA   (FOREGROUND_INTENSITY | CGE_FG_AQUA)
#define CGE_FG_LIGHTPURPLE (FOREGROUND_INTENSITY | CGE_FG_PURPLE)
#define CGE_FG_YELLOW      (FOREGROUND_INTENSITY | CGE_FG_GOLD)
#define CGE_FG_BRIGHTWHITE (FOREGROUND_INTENSITY | CGE_FG_WHITE)

#define CGE_BG_BLACK       (0)
#define CGE_BG_RED         (BACKGROUND_RED)
#define CGE_BG_GREEN       (BACKGROUND_GREEN)
#define CGE_BG_BLUE        (BACKGROUND_BLUE)
#define CGE_BG_GOLD        (BACKGROUND_RED | BACKGROUND_GREEN)
#define CGE_BG_AQUA        (BACKGROUND_GREEN | BACKGROUND_BLUE)
#define CGE_BG_PURPLE      (BACKGROUND_BLUE | BACKGROUND_RED)
#define CGE_BG_WHITE       (BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE)
#define CGE_BG_GRAY        (BACKGROUND_INTENSITY)
#define CGE_BG_LIGHTRED    (BACKGROUND_INTENSITY | CGE_BG_RED)
#define CGE_BG_LIGHTGREEN  (BACKGROUND_INTENSITY | CGE_BG_GREEN)
#define CGE_BG_LIGHTBLUE   (BACKGROUND_INTENSITY | CGE_BG_BLUE)
#define CGE_BG_LIGHTAQUA   (BACKGROUND_INTENSITY | CGE_BG_AQUA)
#define CGE_BG_LIGHTPURPLE (BACKGROUND_INTENSITY | CGE_BG_PURPLE)
#define CGE_BG_YELLOW      (BACKGROUND_INTENSITY | CGE_BG_GOLD)
#define CGE_BG_BRIGHTWHITE (BACKGROUND_INTENSITY | CGE_BG_WHITE)

///
/// CGE
///

struct cge_State {
    int w, h;
    HANDLE cmd;
    COORD canvas_front;
    COORD canvas_back;
};

void cge_init (struct cge_State *s, int w, int h);

CHAR_INFO cge_at (struct cge_State *s, int x, int y);
void cge_next_frame (struct cge_State *s);

///
/// Windows
///
// DONE

SMALL_RECT cge__WIN_canvas_rect  (struct cge_State *s, SHORT x1, SHORT y1, SHORT x2, SHORT y2);
COORD      cge__WIN_rect_to_sz   (PSMALL_RECT rectPtr);
CHAR_INFO* cge__WIN_canvas_alloc (COORD sz);
CHAR_INFO* cge__WIN_canvas_at    (struct cge_State *s, CHAR_INFO* char_info, SMALL_RECT* rectPtr, SHORT ofsx, SHORT ofsy);
void       cge__WIN_canvas_read  (struct cge_State *s, CHAR_INFO *char_info, SMALL_RECT* rectPtr);
void       cge__WIN_canvas_draw  (struct cge_State *s, CHAR_INFO *char_info, SMALL_RECT* rectPtr);

///
/// Indirect draw
///
// CHECK

// Geometry
void cgeD_rect_bounds_wh     (struct cge_State *s, int x, int y, int w, int h, double deg, CHAR c, WORD attrib);
void cgeD_rect_bounds_xy     (struct cge_State *s, int x1, int y1, int x2, int y2, double deg, CHAR c, WORD attrib);
void cgeD_triangle_wh        (struct cge_State *s, int x, int y, int w, int h, double deg, CHAR c, WORD attrib);
void cgeD_triangle_bounds_wh (struct cge_State *s, int x, int y, int w, int h, double deg, CHAR c, WORD attrib);

///
/// Direct draw
///

// Geometry
void cgeD_xy          (struct cge_State *s, int x , int y,                                                              CHAR c, WORD attrib);
void cgeD_line_xy     (struct cge_State *s, int x1, int y1, int x2, int y2,                                             CHAR c, WORD attrib);
void cgeD_triangle_xy (struct cge_State *s, int x1, int y1, int x2, int y2, int x3, int y3,                 double deg, CHAR c, WORD attrib);
void cgeD_rect_wh     (struct cge_State *s, int x,  int y,  int w,  int h,                                  double deg, CHAR c, WORD attrib);
void cgeD_quad_xy     (struct cge_State *s, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, double deg, CHAR c, WORD attrib);

void cgeD_array       (struct cge_State *s, CHAR arr[], int x, int y, int w, int h, WORD attrib);

///
/// Math
///
// DONE

double cgeM_deg (double rad);
double cgeM_rad (double deg);
void cgeM_rotate_xy     (double deg, double x, double y, double cx, double cy, double *rx, double *ry);
void cgeM_rotate_xy_int (double deg, double x, double y, double cx, double cy, int *rx, int *ry);

int cgeM__minint  (int a, int b);              
int cgeM__maxint  (int a, int b);              
int cgeM__minint3 (int a, int b, int c);       
int cgeM__maxint3 (int a, int b, int c);       
int cgeM__minint4 (int a, int b, int c, int d);
int cgeM__maxint4 (int a, int b, int c, int d);

int cgeM__clampx (struct cge_State *s, int x);
int cgeM__clampy (struct cge_State *s, int y);

///
/// Text
///
// CHECK

void cgeT_text  (struct cge_State *s, int x, int y, const char* str, WORD attrib);
void cgeT_textl (struct cge_State *s, int x, int y, const char* str, int len, WORD attrib);

#endif // CGE_H