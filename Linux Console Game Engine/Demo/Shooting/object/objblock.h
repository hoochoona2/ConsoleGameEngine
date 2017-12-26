#ifndef _OBJBLOCK_H_
#define _OBJBLOCK_H_

#include <game_engine/text_graphic.h>
#include <game_engine/lib/game_engine_lib.h>

#define OBJ_BLOCK 4

struct objblock
{
    int x;
    int y;
    int w;
    int h;
    int max_hp;
    int hp;
    tick_t oldtick;
};

struct object* create_block(int x, int y, int w, int h, int hp);

#endif
