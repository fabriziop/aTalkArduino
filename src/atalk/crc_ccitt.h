/* .+

.context    : aTalk, real time data communication
.title      : cyclic redundancy check 16 bits by CCITT
.kind       : c include
.author     : Fabrizio Pollastri <mxgbot@gmail.com>
.site       : Revello - Italy
.creation   : 25-Jul-2019
.copyright  : see http://automationwiki.com/index.php?title=CRC-16-CCITT
              (c) 2019 Fabrizio Pollastri
.credits

  taken from http://automationwiki.com/index.php?title=CRC-16-CCITT

.description

This function computes the 16 bits cyclic redundancy check according to the
CCITT standard.

.- */

#ifndef CRC_CCITT_H
#define CRC_CCITT_H

#include <stdint.h>
#include <stdlib.h>

unsigned short crc_ccitt(uint8_t *data, uint8_t length,
		uint16_t seed, uint16_t final);


#endif	/* CRC_CCITT_H */

/**** END ****/
