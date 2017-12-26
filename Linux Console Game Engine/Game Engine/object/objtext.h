#ifndef _OBJTEXT_H_
#define _OBJTEXT_H_

#define OBJ_TEXT    1

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
