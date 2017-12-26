#include "objgamemaster.h"

static void draw(struct object* obj)
{

}

static int key_press(struct object *obj, int ch)
{
    switch(ch)
    {
        case 'q':
            return 1;
    }

    return 0;
}

static void control(struct object *obj)
{

}

static void destroy(struct object *obj)
{
    lvmv_free(&g_mem, obj);
}

struct object* create_gamemaster()
{
    struct object *obj = (struct object*)lvmv_malloc(&g_mem, sizeof(struct object));

    obj->draw = draw;
    obj->key_press = key_press;
    obj->ctr = control;
    obj->dst = destroy;
    obj->type = OBJ_GM;
    obj->private = NULL;

    return obj;
}
