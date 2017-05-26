/** @file   eeprom.c
    @author M. P. Hayes, UCECE
    @date   15 May 2007
    @brief 
*/
#include "system.h"
#include <avr/io.h>
#include "eeprom.h"

static void
eeprom_write_byte (eeprom_addr_t addr, uint8_t data)
{
    /* Wait for completion of previous write.  */
    while (EECR & _BV (EEPE))
        continue;

    /* Set up address and data registers */
    EEAR = addr;
    EEDR = data;

    EECR |= _BV (EEMPE);

    /* Start eeprom write.  */
    EECR |= _BV (EEPE);
}


static uint8_t eeprom_read_byte (eeprom_addr_t addr)
{
    /* Wait for completion of previous write.  */
    while (EECR & _BV (EEPE))
        continue;

    /* Set up address register */
    EEAR = addr;
    
    /* Start eeprom read.  */
    EECR |= _BV (EERE);

    /* Return data from data register */
    return EEDR;
}


/* Read SIZE bytes from ADDR into BUFFER.  */
eeprom_size_t
eeprom_read (eeprom_addr_t addr, void *buffer, eeprom_size_t size)
{
    eeprom_size_t i;
    uint8_t *data = buffer;

    for (i = 0; i < size; i++)
        data[i] = eeprom_read_byte (addr++);

    return i;

}


/* Write SIZE bytes to ADDR from BUFFER.  */
eeprom_size_t 
eeprom_write (eeprom_addr_t addr, const void *buffer, eeprom_size_t size)
{
    eeprom_size_t i;
    const uint8_t *data = buffer;

    for (i = 0; i < size; i++)
        eeprom_write_byte (addr++, data[i]);

    return i;
}
