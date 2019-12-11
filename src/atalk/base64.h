/* .+

.context    : Data coding
.title      : Base 64 coder/decoder include file
.kind       : c source
.author     : Fabrizio Pollastri <mxgbot@gmail.com>
.site       : Revello - Italy
.creation   : 23-May-2017
.copyright  : (c) 2017 Fabrizio Pollastri
.license    : GNU Lesser General Public License version 3

.credits

Software derived from Base64 (RFC1341) 2005, by Jouni Malinen <j@w1.fi>

.description

Two functions are provided for encoding and decoding to/from base 64.
WARNING: no control is done about output buffer overflow.


.- */

#ifndef BASE64_H
#define BASE64_H

#include <stdint.h>
#include <stdlib.h>

/* decode error codes */
#define SUCCESS 0
#define B64D_ERR_LEN_NOT_X4 -2
#define B64D_ERR_POS0_NO_B64_CHAR -3
#define B64D_ERR_POS1_NO_B64_CHAR -4
#define B64D_ERR_POS2_ISEQ_POS3_NOEQ -5
#define B64D_ERR_POS2_NO_B64_CHAR -6
#define B64D_ERR_POS3_NO_B64_CHAR -7

  
int b64encode(unsigned char *in, int inlen, char *out);

int b64decode(char *in, unsigned char *out, int *outlen);

#endif /* BASE64_H */

/**** END ****/
