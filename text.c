#include "cge.h"
#include <assert.h>
#include <string.h>

// DONE
inline void cgeT_text (struct cge_State *s, int x, int y, const char* str, WORD attrib) {
    cgeT_textl(s, x, y, str, (int)strlen(str), attrib);
}

// CHECK
void cgeT_textl (struct cge_State *s, int x, int y, const char* str, int len, WORD attrib) {
    assert(s != NULL);
    assert(len > 0);

    if (y < 0)     return;
    if (y >= s->h) return;
    if (x <= -len) return;
    if (x >= s->w) return;

    int i = 0;
    COORD sz = {len, 1};
    if (x < 0) {
        i -= x;
        sz.X += x;
        x = 0;
    }
    else if (x > s->w - len) {
        len = s->w - x;
        sz.X = len;
    }

    SMALL_RECT rect = cge__WIN_canvas_rect(s, x, y, x + sz.X - 1, y);
    CHAR_INFO* char_info = cge__WIN_canvas_alloc(sz);
    for (int si = i; i < len; i++) {
        char_info[i - si].Char.AsciiChar = str[i];
        char_info[i - si].Attributes = attrib;
    }
    cge__WIN_canvas_draw(s, char_info, &rect);
    free(char_info);
}