#include "text_graphic.h"
#include "data_struct/list.h"
#include <stdarg.h>
#include <pthread.h>

#define BLANK ' '
#define DUMMY0 'h'
#define DUMMY1 'c'
#define DUMMY2 'n'

extern void game_init();
extern void game_main();
extern int game_keypress();

struct monitor g_monitor; //가상모니터
struct object_list g_objdel; //삭제될 객체리스트

/**
* game_engine_init()
* 프로그램 초기화
**/
int game_engine_init()
{
    int y, x, fd;

    lvmv_memory_init(&g_mem);//메모리 관리 초기화

    fd = fileno(stdin);
    ioctl(fd, TIOCGWINSZ, &g_monitor.ws);//윈도우 사이즈를 얻음

    /////////
    //가상모니터 할당
    g_monitor.pixel = (struct pixel**)lvmv_malloc(&g_mem, sizeof(struct pixel*) * g_monitor.ws.ws_row);

    for(y=0; y<g_monitor.ws.ws_row; y++)
    {
        g_monitor.pixel[y] = (struct pixel*)lvmv_malloc(&g_mem, sizeof(struct pixel) * g_monitor.ws.ws_col + 1);
        for(x=0; x<g_monitor.ws.ws_col; x++)
        {
            list_init(&g_monitor.pixel[y][x].owners);
            g_monitor.pixel[y][x].owners_cnt = 0;
        }
    }
    ////////

    _clear(); // 가상모니터 리셋

    list_init(&g_monitor.obj); //동작중인 객체 리스트

    /////////
    //화면 & 키보드 초기화
    WINDOW* w;
    w = initscr();
    nodelay(w, TRUE);
    keypad(stdscr, TRUE);
    changemode(1);
    //////////

    //////////
    //색상 초기화
    has_colors();
    start_color();

    int color_index = 0;

#define xx(c1, c2)\
    init_pair(color_index++, COLOR_##c1, COLOR_##c2);
#include "text_graphic_color.h"
#undef xx
    ///////////

    list_init(&g_objdel);//삭제될 객체 리스트 초기화
    game_init();//게임 초기화

    return 0;
}

/**
* _clear()
* 가상화면 리셋
**/
void _clear()
{
    int y, x;

    for(y=0; y<g_monitor.ws.ws_row; y++)
    {
        for(x=0; x<g_monitor.ws.ws_col; x++)
        {
            struct object_list *v, *t;
            //공백(‘ ‘)으로 채움
            g_monitor.pixel[y][x].ch_pixel.ch = BLANK;
            g_monitor.pixel[y][x].ch_pixel.dummy[0] = DUMMY0;
            g_monitor.pixel[y][x].ch_pixel.dummy[1] = DUMMY1;
            g_monitor.pixel[y][x].ch_pixel.dummy[2] = DUMMY2;
            g_monitor.pixel[y][x].ch_pixel.attr = COLOR_PAIR(C_WHITE);

            list_iterate_safe(v, t, &g_monitor.pixel[y][x].owners)
            {
                list_del(v);
                lvmv_free(&g_mem, v);
            }

            g_monitor.pixel[y][x].owners_cnt = 0;
        }
        //NUL문자
        g_monitor.pixel[y][x].ch_pixel.ch = '\0';
        g_monitor.pixel[y][x].ch_pixel.dummy[0] = DUMMY0;
        g_monitor.pixel[y][x].ch_pixel.dummy[1] = DUMMY1;
        g_monitor.pixel[y][x].ch_pixel.dummy[2] = DUMMY2;
        g_monitor.pixel[y][x].ch_pixel.attr = COLOR_PAIR(C_WHITE);
    }

    return;
}

/**
* invalidate()
* 가상화면에 그려진 텍스트를 실제 화면에 쓰고 가상화면을 리셋함
**/
void invalidate()
{
    int y, x;

    /////////
    //실제화면에 쓰는 작업
    for(y=0; y<g_monitor.ws.ws_row; y++)
    {
        for(x=0; x<g_monitor.ws.ws_col; x++)
        {
            attrset(g_monitor.pixel[y][x].ch_pixel.attr);
            mvprintw(y, x, "%c", g_monitor.pixel[y][x].ch_pixel.ch);
        }
    }

    refresh();
    /////////

    _clear(); //가상화면 리셋

    return;
}

/**
* textoutc(소유자, x좌표, y좌표, 색상, 속성, 문자열)
* 지정한 좌표에 문자열을 씀
**/
int textoutc(struct object *owner, int x, int y, int color, int attr, char* format, ...)
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
        g_monitor.pixel[y][x].ch_pixel.ch = *pch;

        if(owner)
        {
            struct object_list *v = (struct object_list*)lvmv_malloc(&g_mem, sizeof(struct object_list));
            v->obj = owner;
            list_add(&g_monitor.pixel[y][x].owners, v);
            g_monitor.pixel[y][x].owners_cnt++;
        }
        x++;
        ret++;
    }

    return ret;
}

