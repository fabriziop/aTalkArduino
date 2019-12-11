/* .+

.context    : aTalk, real time data communication
.title      : core functions
.kind       : c source
.author     : Fabrizio Pollastri <mxgbot@gmail.com>
.site       : Revello - Italy
.creation   : 25-Jul-2019
.copyright  : (c) 2019 Fabrizio Pollastri
.license    : GNU Lesser General Public License version 3

.- */

#include <stdio.h>

#include "atalk.h"


/**** API functions ****/

struct aTalk *atalk_init(uint8_t data_size_max, enum aTalkEncoder encoder) { 

  struct aTalk *atalk;
  uint16_t msg_len_max;
  uint16_t msg_encoded_len_max;

  /* allocate control structure */
  atalk = (struct aTalk *)malloc(sizeof(struct aTalk));
  if (atalk == NULL)
    return NULL;

  /* save parameters */
  atalk->data_size_max = data_size_max;
  atalk->encoder = encoder;

  /* set the requested encoder and compute the max size of encoded
   * message (including start/end) according to selected encoder. */
  switch (encoder)
    {
    case BASE16:
      atalk->encode = b16encode;
      atalk->decode = b16decode;
      atalk->msg_encoded_len_max = (data_size_max << 1) + 15;
      break;
    case BASE64:
      atalk->encode = b64encode;
      atalk->decode = b64decode;
      atalk->msg_encoded_len_max = ((data_size_max+data_size_max%3)/3<<2)+11;
      break;
      /* unknown codec, return error */
    default:
      return NULL;
    }

    /* size of receive buffer for encoded msg with room for non msg chars */
    atalk->receive_buf_enc_len = atalk->msg_encoded_len_max;
    atalk->receive_buf_enc_len += ATALK_REC_MSG_ENC_GUARD;

    /** allocate message buffers **/

    /* binary send message assembling */
    atalk->send_msg_binary = (uint8_t *)malloc(data_size_max + 6);
    if (atalk->send_msg_binary == NULL)
      return NULL;

    /* encoded send message */
    atalk->send_msg_encoded = (char *)malloc(atalk->msg_encoded_len_max);
    if (atalk->send_msg_encoded == NULL)
      return NULL;

    /* encoded receive message */
    atalk->receive_msg_encoded = (char *)malloc(atalk->receive_buf_enc_len);
    if (atalk->receive_msg_encoded == NULL)
      return NULL;

    /* binary receive data message */
    atalk->receive_msg_binary = (uint8_t *)malloc(data_size_max + 6);
    if (atalk->receive_msg_binary == NULL)
      return NULL;

    /* clear sequence numbers */
    atalk->send_seq_num = 0;
    atalk->receive_seq_num = 0;

    /* clear counters */
    atalk->sent_chars = 0;
    atalk->sent_msgs = 0;
    atalk->sent_data = 0;
    atalk->send_errors = 0;
    atalk->received_chars = 0;
    atalk->received_msgs = 0;
    atalk->received_data = 0;
    atalk->receive_errors = 0;

    /* return control structure pointer */
    return atalk;
}


