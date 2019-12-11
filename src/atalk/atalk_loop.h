/* .+

.context    : aTalk, real time data communication
.title      : tx/rx communication loop definitions
.kind       : c source
.author     : Fabrizio Pollastri <mxgbot@gmail.com>
.site       : Revello - Italy
.creation   : 11-Aug-2019
.copyright  : (c) 2019 Fabrizio Pollastri
.license    : GNU Lesser General Public License version 3

.- */

#ifndef ATALK_LOOP_H
#define ATALK_LOOP_H

#include <string.h>
#include <stdlib.h>

#include "atalk.h"
#include "queue.h"

#define SUCCESS 0
#define ATALK_TX_FULL -1
#define ATALK_RX_DATA_NOT_AVAILABLE -2


int atalk_loop_init(struct aTalk *atalk,uint8_t depth);

int atalk_loop_tx(struct aTalk *atalk);

int atalk_loop_rx(struct aTalk *atalk);

#endif	/* ATALK_LOOP_H */

/**** end ****/
