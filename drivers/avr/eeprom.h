/** @file eeprom.h
    @author Michael Hayes
    @date 13 Nov 2006
    @brief Routines to read/write internal EEPROM.

    @defgroup eeprom EEPROM driver
       
    This module implements a driver to read/write the EEPROM of an
    ATmega microcontroller.  The EEPROM is useful for storing
    persistent data when the power is removed.
 */
 
#ifndef EEPROM_H
#define EEPROM_H

#include "system.h"

typedef uint16_t eeprom_addr_t;
typedef uint16_t eeprom_size_t;

/* Read SIZE bytes from ADDR into BUFFER.  */
extern eeprom_size_t
eeprom_read (eeprom_addr_t addr, void *buffer, eeprom_size_t size);


/* Write SIZE bytes to ADDR from BUFFER.  */
extern eeprom_size_t 
eeprom_write (eeprom_addr_t addr, const void *buffer, eeprom_size_t size);

#endif
