#include <time.h>

typedef unsigned long tick_t;

//윈도우즈API의 GetTickCount()와 유사한 함수
tick_t get_tick_count()
{
    tick_t tick = 0ul;

    struct timespec tp;

    clock_gettime(CLOCK_MONOTONIC, &tp);

    tick = (tp.tv_sec*1000ul) + (tp.tv_nsec/1000ul/1000ul);

    return tick;
}
