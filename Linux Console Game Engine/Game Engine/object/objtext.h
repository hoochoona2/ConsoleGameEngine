#ifndef _OBJTEXT_H_
#define _OBJTEXT_H_

#define OBJ_TEXT    1

/**
* struct objtext
* objtext의 클래스, struct object의 private에 할당된다.
**/
struct objtext
{
    char text[256];
    int x;
    int y;
    int color;
    char *keylist;
};

struct object* create_objtext(char *text, int x, int y, int color, char *keylist);

#endif
