#include <game_engine/text_graphic.h>
#include <game_engine/lib/game_engine_lib.h>
#include <object/objtext.h>
#include <object/objgamemaster.h>

/**
* game_init()
* 
* 게임 초기화, 프로그램이 시작되고 바로 한번 호출 됨
**/
void game_init()
{
    struct object *GM = create_gamemaster(); //gamemaster클래스 생성
    object_add(GM);//gamemaster클래스 등록

    struct object *obj1 = create_objtext("play1", 10, 10, C_GREEN, "adws");//objtext클래스 생성
    object_add(obj1);//objtext클래스 등록

    struct object *obj2 = create_objtext("play2", 10, 12, C_RED, "jlik");
    object_add(obj2);
    
    //등록된 클래스에 이벤트가 발생하면 클래스 코드에 정의된 함수가 호출 됨 
}

/**
* game_main()
* 
* 게임 코드를 작성하는 함수, game_init()이 호출되고 프로그램 종료 전까지 주기적으로 호출 됨
**/
void game_main()
{

}
