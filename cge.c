#include "cge.h"
#include <assert.h>
#include <stdlib.h>
#include <math.h>

/*
 *
 * CGE
 * 
 */

// DONE
//#
//# Initialization
//#
void cge_init (struct cge_State *s, int w, int h) {
    assert(s != NULL);
    assert(w > 0);
    assert(h > 0);

    s->w = w;
    s->h = h;
    s->cmd = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(s->cmd, &csbi);
    s->canvas_front = csbi.dwCursorPosition;
    s->canvas_back.X = s->canvas_front.X;
    s->canvas_back.Y = s->canvas_front.Y + h + 2;
}

// CHECK
void cge_next_frame (struct cge_State *s) {
    assert(s != NULL);

    COORD sz = {s->w, s->h};
    COORD pos = {0, 0};
    CHAR_INFO* char_info = malloc(sizeof(*char_info) * s->w * s->h);
    SMALL_RECT rect = {
        s->canvas_back.X + 0,
        s->canvas_back.Y + 0,
        s->canvas_back.X + s->w - 1,
        s->canvas_back.Y + s->h - 1
    };
    ReadConsoleOutput(s->cmd, char_info, sz, pos, &rect);
    rect = (SMALL_RECT){
        s->canvas_front.X + 0,
        s->canvas_front.Y + 0,
        s->canvas_front.X + s->w - 1,
        s->canvas_front.Y + s->h - 1
    };
    WriteConsoleOutput(s->cmd, char_info, sz, pos, &rect);

    cgeD_rect_wh(s, 0, 0, s->w, s->h, 0.0, ' ', 0);
}

// FIX
//#
//# Retrieve the character at specified X and Y coordinates
//#
CHAR_INFO cge_at (struct cge_State *s, int x, int y) {
    assert(s != NULL);

    CHAR_INFO c;
    COORD sz = {1, 1};
    COORD pos = {0, 0};
    SMALL_RECT rect = {x, y, x, y};
    ReadConsoleOutput(s->cmd, &c, sz, pos, &rect);

    return c;
}