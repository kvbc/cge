#include "cge.h"
#include <Windows.h>
#include <stdlib.h>
#include <assert.h>

// DONE
//#
//# Rect relative to the back canvas
//# (x1, y1) top-left
//# (x2, y2) bot-right
//#
inline SMALL_RECT cge__WIN_canvas_rect (struct cge_State *s, SHORT x1, SHORT y1, SHORT x2, SHORT y2) {
    assert(s != NULL);
    return (SMALL_RECT) {
        s->canvas_back.X + x1,
        s->canvas_back.Y + y1,
        s->canvas_back.X + x2,
        s->canvas_back.Y + y2 
    };
}

// DONE
//#
//# Rect to size (COORD){width, height}
//#
inline COORD cge__WIN_rect_to_sz (PSMALL_RECT rectPtr) {
    assert(rectPtr != NULL);
    return (COORD) {
        abs(rectPtr->Right - rectPtr->Left) + 1,
        abs(rectPtr->Bottom - rectPtr->Top) + 1
    };
}

// DONE
//#
//# Allocate canvas of width {sz.X} and height {sz.Y}
//#
inline CHAR_INFO* cge__WIN_canvas_alloc (COORD sz) {
    assert(sz.X > 0);
    assert(sz.Y > 0);
    CHAR_INFO* char_info = malloc(sizeof(*char_info) * sz.X * sz.Y);
    assert(char_info != NULL);
    return char_info;
}

// DONE
//#
//# Get CHAR_INFO of canvas at given offset
//# {ofsx, ofsy} is the offset from rect {rectPtr}
//#
inline CHAR_INFO* cge__WIN_canvas_at (struct cge_State *s, CHAR_INFO* char_info, PSMALL_RECT rectPtr, SHORT ofsx, SHORT ofsy) {
    assert(s != NULL);
    assert(char_info != NULL);
    assert(rectPtr != NULL);
    COORD sz = cge__WIN_rect_to_sz(rectPtr);
    CGE__ASSERT_XY(s, ofsx, ofsy);
    ofsx = ofsx + s->canvas_back.X - rectPtr->Left;
    ofsy = ofsy + s->canvas_back.Y - rectPtr->Top;
    return &char_info[sz.X * ofsy + ofsx];
}

// DONE
//#
//# Read rect {rectPtr} from console to {char_info}
//#
inline void cge__WIN_canvas_read (struct cge_State *s, CHAR_INFO *char_info, PSMALL_RECT rectPtr) {
    assert(s != NULL);
    assert(char_info != NULL);
    assert(rectPtr != NULL);
    COORD pos = {0, 0};
    COORD sz = cge__WIN_rect_to_sz(rectPtr);
    ReadConsoleOutput(s->cmd, char_info, sz, pos, rectPtr);
}

// DONE
//#
//# Write rect {rectPtr} from {char_info} to console
//#
inline void cge__WIN_canvas_draw (struct cge_State *s, CHAR_INFO *char_info, PSMALL_RECT rectPtr) {
    assert(s != NULL);
    assert(char_info != NULL);
    assert(rectPtr != NULL);
    COORD pos = {0, 0};
    COORD sz = cge__WIN_rect_to_sz(rectPtr);
    WriteConsoleOutput(s->cmd, char_info, sz, pos, rectPtr);
}