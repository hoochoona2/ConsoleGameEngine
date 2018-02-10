#include <game_engine/text_graphic.h>
#include <game_engine/lib/game_engine_lib.h>
#include "objtext.h"

/**
* objtext_draw(자신의 클래스 정보)
* 드로잉 함수
**/
static void objtext_draw(struct object* obj)
{
    struct objtext *objtext = obj->private;

    textoutc(objtext->x, objtext->y, objtext->color, 0, objtext->text);
}

/**
* objtext_key_press(자신의 클래스정보, 입력된 키값)
* 키가 눌러지면 호출되는 함수
**/
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

/**
* objtext_control(자신의 클래스정보)
* 주기적으로 호출되는 함수
**/
static void objtext_control(struct object *obj)
{

}

/**
* objtext_destroy(자신의 클래스정보)
* 소멸자 함수
**/
static void objtext_destroy(struct object *obj)
{
    lvmv_free(&g_mem, obj->private);
    lvmv_free(&g_mem, obj);
}

/**
* create_objtext(문장, X좌표, Y좌표, 색상, 조작키)
* 생성자 함수
**/
struct object* create_objtext(char *text, int x, int y, int color, char *keylist)
{
    struct object *obj = (struct object*)lvmv_malloc(&g_mem, sizeof(struct object));
    struct objtext *objtext = (struct objtext*)lvmv_malloc(&g_mem, sizeof(struct objtext));

    //클래스 함수 매핑 & 클래스ID 설정
    obj->draw = objtext_draw;
    obj->key_press = objtext_key_press;
    obj->ctr = objtext_control;
    obj->dst = objtext_destroy;
    obj->type = OBJ_TEXT;

    //클래스 개인 데이터 설정
    sprintf(objtext->text, "%s", text);
    objtext->x = x;
    objtext->y = y;
    objtext->color = color;
    objtext->keylist = keylist;
    obj->private = objtext;

    return obj;
}