void rectanglec(struct object *owner, int sx, int sy, int dx, int dy, int color)
{
    int i;

    textoutc(owner, sx, sy, color, 0, "+");
    textoutc(owner, dx, sy, color, 0, "+");
    textoutc(owner, sx, dy, color, 0, "+");
    textoutc(owner, dx, dy, color, 0, "+");

    for(i=sx+1; i<dx; i++)
    {
        textoutc(owner, i, sy, color, 0, "-");
        textoutc(owner, i, dy, color, 0, "-");
    }

    for(i=sy+1; i<dy; i++)
    {
        textoutc(owner, sx, i, color, 0, "!");
        textoutc(owner, dx, i, color, 0, "!");
    }

    return;
}

void fillrectc(struct object *owner, int sx, int sy, int dx, int dy, int color)
{
    int i, j;

    if(color < 9)
        color *= 9;

    for(i=sy; i<=dy; i++)
    {
        for(j=sx; j<=dx; j++)
        {
            textoutc(owner, j, i, color, 0, " ");
        }
    }

    return;
}

/**
* object_add(객체)
* 객체를 등록함. 등록된 객체는 이벤트를 받을 수 있음
**/
void object_add(struct object *obj)
{
    list_add(&g_monitor.obj, obj);

    return;
}

/**
* object_del(객체)
* 삭제할 객체를 리스트에 담는다.
**/
void object_del(struct object *obj)
{
    struct object_list *del = (struct object_list*)lvmv_malloc(&g_mem, sizeof(struct object_list));

    del->obj = obj;
    list_add(&g_objdel, del);
}

/**
* _draw()
* 객체들의 draw()함수를 호출함
**/
void _draw()
{
    struct object *v, *t;

    list_iterate_safe(v, t, &g_monitor.obj)
    {
        v->draw(v);
    }
}

/**
* _control()
* 주기적으로 조작 함수를 호출함.
**/
void _control()
{
    struct object *v ,*t;

    game_main();

    list_iterate_safe(v, t, &g_monitor.obj)
    {
        v->ctr(v);
    }
}

/**
* _delobj()
* 리스트에 담아뒀던 삭제될 객체를 소멸자를 호출해 삭제함.
**/
void _delobj()
{
    struct object_list *v, *t;

    list_iterate_safe(v, t, &g_objdel)
    {
        struct object *obj = v->obj;

        list_del(obj);
        obj->dst(obj);
        list_del(v);
        lvmv_free(&g_mem, v);
    }

}

/**
* _key_press(입력된 키값)
* key_press()함수 내에서 0외의 값이 반환되면 프로그램이 종료하게 됨.
**/
int _key_press(int ch)
{
    struct object *v, *t;

    if(game_keypress(ch))
        return 1;

    list_iterate_safe(v, t, &g_monitor.obj)
    {
        if(v->key_press(v, ch))
            return 1;
    }

    return 0;
}

/**
* _conflict()
* 가상화면의 각 픽셀의 소유자를 확인하고 2이상인 픽셀에 대해 충돌을 대상 객체의 충돌함수를 호출함.
**/
void _conflict()
{
    int x, y;

    for(y=0; y<g_monitor.ws.ws_row; y++)
    {
        for(x=0; x<g_monitor.ws.ws_col; x++)
        {
            if(g_monitor.pixel[y][x].owners_cnt < 2)
                continue;

            struct object_list *v1, *v2;

            list_iterate(v1, &g_monitor.pixel[y][x].owners)
            {
                list_iterate(v2, &g_monitor.pixel[y][x].owners)
                {
                    if(v1->obj == v2->obj)
                        continue;

                    v1->obj->conf(v1->obj, v2->obj);
                }
            }
        }
    }
}

/**
* game_destroy()
* 게임엔진, 가상화면, 각 클래스를 해제함.
**/
void game_destroy()
{
    struct object *v, *t;
    int x, y;

    changemode(0);
    for(y=0; y<g_monitor.ws.ws_row; y++)
    {
        for(x=0; x<g_monitor.ws.ws_col; x++)
        {
            struct object_list *v, *t;

            list_iterate_safe(v, t, &g_monitor.pixel[y][x].owners)
            {
                list_del(v);
                lvmv_free(&g_mem, v);
            }
        }
        lvmv_free(&g_mem, g_monitor.pixel[y]);
    }

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
