#ifndef _GE_LIB_
#define _GE_LIB_

typedef unsigned long tick_t;
tick_t get_tick_count();

typedef (key_command_f)(void);

struct key_command
{
    char ch;
    key_command_f* f;
};

#endif
