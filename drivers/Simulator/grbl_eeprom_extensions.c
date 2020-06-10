/*
  grbl_eeprom_extensions.c - 
  Grbl adds 2 functions to the orignal avr eeprom library. 
  They need to be reproduced here because we need to completely override the
  original eeprom interface for simulation

  Grbl is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Grbl is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Grbl.  If not, see <http://www.gnu.org/licenses/>.
*/

// Extensions added as part of Grbl 
// KEEP IN SYNC WITH ../eeprom.c

#include "eeprom.h"

#include "grbl/grbl.h"

void memcpy_to_eeprom_with_checksum(uint32_t destination, uint8_t *source, uint32_t size)
{
    uint32_t dest = destination;
    uint8_t checksum = calc_checksum(source, size);

    for(; size > 0; size--)
        eeprom_put_char(dest++, *(source++));

hal.stream.write("WR:");
hal.stream.write(uitoa(dest));
hal.stream.write(":");
hal.stream.write(uitoa(checksum));
hal.stream.write(":");
hal.stream.write(uitoa(*source));

    eeprom_put_char(dest, checksum);
}

bool memcpy_from_eeprom_with_checksum(uint8_t *destination, uint32_t source, uint32_t size)
{
uint8_t *dest = destination; uint32_t sz = size;
//hal.stream.write("EP:");
//hal.stream.write(uitoa(source));

    for(; size > 0; size--)
        *(destination++) = eeprom_get_char(source++);

/*
hal.stream.write(":");
hal.stream.write(uitoa(checksum));
hal.stream.write(":");
hal.stream.write(uitoa(eeprom_get_char(source)));
*/
    return calc_checksum(dest, sz) == eeprom_get_char(source);
}

// end of file
