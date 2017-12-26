#include "text_graphic.h"
#include "data_struct/list.h"
#include <stdarg.h>

#define BLANK ' '
#define DUMMY0 'h'
#define DUMMY1 'c'
#define DUMMY2 'n'

extern void game_init();
extern game_main();

struct monitor g_monitor;

int game_engine_init()
{
    int y, x, fd;

    lvmv_memory_init(&g_mem);

    fd = fileno(stdin);
    ioctl(fd, TIOCGWINSZ, &g_monitor.ws);

    g_monitor.pixel = (union pixel**)lvmv_malloc(&g_mem, sizeof(union pixel*) * g_monitor.ws.ws_row);

    for(y=0; y<g_monitor.ws.ws_row; y++)
    {
        g_monitor.pixel[y] = (union pixel*)lvmv_malloc(&g_mem, sizeof(union pixel) * g_monitor.ws.ws_col + 1);
    }

    _clear();

    list_init(&g_monitor.obj);

    WINDOW* w;
    w = initscr();
    nodelay(w, TRUE);

    keypad(stdscr, TRUE);

    has_colors();
    start_color();

    changemode(1);

    int color_index = 0;

#define xx(c1, c2)\
    init_pair(color_index++, COLOR_##c1, COLOR_##c2);
#include "text_graphic_color.h"

#undef xx

    game_init();

    return 0;
}

void _clear()
{
    int y, x;

    for(y=0; y<g_monitor.ws.ws_row; y++)
    {
        for(x=0; x<g_monitor.ws.ws_col; x++)
        {
            g_monitor.pixel[y][x].ch_pixel.ch = BLANK;
            g_monitor.pixel[y][x].ch_pixel.dummy[0] = DUMMY0;
            g_monitor.pixel[y][x].ch_pixel.dummy[1] = DUMMY1;
            g_monitor.pixel[y][x].ch_pixel.dummy[2] = DUMMY2;
            g_monitor.pixel[y][x].ch_pixel.attr = COLOR_PAIR(C_WHITE);
        }

        g_monitor.pixel[y][x].ch_pixel.ch = '\0';
        g_monitor.pixel[y][x].ch_pixel.dummy[0] = DUMMY0;
        g_monitor.pixel[y][x].ch_pixel.dummy[1] = DUMMY1;
        g_monitor.pixel[y][x].ch_pixel.dummy[2] = DUMMY2;
        g_monitor.pixel[y][x].ch_pixel.attr = COLOR_PAIR(C_WHITE);
    }

    return;
}

void invalidate()
{
    int y, x;

    for(y=0; y<g_monitor.ws.ws_row; y++)
    {
        for(x=0; x<g_monitor.ws.ws_col; x++)
        {
            attrset(g_monitor.pixel[y][x].ch_pixel.attr);
            mvprintw(y, x, "%c", g_monitor.pixel[y][x].ch_pixel.ch);
        }
    }

    refresh();

    _clear();

    return;
}

int textoutc(int x, int y, int color, int attr, char* format, ...)
{
    va_list va;
    char tmp[g_monitor.ws.ws_col + 1];
    char* pch;
    int ret = 0;

    memset(tmp, 0x00, g_monitor.ws.ws_col+1);

    va_start(va, format);
    vsprintf(tmp, format, va);
    va_end(va);

    pch = tmp;

    for(; *pch && g_monitor.pixel[y][x].ch_pixel.ch; pch++)
    {
        g_monitor.pixel[y][x].ch_pixel.attr = COLOR_PAIR(color) | attr;
        g_monitor.pixel[y][x++].ch_pixel.ch = *pch;
        ret++;
    }

    return ret;
}

void rectanglec(int sx, int sy, int dx, int dy, int color)
{
    int i;

    textoutc(sx, sy, color, 0, "+");
    textoutc(dx, sy, color, 0, "+");
    textoutc(sx, dy, color, 0, "+");
    textoutc(dx, dy, color, 0, "+");

    for(i=sx+1; i<dx; i++)
    {
        textoutc(i, sy, color, 0, "-");
        textoutc(i, dy, color, 0, "-");
    }

    for(i=sy+1; i<dy; i++)
    {
        textoutc(sx, i, color, 0, "!");
        textoutc(dx, i, color, 0, "!");
    }

    return;
}

void fillrectc(int sx, int sy, int dx, int dy, int color)
{
    int i, j;

    if(color < 9)
        color *= 9;

    for(i=sy; i<=dy; i++)
    {
        for(j=sx; j<=dx; j++)
        {
            textoutc(j, i, color, 0, " ");
        }
    }

    return;
}

void object_add(struct object *obj)
{
    list_add(&g_monitor.obj, obj);

    return;
}

void object_del(struct object *obj)
{
    list_del(obj);
}

void _draw()
{
    struct object *v;

    list_iterate(v, &g_monitor.obj)
    {
        v->draw(v);
    }
}

void _control()
{
    struct object *v;

    game_main();

    list_iterate(v, &g_monitor.obj)
    {
        v->ctr(v);
    }
}

int _key_press(int ch)
{
    struct object *v;

    list_iterate(v, &g_monitor.obj)
    {
        if(v->key_press(v, ch))
            return 1;
    }

    return 0;
}

void game_destroy()
{
    struct object *v, *t;
    int y;

    changemode(0);
    for(y=0; y<g_monitor.ws.ws_row; y++)
        lvmv_free(&g_mem, g_monitor.pixel[y]);

    lvmv_free(&g_mem, g_monitor.pixel);

    list_iterate_safe(v, t, &g_monitor.obj)
    {
        list_del(v);
        v->dst(v);
    }

    endwin();

    lvmv_memory_print(&g_mem);
    lvmv_memory_destroy(&g_mem);
}

int screen_y()
{
    return g_monitor.ws.ws_row;
}

int screen_x()
{
    return g_monitor.ws.ws_col;
}

int screen_isy(int y)
{
    return (0 <= y) && (y < screen_y());
}

int screen_isx(int x)
{
    return (0 <= x) && (x < screen_x());
}
