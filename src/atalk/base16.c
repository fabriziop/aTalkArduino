/* .+

.context    : Data coding
.title      : Base 16 coder/decoder
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

#include "base16.h"
#include <stdio.h>

/**
 * Encode a binary data buffer into a base 16 ASCII string, NULL terminated.
 * in: binary data buffer to be encoded.
 * inlen: in buffer len in bytes.
 * out: base 16 string buffer where to output the base 16 encoded string.
 * Returns the len of the base 16 encoded string (excluding terminator).
 */

int b16encode(unsigned char *in, int inlen, char *out)
{
  /* map 4 bits binary to base 16 chars */
  static const char *enmap16 = "0123456789abcdef";
  
  int outbase = (uintptr_t)out;

  while (inlen--) {
      *out++ = enmap16[*in & 0xf];
      *out++ = enmap16[*in++ >> 4 & 0xf];
  }
  *out = '\0';

  return (uintptr_t)out - outbase;
}


/**
 * Decode a base 16 ASCII string into the corresponding binary data.
 * in: the base 16 string (NULL terminated).
 * out: the binary buffer where to write the decoded data.
 * outlen: the length of the decoded binary data.
 * Returns the following error codes.
 * SUCCESS: no errors.
 * B16D_ERR_LEN_NOT_X2: error, base 16 string len not a 2 multiple.
 * B16D_ERR_POS0_NO_B16_CHAR: error, pos 0 not a base 16 char.
 * B16D_ERR_POS1_NO_B16_CHAR: error, pos 1 not a base 16 char.
 */

int b16decode(char *in, unsigned char *out, int *outlen)
{
  /* maps base 16 chars to bin (0-15), unallowed chars to '~' */
  static const char *demap16 = "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
    "~~~~~~~~~~~~~~~~\0\1\2\3\4\5\6\7\10\11~~~~~~"
    "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
    "~\12\13\14\15\16\17~~~~~~~~~~~~~~~~~~~~~~~~~";

  int inlen;
  unsigned char c;

  int inbase = (uintptr_t)in;
  *outlen = (uintptr_t)out;
  inlen = 0;

  while (*in != '\0') {

      c = demap16[(int)*in++];

      /* check error: input word position 0 not a valid base 16 char. */
      if (c > 0x0f) {
          *outlen = (uintptr_t)out - *outlen;
          return B16D_ERR_POS0_NO_B16_CHAR;
      }

      *out = c;
      c = demap16[(int)*in++];

      /* check error: input word position 1 not a valid base 16 char. */
      if (c > 0x0f) {
          *outlen = (uintptr_t)out - *outlen;
          return B16D_ERR_POS1_NO_B16_CHAR;
      }

      *out++ |= c << 4;
  }

  *outlen = (uintptr_t)out - *outlen;

  /* error check: length not a multiple of 2 */
  if ((uintptr_t)in - inbase & 0x1) return B16D_ERR_LEN_NOT_X2;
  
  return SUCCESS;
}

/**** END ****/
