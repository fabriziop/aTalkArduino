/* .+

.context    : aTalk, real time data communication
.title      : aTalk, asynchronous serial link driver
.kind       : c source
.author     : Fabrizio Pollastri <mxgbot@gmail.com>
.site       : Revello - Italy
.creation   : 18-Oct-2019
.copyright  : (c) 2019 Fabrizio Pollastri
.license    : GNU Lesser General Public License version 3

.description

Implementation of an asynchronous serial link driver for the aTalk
software.

.- */

#ifndef ATALK_SERIAL_H
#define ATALK_SERIAL_H

#include <Arduino.h>

#include "atalk/atalk.h"


/* return error codes */
enum aTalkSerialErrors {
  ATALK_SERIAL_RX_ERROR = -1,
  ATALK_SERIAL_RX_TIMEOUT = -2,
  ATALK_SERIAL_RXBUF_FULL = -3,
  ATALK_SERIAL_TX_PARTIAL = -5,
  ATALK_SERIAL_TX_NL_ERROR = -6
};


/* data definitions */

struct aTalkSerial {
  int speed;
  unsigned long int receive_timeout;
};


/* functions prototipes */

extern "C" {

int atalk_serial_init(struct aTalk *atalk,int speed,uint16_t receive_timeout);

int atalk_serial_tx(struct aTalk *atalk);

int atalk_serial_rx(struct aTalk *atalk);

}

#endif	/* ATALK_SERIAL_H *

/**** end ****/
