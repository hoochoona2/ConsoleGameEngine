#include <game_engine/text_graphic.h>
#include <game_engine/lib/game_engine_lib.h>
#include <object/objtext.h>
#include <object/objgamemaster.h>

void game_init()
{
    struct object *GM = create_gamemaster();
    object_add(GM);

    struct object *obj1 = create_objtext("play1", 10, 10, C_GREEN, "adws");
    object_add(obj1);

    struct object *obj2 = create_objtext("play2", 10, 12, C_RED, "jlik");
    object_add(obj2);
}

void game_main()
{

}
