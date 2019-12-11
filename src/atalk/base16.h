/* .+

.context    : Data coding
.title      : Base 16 coder/decoder include file
.kind       : c source
.author     : Fabrizio Pollastri <mxgbot@gmail.com>
.site       : Revello - Italy
.creation   : 31-Dec-2017
.copyright  : (c) 2017 Fabrizio Pollastri
.license    : GNU Lesser General Public License version 3

.description

Two functions are provided for encoding and decoding to/from base 16.
WARNING: no control is done about output buffer overflow.

.- */

#ifndef BASE16_H
#define BASE16_H

#include <stdint.h>
#include <stdlib.h>

/* decode error codes */
#define SUCCESS 0
#define B16D_ERR_LEN_NOT_X2 -2
#define B16D_ERR_POS0_NO_B16_CHAR -3
#define B16D_ERR_POS1_NO_B16_CHAR -4

  
int b16encode(unsigned char *in, int inlen, char *out);

int b16decode(char *in, unsigned char *out, int *outlen);

#endif /* BASE16_H */

/**** END ****/
