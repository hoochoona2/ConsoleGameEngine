#include <game_engine/text_graphic.h>
#include <game_engine/lib/game_engine_lib.h>
#include <object/objplayer.h>
#include <object/objblock.h>

/**
* game_init()
* 게임 초기화
**/
void game_init()
{
    int x, y;

    /////////
    //플레이어 생성 및 객체 등록
    struct object *player = create_player(screen_x()/2, screen_y()-2);
    object_add(player);
    /////////

    /////////
    //x측/y측 벽돌 생성 및 객체 등록
    for(x=0; x<screen_x(); x++)
    {
        //1x1크기, 체력700의 벽돌 생성 및 등록
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
    ///////////
}

/**
* game_keypress(입력된 키값)
* 키 이벤트를 처리하는 함수. 1을 반환하면 프로그램 종료. 
**/
int game_keypress(int ch)
{
    if('q' == ch)
        return 1;

    return 0;
}

/**
* game_main()
* 주기적으로 호출되는 함수 
**/
void game_main()
{

}
