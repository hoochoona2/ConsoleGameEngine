#ifndef _OBJPLAYER_H_
#define _OBJPLAYER_H_

#include <game_engine/text_graphic.h>
#include <game_engine/lib/game_engine_lib.h>

#define OBJ_PLAYER  2 //클래스ID

/**
* struct objplayer
* 개인데이터
**/
struct objplayer
{
    char *key;//조작키
    int cooltime;//공격 쿨타임
    int ball_speed;//총알 스피드
    int x;//좌
    int y;//표
    tick_t oldtick;//개인데이터
};

struct object* create_player(int x, int y);

#endif
