/* .+

.context    : aTalk, real time data communication
.title      : tx/rx communication loop
.kind       : c source
.author     : Fabrizio Pollastri <mxgbot@gmail.com>
.site       : Revello - Italy
.creation   : 11-Aug-2019
.copyright  : (c) 2019 Fabrizio Pollastri
.license    : GNU Lesser General Public License version 3

.description

Implementation of a software trasmission loop for testing pouposes.
The functions (atalk_tx and atalk_rx) that the user must supply to aTalk to
connect it with the hardware to send and to receive protocol messages are
defined here. In this implementation, atalk_tx stores messages into
a fifo queue and atalk_rx reads messages fron the same queue.

.- */

#include "atalk_loop.h"


int atalk_loop_init(struct aTalk *atalk,uint8_t queue_depth) {

  struct Queue *queue;

  /* init queue and check for error. */
  queue = queue_init(queue_depth);
  if (queue == NULL)
    return QUEUE_INIT_FAIL;
  atalk->data_link = (void *)queue;

  /* set serial tx and rx functions */
  atalk->msgtx = atalk_loop_tx;
  atalk->msgrx = atalk_loop_rx;

  return 0;

}


int atalk_loop_tx(struct aTalk *atalk) {

  struct Queue *queue;
  char *message_copy;
  int error;
  int msglen;

  /* if queue is not full, allocate a buffer for msg and copy into
   * it the incoming msg, then push message into queue. */
  queue = atalk->data_link;
  if (!queue_is_full(queue)) {
    msglen = strlen(atalk->send_msg_encoded);
    message_copy = (char *)malloc(msglen + 1);
    strcpy(message_copy,atalk->send_msg_encoded);
    error = queue_push(queue,message_copy);
    if (error)
      return error;
  }
  else
    return ATALK_TX_FULL;

  return msglen;

}


int atalk_loop_rx(struct aTalk *atalk) {

  struct Queue *queue;
  char *body;

  /* if queue empty, return proper code */
  queue = atalk->data_link;
  if (queue_is_empty(queue))
    return ATALK_RX_DATA_NOT_AVAILABLE;

  /* pop item bodyfrom queue */
  body = (char *)queue_pop(queue);

  /* copy message from queue to output buffer */
  strcpy(atalk->receive_msg_encoded,body);

  /* free body */
  free(body);

  return strlen(atalk->receive_msg_encoded);
    
}

/**** end ****/
