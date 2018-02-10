#ifndef _OBJGM_H_
#define _OBJGM_H_

#include <game_engine/text_graphic.h>
#include <game_engine/lib/game_engine_lib.h>

#define OBJ_GM    0
/**
* gamemaster클래스는, 형태가 없이 이벤트만 받음.
**/
struct object;

struct object* create_gamemaster();

#endif
