#include <stdio.h>
#include <stdlib.h>
#include "cge.h"
#include <windows.h>
#include <math.h>

int main (void) {
    struct cge_State s;
    int w = 400;
    int h = 200;
    cge_init(&s, w, h);

    int dy1, dy2;
    int y1 = h - 1;
    int y2 = 0;
    int a = 0;
    int rx = 150;
    int ry = 10;
    int r2x = 200;
    int r2y = 50;
    for (;;) {
        cgeD_rect_bounds_wh(&s, 0, 0, w, h, 0.0, 'O', FOREGROUND_BLUE);
        cgeD_line_xy(&s, 0, y1, w - 1, y2, 'x', FOREGROUND_RED);
        cgeD_triangle_bounds_wh(&s,
            100 + y2, 50,
            50, 50,
            a, '#', FOREGROUND_GREEN
        );
        cgeD_rect_bounds_wh(&s, y1, y2, 50, 50, a, '#', FOREGROUND_GREEN);
        cgeD_rect_wh(&s,
            rx, ry,
            20, 20,
            a, '#', FOREGROUND_RED
        );
        cgeD_rect_wh(&s,
            r2x, r2y,
            10, 10,
            a, '#', FOREGROUND_GREEN
        );
        const char *str = "test123";
        cgeT_text(&s, r2x - 200, r2y, str, CGE_FG_BRIGHTWHITE);

        CHAR arr[] = {
            'x',  0, 'x',
             0 , 'x', 0,
            'x',  0, 'x'
        };
        cgeD_array(&s, arr, 5, 5, 3, 3, CGE_BG_LIGHTRED);

        cgeD_line_xy(&s, 10, 10, 30, 10, 'x', CGE_BG_AQUA);

        if (y1 == 0)     dy1 = 1;
        if (y1 == h - 1) dy1 = -1;

        if (y2 == 0)     dy2 = 1;
        if (y2 == h - 1) dy2 = -1;
        
        if (GetKeyState(VK_ESCAPE) & 0x8000) {
            cge_next_frame(&s);
            break;
        }

        if (GetKeyState('W') & 0x8000) ry--;
        if (GetKeyState('S') & 0x8000) ry++;
        if (GetKeyState('A') & 0x8000) rx--;
        if (GetKeyState('D') & 0x8000) rx++;

        if (GetKeyState(VK_UP)    & 0x8000) r2y--;
        if (GetKeyState(VK_DOWN)  & 0x8000) r2y++;
        if (GetKeyState(VK_LEFT)  & 0x8000) r2x--;
        if (GetKeyState(VK_RIGHT) & 0x8000) r2x++;

        a = (a + 10) % 360;
        y1 += dy1;
        y2 += dy2;

        cge_next_frame(&s);
    }

    return EXIT_SUCCESS;
}