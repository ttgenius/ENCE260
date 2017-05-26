/** @file   eeprom.c
    @author M. P. Hayes, UCECE
    @date   15 May 2007
    @brief 
*/
#include "system.h"
#include "eeprom.h"
#include <stdio.h>
#include <string.h>


#define EEPROM_SIZE 512
#define EEPROM_FILENAME "ucfk_eeprom.dat"


static void
eeprom_file_write (void *buffer, eeprom_size_t size)
{
    FILE *fp;

    fp = fopen (EEPROM_FILENAME, "w");
    if (fp)
    {
        fwrite (buffer, size, 1, fp);
        fclose (fp);
    }
}


static void
eeprom_file_read (void *buffer, eeprom_size_t size)
{
    FILE *fp;

    fp = fopen (EEPROM_FILENAME, "r");
    if (!fp)
    {
        /* If file not present assume EEPROM is erased and thus
           contains the values 0xff.  */
        memset (buffer, 0xff, size);
        eeprom_file_write (buffer, size);
    }
    else
    {
        fread (buffer, size, 1, fp);
        fclose (fp);
    }
}


/* Read SIZE bytes from ADDR into BUFFER.  */
eeprom_size_t
eeprom_read (eeprom_addr_t addr, void *buffer, eeprom_size_t size)
{
    eeprom_size_t i;
    uint8_t *data = buffer;
    uint8_t eeprom[EEPROM_SIZE];

    eeprom_file_read (eeprom, EEPROM_SIZE);

    for (i = 0; i < size && addr < EEPROM_SIZE; i++)
        data[i] = eeprom[addr++];

    return i;

}


/* Write SIZE bytes to ADDR from BUFFER.  */
eeprom_size_t 
eeprom_write (eeprom_addr_t addr, const void *buffer, eeprom_size_t size)
{
    eeprom_size_t i;
    const uint8_t *data = buffer;
    uint8_t eeprom[EEPROM_SIZE];

    eeprom_file_read (eeprom, EEPROM_SIZE);

    for (i = 0; i < size && addr < EEPROM_SIZE; i++)
        eeprom[addr++] = data[i];

    eeprom_file_write (eeprom, EEPROM_SIZE);

    return i;
}
