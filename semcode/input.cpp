/*******************************************************************
  Simple program to implement video game Snake on MicroZed based 
  MZ_APO board designed by Petr Porazil at PiKRON

  input.h      - changing stdin mode to RAW, implementation used from 
  https://github.com/capiman/lpc21isp/blob/master/lpc21isp.c#L516, as
  advised by Dr. Pavel Pisa on APO CourseWare forum

  (C) Copyright 2020 by Jiri Povolny and Marian Krotil
      e-mail:   povolji2@fel.cvut.cz, krotima1@fel.cvut.cz
      license:  any combination of GPL, LGPL, MPL or BSD licenses

 *******************************************************************/
 
#define _POSIX_C_SOURCE 200112L

#include "input.h"

struct termios keyboard_origtty;

void PrepareKeyboardTtySettings()
{
	/* store the current tty settings */
	if (!tcgetattr(0, &keyboard_origtty))
	{
		struct termios tty;
		/* start with the current settings */
		tty = keyboard_origtty;
		/* make modifications to put it in raw mode, turn off echo */
		tty.c_lflag &= ~ICANON;
		tty.c_lflag &= ~ECHO;
		tty.c_lflag &= ~ISIG;
		tty.c_cc[VMIN] = 1;
		tty.c_cc[VTIME] = 0;

		/* put the settings into effect */
		tcsetattr(0, TCSADRAIN, &tty);
	}
}

 
int kbhit()
{
    /* return 0 for no key pressed, 1 for key pressed */
	int return_value = 0;

    /* time struct for the select() function, to only wait a little while */
    struct timeval select_time;
    /* file descriptor variable for the select() call */
    fd_set readset;

    /* we're only interested in STDIN */
    FD_ZERO(&readset);
    FD_SET(STDIN_FILENO, &readset);

    /* how long to block for - this must be > 0.0, but could be changed
    to some other setting. 10-18msec seems to work well and only
    minimally load the system (0% CPU loading) */
    select_time.tv_sec = 0;
    select_time.tv_usec = 10;

    /* is there a keystroke there? */
    if (select(1, &readset, NULL, NULL, &select_time))
    {
        /* yes, remember it */
        return_value = 1;
    }


    /* return with what we found out */
    return return_value;
}

char getch()
{
	char ch;

    /* Read in one character */
    if(!read(0,&ch,1)){
        fprintf(stderr,"ERROR: cannot read a char!\n");
    }

    return ch;
}
