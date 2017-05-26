/** @file   mgetkey.h
    @author M. P. Hayes, UCECE
    @date   17 July 2011
    @brief  Non-blocking raw keyboard reading.
*/

#ifndef MGETKEY_H
#define MGETKEY_H

/* Read a single key stroke from the keyboard.  If no key has been
   pressed then zero is returned.  Note that the key is not
   echoed.  */
int mgetkey (void);


/* Restore terminal settings.  */
void mgetkey_reset (void);

#endif
