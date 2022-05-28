#include "cge.h"

// CHECK
void cgeD_array (struct cge_State *s, CHAR arr[], int x, int y, int w, int h, WORD attrib) {
    assert(s != NULL);
    assert(arr != NULL);

    int x1 = x;
    int y1 = y;
    int x2 = x + w - 1;
    int y2 = y + h - 1;

    x1 = cgeM__clampx(s, x1);
    y1 = cgeM__clampy(s, y1);
    x2 = cgeM__clampx(s, x2);
    y2 = cgeM__clampy(s, y2);

    COORD sz = {w, h};
    SMALL_RECT rect = cge__WIN_canvas_rect(s, x1, y1, x2, y2);
    CHAR_INFO *char_info = cge__WIN_canvas_alloc(sz);
    cge__WIN_canvas_read(s, char_info, &rect);

    for (int i = 0; i < abs(w * h); i++) {
        char_info[i].Attributes = attrib;
        if (arr[i] != 0)
            char_info[i].Char.AsciiChar = arr[i];
    }

    cge__WIN_canvas_draw(s, char_info, &rect);
    free(char_info);
}