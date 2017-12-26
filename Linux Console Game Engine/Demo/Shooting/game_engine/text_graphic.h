#ifndef _TEXT_GRAPHIC_H_
#define _TEXT_GRAPHIC_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <curses.h>
#include <game_engine/lib/lvmv_memory.h>
#include "kbhit.h"

#define xx(c1, c2) \
    C_##c1##_##c2,

enum
{
#include "text_graphic_color.h"
    C_MAX_COLOR
};

#undef xx

#define C_BLACK     C_BLACK_BLACK
#define C_RED       C_RED_BLACK
#define C_GREEN     C_GREEN_BLACK
#define C_YELLOW    C_YELLOW_BLACK
#define C_BLUE      C_BLUE_BLACK
#define C_MAGENTA   C_MAGENTA_BLACK
#define C_CYAN      C_CYAN_BLACK
#define C_WHITE     C_WHITE_BLACK

struct lvmv_memory g_mem;

struct object;
struct object_list;

typedef void (draw_f)(struct object *obj);
typedef int (key_press_f)(struct object *obj, int ch);
typedef void (object_ctr_f)(struct object *obj);
typedef void (object_dst_f)(struct object *obj);
typedef void (conflict_f)(struct object *obj, struct object *target);

struct object
{
    struct object *n, *p;
    draw_f *draw;
    key_press_f *key_press;
    object_ctr_f *ctr;
    object_dst_f *dst;
    conflict_f *conf;
    int type;
    void *private;
};

struct object_list
{
    struct object_list *n, *p;
    struct object *obj;
};

struct pixel
{
    union
    {
        struct
        {
            char ch;
            char dummy[3];
            int attr;
        } ch_pixel;
        unsigned long long int ull_pixel;
    };
    struct object_list owners;
    int owners_cnt;
};

struct monitor
{
    struct winsize ws;
    struct pixel** pixel;
    struct object obj;
};

#define gotoxy(x, y) wmove(stdscr, y-1, x-1)

int game_engine_init();
void _clear();
void invalidate();
int textoutc(struct object *owner, int x, int y, int color, int attr, char *format, ...);
#define textout(owner, x, y, format, args...)\
    textoutc(owner, x, y, C_WHITE, 0, format, ##args)

void rectanglec(struct object *owner, int sx, int sy, int dx, int dy, int color);
#define rectangle(owner, sx, sy, dx, dy)\
    rectanglec(owner, sx, sy, dx, dy, C_BLACK)

void fillrectc(struct object *owner, int sx, int sy, int dx, int dy, int color);
#define fillrect(owner, sx, sy, dx, dy)\
    fillrectc(owner, sx, sy, dx, dy, C_WHITE)

void object_add(struct object *obj);
void object_del(struct object *obj);
void _draw();
void _control();
int _key_press(int ch);
void _conflict();
void game_destroy();
int screen_y();
int screen_x();
int screen_isy(int y);
int screen_isx(int x);

#endif // _TEXT_GRAPHIC_H_
