#include <game_engine/text_graphic.h>
#include <game_engine/lib/game_engine_lib.h>
#include "objtext.h"

static struct object *g_this;

static void objtext_draw(struct object* obj)
{
    struct objtext *objtext = obj->private;

    textoutc(g_this, objtext->x, objtext->y, objtext->color, 0, objtext->text);
}

static int objtext_key_press(struct object *obj, int ch)
{
    struct objtext *objtext = obj->private;
    int i;

    for(i=0; objtext->keylist[i]; i++)
    {
        if(ch == objtext->keylist[i])
            break;
    }

    switch(i)
    {
        case 0:
            objtext->x--;
            break;
        case 1:
            objtext->x++;
            break;
        case 2:
            objtext->y--;
            break;
        case 3:
            objtext->y++;
            break;
    }

    return 0;
}

static void objtext_control(struct object *obj)
{

}

static void objtext_destroy(struct object *obj)
{
    lvmv_free(&g_mem, obj->private);
    lvmv_free(&g_mem, obj);
}

struct object* create_objtext(char *text, int x, int y, int color, char *keylist)
{
    struct object *obj = (struct object*)lvmv_malloc(&g_mem, sizeof(struct object));
    struct objtext *objtext = (struct objtext*)lvmv_malloc(&g_mem, sizeof(struct objtext));

    obj->draw = objtext_draw;
    obj->key_press = objtext_key_press;
    obj->ctr = objtext_control;
    obj->dst = objtext_destroy;
    obj->type = OBJ_TEXT;

    sprintf(objtext->text, "%s", text);
    objtext->x = x;
    objtext->y = y;
    objtext->color = color;
    objtext->keylist = keylist;
    obj->private = objtext;

    g_this = obj;

    return obj;
}
