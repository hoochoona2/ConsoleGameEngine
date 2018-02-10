#ifndef _OBJBALL_H_
#define _OBJBALL_H_

#include <game_engine/text_graphic.h>
#include <game_engine/lib/game_engine_lib.h>

#define OBJ_BALL 3 //클래스ID

struct objball
{
    int x;
    int y;
    int speed;
    int color;
    tick_t oldtick;
};

struct object* create_ball(int x, int y, int speed);

#endif
