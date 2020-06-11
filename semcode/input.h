/*******************************************************************
  Simple program to check LCD functionality on MicroZed
  based MZ_APO board designed by Petr Porazil at PiKRON

  input.h      - changing stdin mode to RAW, implementation used from 
  https://github.com/capiman/lpc21isp/blob/master/lpc21isp.c#L516, as
  advised by Dr. Pavel Pisa on APO CourseWare forum

  (C) Copyright 2020 by Jiri Povolny and Marian Krotil
      e-mail:   povolji2@fel.cvut.cz, krotima1@fel.cvut.cz
      license:  any combination of GPL, LGPL, MPL or BSD licenses

 *******************************************************************/

#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <stdio.h>

void PrepareKeyboardTtySettings();

int kbhit();

char getch();