/* encode and send data on phisical link immediately */
int atalk_send (struct aTalk *atalk, uint8_t *data, uint8_t data_size) {

  uint8_t *bmsg;
  uint16_t crc;
  int msglen;
  int wrlen;

  /* check for exceeding maximum data size */
  if (data_size > atalk->data_size_max)
    return ATALK_SEND_DATA_SIZE_OVER_MAX;

  /** assemble data message in binary format **/

  /* set source source/destination addresses: 0x1/0x1 for now*/
  bmsg = atalk->send_msg_binary;
  *bmsg++ = 0x11;

  /* update and set the send sequence number for current message */
  atalk->send_seq_num++;
  *bmsg++ = atalk->send_seq_num;

  /* set reserved byte before data */
  *bmsg++ = ATALK_RESERVED_MARK;

  /* set data to send */
  for (int i=0; i < data_size; i++)
	  *bmsg++ = *data++;

  /* set reserved byte after data */
  *bmsg++ = ATALK_RESERVED_MARK;

  /* add CRC */
  crc = crc_ccitt (atalk->send_msg_binary,data_size + 4, 0xffff, 0);
  *bmsg++ = crc >> 8 & 0xff;
  *bmsg = crc & 0xff;

  /* encode to ASCII base 16 or base 64 */
  msglen = atalk->encode(atalk->send_msg_binary,data_size + 6,
    atalk->send_msg_encoded + 1);

  /* add start, end characters and string terminator */
  atalk->send_msg_encoded[0] = ATALK_MSG_START;
  atalk->send_msg_encoded[msglen + 1] = ATALK_MSG_END;
  atalk->send_msg_encoded[msglen + 2] = 0;

  /* send encoded message over data link */
  wrlen = atalk->msgtx(atalk);

  /* if send error, update counter */
  if (wrlen < 0) {
    atalk->send_errors++;
    atalk->errdat = wrlen;
    return ATALK_TX_ERROR;
  }
  else if (wrlen < msglen) {
    atalk->send_errors++;
    atalk->errdat = wrlen;
    return ATALK_SEND_INCOMPLETE;
  }
  /* if send success, update counters */
  else {
   atalk->sent_chars += wrlen;
   atalk->sent_msgs++;
   atalk->sent_data += data_size;
    return ATALK_SEND_SUCCESS;
  }
}


int atalk_receive (struct aTalk *atalk,uint8_t *data,uint8_t *data_size) { 

  int error;
  char *msg;
  char *msgend;
  int msglen;
  int receive_size;
  time_t now;
  uint16_t crc;
  uint8_t *bmsg;

  /* try to receive an ASCII encoded message from the physical data
   * link using the user defined receiving function. Read must be
   * blocking with timeout. */
  msglen = atalk->msgrx(atalk);

  /* if there is a message ... */
  if (msglen > 0) {

    /* find the position of message start char */
    msg = strchr(atalk->receive_msg_encoded,ATALK_MSG_START);

    /* if msg start not found */
    if (msg == NULL) {

      /* update error counter and ignore message */
      atalk->receive_errors++;

      return ATALK_MSG_NO_START;
    }
 
    /* find the position of message end char */
    msgend = strchr(msg,ATALK_MSG_END);

    /* if msg end not found */
    if (msgend == NULL) {

      /* update error counter and ignore message */
      atalk->receive_errors++;

      return ATALK_MSG_NO_END;
    }
    
    /* strip start and end charaters from received message and
     * set string terminator */
    msg++;
    *msgend = '\0';

    /* decode it from ASCII base 16 or base 64 to binary */
    error = atalk->decode(msg,atalk->receive_msg_binary,&receive_size);
    *data_size = (uint8_t)receive_size - 6;

    /* if decode error ... */
    if (error) {

      /* update error counter and ignore message */
      atalk->receive_errors++;

      /* save decode error code */
      atalk->errdat = error;

      return ATALK_DECODE_ERROR;
    }

    /* CRC on the whole data including CRC: new CRC must be zero. */
    crc = crc_ccitt (atalk->receive_msg_binary,receive_size, 0xffff, 0);

    /* if decode error or CRC error ... */
    if (crc) {

      /* update error counter and ignore message */
      atalk->receive_errors++;

      /* save wrong crc */
      atalk->errdat = crc;

      return ATALK_CRC_ERROR;
    }
  }

  /* if link rx error */
  else {

    /* update error counter and ignore message */
    atalk->receive_errors++;

    /* save link rx error */
    atalk->errdat = msglen;

    return ATALK_RX_ERROR;
  }

  /* copy data to output buffer */
  bmsg = atalk->receive_msg_binary + 3;
  for (int i=0; i < *data_size; i++)
    *data++ =  *bmsg++;

  /* update counters */
  atalk->received_chars += receive_size;
  atalk->received_msgs++;
  atalk->received_data += *data_size;

  /* check sequence number */
  atalk->receive_seq_num++;
  if (atalk->receive_msg_binary[1] != atalk->receive_seq_num) {
    atalk->receive_seq_num = atalk->receive_msg_binary[1];
    if (atalk->received_msgs != 1)
      return ATALK_SEQ_NUM_ERROR;
  }

  return ATALK_RECEIVE_SUCCESS;
}

/**** END ****/
