#include "text_graphic.h"

int main(int argc, char** argv)
{
    game_engine_init();

    while(1)
    {
        int ch = 0;
        int i;

        if(kbhit())
        {
            ch = getchar();
            if(_key_press(ch))
                break;
        }

        invalidate();
        _control();
        _draw();
    }

    game_destroy();

    return 0;
}
