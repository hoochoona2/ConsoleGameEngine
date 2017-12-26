#ifndef _OBJPLAYER_H_
#define _OBJPLAYER_H_

#include <game_engine/text_graphic.h>
#include <game_engine/lib/game_engine_lib.h>

#define OBJ_PLAYER  2

struct objplayer
{
    char *key;
    int cooltime;
    int ball_speed;
    int x;
    int y;
    tick_t oldtick;
};

struct object* create_player(int x, int y);

#endif
