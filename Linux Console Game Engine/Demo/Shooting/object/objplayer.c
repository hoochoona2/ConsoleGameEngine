#include "objplayer.h"
#include <object/objball.h>

/**
* draw(자신의 객체)
* 드로잉 함수
**/
static void draw(struct object* obj)
{
    struct objplayer *player = (struct objplayer*)obj->private;

    textoutc(obj, player->x-1, player->y, C_RED, 0, "-0-");
}

/**
* key_press(자신의 객체, 입력된 키값)
* z를 누르면 총알이 생성됨
**/
static int key_press(struct object *obj, int ch)
{
    struct objplayer *player = (struct objplayer*)obj->private;

    switch(ch)
    {
        case 'j':
            player->x--;
            break;

        case 'l':
            player->x++;
            break;

        case 'i':
            player->y--;
            break;

        case 'k':
            player->y++;
            break;

        //공격키, 
        case 'z':
            {
                tick_t tick = get_tick_count();

                if(player->cooltime <= tick - player->oldtick)
                {
                    ///////////////
                    //총알 생성 및 등록
                    struct object *ball[3] = 
                    {
                        create_ball(player->x-2, player->y-1, player->ball_speed),
                        create_ball(player->x, player->y-1, player->ball_speed),
                        create_ball(player->x+2, player->y-1, player->ball_speed),
                    };

                    int i;
                    for(i=0; i<3; i++)
                        object_add(ball[i]);
                    ////////////

                    player->oldtick = tick;
                }

                break;
            }
    }

    return 0;
}

static void control(struct object *obj)
{

}

static void conflict(struct object *obj, struct object *target)
{

}

static void destroy(struct object *obj)
{
    lvmv_free(&g_mem, obj->private);
    lvmv_free(&g_mem, obj);
}

struct object* create_player(int x, int y)
{
    struct object *obj = (struct object*)lvmv_malloc(&g_mem, sizeof(struct object));
    struct objplayer *player = (struct objplayer*)lvmv_malloc(&g_mem, sizeof(struct objplayer));

    ///////////
    //클래스 함수 매핑
    obj->draw = draw;
    obj->key_press = key_press;
    obj->ctr = control;
    obj->dst = destroy;
    obj->conf = conflict;
    obj->type = OBJ_PLAYER;
    ////////////

    //////////
    //개인 데이터
    player->cooltime = 50; //쿨타임 50ms
    player->ball_speed = 30; //총알 스피드 30ms
    player->x = x;
    player->y = y;
    player->oldtick = get_tick_count();
    obj->private = player;
    ///////////

    return obj;
}
