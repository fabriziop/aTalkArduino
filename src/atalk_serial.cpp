/* .+

.context    : aTalk, real time data communication
.title      : aTalk, asynchronous serial link driver
.kind       : c++ source
.author     : Fabrizio Pollastri <mxgbot@gmail.com>
.site       : Revello - Italy
.creation   : 18-Oct-2019
.copyright  : (c) 2019 Fabrizio Pollastri
.license    : GNU Lesser General Public License version 3

.description

Implementation of an asynchronous serial link driver for the aTalk software.

.- */


#include "atalk_serial.h"


int atalk_serial_init(struct aTalk *atalk,int speed,uint16_t receive_timeout) {
  struct aTalkSerial *serial;

  /* allocate aTalkSerial structure */
  serial = (struct aTalkSerial *)malloc(sizeof(struct aTalkSerial));
  atalk->data_link = serial;

  /* save arguments */
  serial->speed = speed;
  serial->receive_timeout = receive_timeout;

  /* set serial tx and rx functions */
  atalk->msgtx = atalk_serial_tx;
  atalk->msgrx = atalk_serial_rx;

  /* init serial with requested speed */
  Serial.begin(speed);

  return 0;
}


int atalk_serial_tx(struct aTalk *atalk) {

  int msglen;
  int txlen;
  int error;

  /* send message body */
  msglen = strlen(atalk->send_msg_encoded);
  txlen = Serial.write(atalk->send_msg_encoded,msglen);

  /* check for error */
  if (msglen < txlen) {
    atalk->errdat = txlen;
    return ATALK_SERIAL_TX_PARTIAL;
  }

  /* send line terminator */
  txlen = Serial.write("\n",1);
  if (txlen != 1)
    return ATALK_SERIAL_TX_NL_ERROR;

  return msglen;
}


int atalk_serial_rx(struct aTalk *atalk) {

  struct aTalkSerial *serial;
  int len;
  int avail;
  char *rxbuf;
  unsigned long int start;
  unsigned long int now;

  /* receive all incoming charaters up to line terminator, timeout 
   * or buffer full */
  rxbuf = atalk->receive_msg_encoded;
  serial = (struct aTalkSerial *)atalk->data_link;
  start = millis();
  len = 0;

  while (millis() - start < serial->receive_timeout) {

    avail = Serial.available();

    for (int c=0; c < avail; c++) {

      /* check for receive buffer overflow */
      if (len >= atalk->receive_buf_enc_len - 1) {

        /* add string terminator character */
        *rxbuf = '\0';

        /* save the number of read characters */
        atalk->receive_msg_enc_len = len + 1;

        return ATALK_SERIAL_RXBUF_FULL;
      }

      /* read one char into receive msg encoded buffer */
      *rxbuf = Serial.read();
      len++;

      /* if line terminator: success, return the number of read characters
       * including line terminator. */
      if (*rxbuf == '\n') {

        /* add string terminator character */
        *++rxbuf = '\0';

        return len + 1;
      }
      rxbuf++;
    }
  }

  /* add string terminator character */
  *++rxbuf = '\0';

  /* save the number of read characters */
  atalk->receive_msg_enc_len = len + 1;

  return ATALK_SERIAL_RX_TIMEOUT;
}

/**** end ****/
