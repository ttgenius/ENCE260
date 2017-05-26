/** @file   mgetkey.c
    @author M. P. Hayes, UCECE
    @date   17 July 2011
    @brief  Non-blocking raw keyboard reading.
*/

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <errno.h>

#include "mgetkey.h"

static struct termios tty_attr_save;
static int tty_init = 0;


/* Get the terminal attributes.  */
static int mgetkey_attr_get (struct termios *ptty_attr)
{
    if (tcgetattr (STDIN_FILENO, ptty_attr) == -1)
    {
	perror ("mgetkey: Cannot get tty attributes");
	return 0;
    }
    return 1;
}


/* Set terminal attributes ensuring that the original attributes are
   saved.  */
static int mgetkey_attr_set (struct termios *ptty_attr)
{
    if (tcsetattr (STDIN_FILENO, TCSANOW, ptty_attr) == -1)
    {
	perror ("mgetkey: Cannot set tty attributes");
	return 0;
    }
    return 1;
}


int mgetkey_init (void)
{
    struct termios tty_attr;
    
    if (!tty_init)
    {
	if (!mgetkey_attr_get (&tty_attr_save))
	    return 0;
	
	/* Restore terminal attributes on exit.  */
	atexit (mgetkey_reset);
	tty_init = 1;
    }
    
    /* Get terminal attributes.  */
    if (!mgetkey_attr_get (&tty_attr))
	return 0;
    
    tty_attr.c_lflag &= ~ECHO; /* No echo.  */
    tty_attr.c_lflag &= ~ICANON; /* No canonical processing.  */
    tty_attr.c_cc[VMIN] = 0; /* Don't block.  */
    tty_attr.c_cc[VTIME] = 0; /* Don't block.  */
    
    /* Set terminal attributes.  */
    return mgetkey_attr_set (&tty_attr);
}


/* Restore the terminal attributes.  */
void mgetkey_reset (void)
{
    mgetkey_attr_set (&tty_attr_save);
}


/* Read a single key stroke from the keyboard.  This blocks until
   a key is typed.  Note that the key is not echoed.  */
int mgetkey (void)
{
    int key;

    if (!tty_init)
	mgetkey_init ();

    key = getchar ();
    if (key == -1)
	key = 0;
    return key;
}
