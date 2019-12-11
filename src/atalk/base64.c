/* .+

.context    : Data coding
.title      : Base 64 coder/decoder
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


#include "base64.h"

/**
 * Encode a binary data buffer into a base 64 ASCII string, NULL terminated.
 * in: binary data buffer to be encoded.
 * inlen: in buffer len in bytes.
 * out: base 64 string buffer where to output the base 64 encoded string.
 * Returns the len of the base 64 encoded string (excluding terminator).
 */

int b64encode(unsigned char *in, int inlen, char *out)
{
  /* map 6 bits binary to base 64 chars */
  static const char *enmap64 = 
      "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

  unsigned char v;
  int outlen;

  outlen = (uintptr_t)out;

  while (inlen--) {
      *out++ = enmap64[*in >> 2];
      v = *in++ << 4 & 0x30;
      if (inlen--)
          *out++ = enmap64[v | *in >> 4];
      else {
          *out++ = enmap64[v];
          *out++ = '=';
          *out++ = '=';
	  *out = '\0';
          return (uintptr_t)out - outlen;
      }
      v = *in++ << 2 & 0x3c;
      if (inlen--) {
          *out++ = enmap64[v | *in >> 6];
          *out++ = enmap64[*in++ & 0x3f];
      }
      else {
          *out++ = enmap64[v];
          *out++ = '=';
	  *out = '\0';
          return (uintptr_t)out - outlen;
      }
  }
  *out = '\0';
  return (uintptr_t)out - outlen;
}


/**
 * Decode a base 64 ASCII string into the corresponding binary data.
 * in: the base 64 string (NULL terminated).
 * out: the binary buffer where to write the decoded data.
 * outlen: the length of the decoded binary data.
 * Returns the following error codes.
 * SUCCESS: no errors.
 * B64D_ERR_LEN_NOT_X4: error, base 64 string len not a 4 multiple.
 * B64D_ERR_POS0_NO_B64_CHAR: error, quatrain pos 0 not a base 64 char.
 * B64D_ERR_POS1_NO_B64_CHAR: error, quatrain pos 1 not a base 64 char.
 * B64D_ERR_POS2_ISEQ_POS3_NOEQ: error, quatrain pos 2 is = and pos 3 is not =. 
 * B64D_ERR_POS2_NO_B64_CHAR: error, quatrain pos 2 not a base 64 char.
 * B64D_ERR_POS3_NO_B64_CHA: error, quatrain pos 3 not a base 64 char.
 */

int b64decode(char *in, unsigned char *out, int *outlen)
{
  /* maps base 64 chars to bin (0-63), unallowed chars to '~' and '=' to '|' */
  static const char *demap64 = "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
    "\76~~~\77\64\65\66\67\70\71\72\73\74\75~~~|~~~\0\1\2\3\4\5\6\7\10\11\12"
    "\13\14\15\16\17\20\21\22\23\24\25\26\27\30\31~~~~~~\32\33\34\35\36\37"
    "\40\41\42\43\44\45\46\47\50\51\52\53\54\55\56\57\60\61\62\63";

  int inlen;
  unsigned char c0, c1, c2, c3;

  *outlen = (uintptr_t)out;
  inlen = 0;

  //while (*in != '\0' && *in != '}') {
  while (*in != '\0') {

      c0 = demap64[(int)*in++];

      /* check error: input word position 0 not a valid base 64 char. */
      if (c0 > 63) {
          *outlen = (uintptr_t)out - *outlen;
          return B64D_ERR_POS0_NO_B64_CHAR;
      }
      inlen++;

      c1 = demap64[(int)*in++];

      /* check error: input word position 1 not a valid base 64 char. */
      if (c1 > 63) {
          *outlen = (uintptr_t)out - *outlen;
          return B64D_ERR_POS1_NO_B64_CHAR;
      }
      inlen++;

      *out++ = c0 << 2 | (c1 >> 4 & 0x03);

      /* if word ends with '==' ok end, if there is only the first '=',
      end with error: third byte is '=', but fourth byte is not '='. */
      if (*in == '=') {
          inlen++;
          if (in[1] == '=') {
              inlen++;
              break;
          }
          else {
              *outlen = (uintptr_t)out - *outlen;
              return B64D_ERR_POS2_ISEQ_POS3_NOEQ;
          }
      }

      c2 = demap64[(int)*in++];

      /* check error: input word position 2 not a valid base 64 char. */
      if (c2 > 63) {
          *outlen = (uintptr_t)out - *outlen;
          return B64D_ERR_POS2_NO_B64_CHAR;
      }
      inlen++;

      *out++ = c1 << 4 | c2 >> 2;

      c3 = demap64[(int)*in++];

      /* if word ends with '=', terminate here, only 2 byte to convert */
      if (c3 == '|') {
          inlen++;
          break;
      }

      /* check error: input word position 3 not a valid base 64 char. */
      if (c3 > 63) {
          *outlen = (uintptr_t)out - *outlen;
          return B64D_ERR_POS3_NO_B64_CHAR;
      }
      inlen++;

      *out++ = c2 << 6 | (c3 & 0x3f);
  }

  *outlen = (uintptr_t)out - *outlen;

  /* error check: length not a multiple of 4 */
  if (inlen & 0x3) return B64D_ERR_LEN_NOT_X4;
  
  return SUCCESS;
}

/**** END ****/
