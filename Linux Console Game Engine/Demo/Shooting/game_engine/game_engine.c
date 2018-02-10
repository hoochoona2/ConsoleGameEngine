#include "text_graphic.h"

int main(int argc, char** argv)
{
    game_engine_init();//게임엔진 프로그램 초기화

    while(1)
    {
        int ch = 0;
        int i;

        if(kbhit())//키가 눌리면
        {
            ch = getchar();
            if(_key_press(ch))//객체들의 key_press()를 호출함
                break;
        }

        _control();//객체들의 control()함수를 호출함
        _draw();//객체들의 draw()함수를 호출함
        _conflict();//객체들의 충돌함수를 호출함
        _delobj();//객체들의 소멸자를 호출함
        invalidate();//실제화면에 텍스트를 쓰고 가상화면을 리셋함
    }

    game_destroy();//게임엔진 프로그램을 종료하는 준비, 남아있는 클래스의 소멸자를 호출함

    return 0;
}
