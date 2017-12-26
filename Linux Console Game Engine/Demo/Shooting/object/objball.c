#include "objball.h"
#include "objblock.h"

static void draw(struct object* obj)
{
    struct objball *ball = (struct objball*)obj->private;

    textoutc(obj, ball->x, ball->y, ball->color, 0, "*");
}

static int key_press(struct object *obj, int ch)
{

    return 0;
}

static void control(struct object *obj)
{
    struct objball *ball = (struct objball*)obj->private;
    tick_t tick = get_tick_count();

    if(ball->speed <= (tick - ball->oldtick))
    {
        ball->y--;
        if(ball->y < 0)
        {
            object_del(obj);

            return;
        }

        ball->oldtick = tick;
    }

}

static void conflict(struct object *obj, struct object *target)
{
    struct objball *ball = (struct objball*)obj->private;

    switch(target->type)
    case OBJ_BLOCK:
    object_del(obj);
}

static void destroy(struct object *obj)
{
    lvmv_free(&g_mem, obj->private);
    lvmv_free(&g_mem, obj);
}

struct object* create_ball(int x, int y, int speed)
{
    struct object *obj = (struct object*)lvmv_malloc(&g_mem, sizeof(struct object));
    struct objball *ball = (struct objball*)lvmv_malloc(&g_mem, sizeof(struct objball));

    obj->draw = draw;
    obj->key_press = key_press;
    obj->ctr = control;
    obj->dst = destroy;
    obj->conf = conflict; 
    obj->type = OBJ_BALL;
    ball->x = x;
    ball->y = y;
    ball->speed = speed;
    ball->color = C_WHITE;
    ball->oldtick = get_tick_count();
    obj->private = ball;

    return obj;
}
