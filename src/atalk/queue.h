/* .+

.context    : aTalk, real time data communication
.title      : fifo queue
.kind       : c source
.author     : Fabrizio Pollastri <mxgbot@gmail.com>
.site       : Revello - Italy
.creation   : 11-Aug-2019
.copyright  : (c) 2019 Fabrizio Pollastri
.license    : GNU Lesser General Public License version 3

.- */

#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>


struct QueueItem {
    void *body;
    uint8_t next;
};

struct Queue {
    uint8_t first_item;
    uint8_t last_item;
    uint8_t items_num_max;
    uint8_t items_num;
    uint8_t item_push;
    uint8_t item_pop;
    struct QueueItem  *items;
};


struct Queue *queue_init(uint8_t depth);

int queue_push(struct Queue *queue,void *item_body);

void *queue_pop(struct Queue *queue);

bool queue_is_full(struct Queue *queue);

bool queue_is_empty(struct Queue *queue);


#define QUEUE_SUCCESS 0
#define QUEUE_FULL -1
#define QUEUE_EMPTY -2
#define QUEUE_INIT_FAIL -3

#endif	/* QUEUE_H */

/**** END ****/
