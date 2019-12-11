/* .+

.context    : aTalk, real time data communication
.title      : fifo queue
.kind       : c source
.author     : Fabrizio Pollastri <mxgbot@gmail.com>
.site       : Revello - Italy
.creation   : 18-Aug-2019
.copyright  : (c) 2019 Fabrizio Pollastri
.license    : GNU Lesser General Public License version 3

.description

Implementation of a FIFO queue of generic items.

.- */

#include "queue.h"


struct Queue *queue_init(uint8_t depth) {

  struct Queue *queue;

  /* allocate queue structures */
  queue = (struct Queue *)malloc(sizeof(struct Queue));
  queue->items =
    (struct QueueItem *)malloc(sizeof(struct QueueItem) * depth);

  /* init queue to empty */
  queue->first_item = 0;
  queue->last_item = 0;
  queue->items_num_max = depth;
  queue->items_num = 0;
  queue->item_push = 0;
  queue->item_pop = 0;

  return queue;

}


int queue_push(struct Queue *queue,void *item_body) {

  uint8_t item;

  /* if queue full, return proper code. */
  if (queue->items_num >= queue->items_num_max)
    return QUEUE_FULL;

  /* init queue item */
  item = queue->item_push;
  queue->items[item].next = item;
  queue->items[item].body = item_body;

  /* add item to non empty queue */
  if (queue->items_num){
    queue->items[queue->last_item].next = item;
      queue->last_item = item;
  }
  /* add item to empty queue */
  else {
    queue->first_item = item;
    queue->last_item = item;
  }
  queue->items_num++;
  queue->item_push++;
  queue->item_push %= queue->items_num_max;

  return QUEUE_SUCCESS;

}


void *queue_pop(struct Queue *queue) {

  uint8_t item;

  /* if queue empty, return proper code */
  if (queue->items_num == 0)
    return NULL;

  /* remove item from queue */
  item = queue->first_item;
  queue->first_item = queue->items[item].next;
  queue->items_num--;
  queue->item_pop++;
  queue->item_pop %= queue->items_num_max;

  return queue->items[item].body;
    
}


bool queue_is_full(struct Queue *queue) {

    if (queue->items_num >= queue->items_num_max)
	return true;
    else
	return false;

}


bool queue_is_empty(struct Queue *queue) {

    if (queue->items_num == 0)
	return true;
    else
	return false;

}

/**** end ****/
