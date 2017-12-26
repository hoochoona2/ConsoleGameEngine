#include <game_engine/text_graphic.h>
#include <game_engine/lib/game_engine_lib.h>
#include <object/objplayer.h>
#include <object/objblock.h>

void game_init()
{
    int x, y;
    struct object *player = create_player(screen_x()/2, screen_y()-2);
    object_add(player);

    for(x=0; x<screen_x(); x++)
    {
        struct object *block = create_block(x, 0, 1, 1, 700);
        object_add(block);

        block = create_block(x, screen_y()-1, 1, 1, 700);
        object_add(block);
    }

    for(y=1; y<screen_y()-1; y++)
    {
        struct object *block = create_block(0, y, 1, 1, 700);
        object_add(block);

        block = create_block(screen_x()-1, y, 1, 1, 700);
        object_add(block);
    }
}

int game_keypress(int ch)
{
    if('q' == ch)
        return 1;

    return 0;
}

void game_main()
{

}
