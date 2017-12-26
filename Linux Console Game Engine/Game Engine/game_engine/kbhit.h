#ifndef _KBHIT_H_
#define _KBHIT_H_

#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h> 

void init_keyboard();
void close_keyboard();
int kbhit();
int readch();

#endif // _KBHIT_H_
