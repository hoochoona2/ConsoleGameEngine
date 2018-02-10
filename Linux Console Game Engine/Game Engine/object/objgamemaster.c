#include "objgamemaster.h"

/**
* draw(자신의 클래스정보)
* 드로잉함수
**/
static void draw(struct object* obj)
{

}

/**
* key_press(자신의 클래스정보, 눌러진 키값)
* 키를 누르면 호출되는 함수, 1을 반환하면 게임엔진이 프로그램을 종료함.
**/
static int key_press(struct object *obj, int ch)
{
    switch(ch)
    {
        case 'q':
            return 1;
    }

    return 0;
}

/**
* control(자신의 클래스정보)
* 주기적으로 호출되는 함수
**/
static void control(struct object *obj)
{

}

/**
* destory(자신의 클래스정보)
* 클래스가 해제될 때 호출됨
**/
static void destroy(struct object *obj)
{
    lvmv_free(&g_mem, obj);
}

/**
* create_gamemaster()
* 클래스 생성자
* 반환 : 클래스
**/
struct object* create_gamemaster()
{
    struct object *obj = (struct object*)lvmv_malloc(&g_mem, sizeof(struct object));

    //클래스 함수 매핑
    obj->draw = draw;
    obj->key_press = key_press;
    obj->ctr = control;
    obj->dst = destroy;
    //////

    obj->type = OBJ_GM; //클래스ID
    obj->private = NULL; //개인데이터 없음

    return obj;
}
