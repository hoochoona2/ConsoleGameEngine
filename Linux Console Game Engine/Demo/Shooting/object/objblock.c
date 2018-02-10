#include "objblock.h"
#include "objball.h"

static void draw(struct object* obj)
{
    struct objblock *block = (struct objblock*)obj->private;
    
    int color[8] = //벽돌 체력을 나타내는 색상
    {
        0,
        C_BLACK_RED,
        C_BLACK_GREEN,
        C_BLACK_YELLOW,
        C_BLACK_BLUE,
        C_BLACK_MAGENTA,
        C_BLACK_CYAN,
        C_BLACK_WHITE
    };
    int x, y;
    double dummy = block->max_hp/7.0;

    for(y=0; y<block->h; y++)
    {
        for(x=0; x<block->w; x++)
        {
            textoutc(obj, block->x+x, block->y+y, color[(int)(block->hp/dummy)], 0, "-");
        }
    }
}

static int key_press(struct object *obj, int ch)
{

    return 0;
}

static void control(struct object *obj)
{
    struct objblock *block = (struct objblock*)obj->private;
}

/**
* conflict(자신의 객체, 대상의 객체)
* 총알이 벽돌(자신)이랑 겹쳤다면, 체력을 깍는다.
**/
static void conflict(struct object *obj, struct object *target)
{
    struct objblock *block = (struct objblock*)obj->private;
    struct objball *ball = (struct objball*)target->private;

    switch(target->type)
    {
        case OBJ_BALL:
            block->hp-=20;
            if(block->hp < 0)
                block->hp = 0;

            break;
    }
}

static void destroy(struct object *obj)
{
    lvmv_free(&g_mem, obj->private);
    lvmv_free(&g_mem, obj);
}

struct object* create_block(int x, int y, int w, int h, int hp)
{
    struct object *obj = (struct object*)lvmv_malloc(&g_mem, sizeof(struct object));
    struct objblock *block = (struct objblock*)lvmv_malloc(&g_mem, sizeof(struct objblock));

    obj->draw = draw;
    obj->key_press = key_press;
    obj->ctr = control;
    obj->dst = destroy;
    obj->conf = conflict;
    obj->type = OBJ_BLOCK;
    block->x = x;
    block->y = y;
    block->w = w;
    block->h = h;
    block->max_hp = block->hp = hp;
    block->oldtick = get_tick_count();
    obj->private = block;

    return obj;
}
