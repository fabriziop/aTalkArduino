/* .+

.context    : aTalk, real time data communication
.title      : core functions definitions
.kind       : c source
.author     : Fabrizio Pollastri <mxgbot@gmail.com>
.site       : Revello - Italy
.creation   : 28-Jul-2019
.copyright  : (c) 2019 Fabrizio Pollastri
.license    : GNU Lesser General Public License version 3

.- */

#ifndef ATALK_H
#define ATALK_H

#include <string.h>
#include <time.h>

#include "base16.h"
#include "base64.h"
#include "crc_ccitt.h"


/* fixed parameters */

#define ATALK_RESERVED_MARK 0xff       /* reserved bytes filler */
#define ATALK_MSG_START '{'            /* message start character */
#define ATALK_MSG_END '}'              /* message end character */
#define ATALK_REC_MSG_ENC_GUARD 32     /* add room for non msg chars */

/* message body encoding index */
enum aTalkEncoder {BASE16=1, BASE64};


/** data definitions **/

struct aTalk {
    /* user init parameters */
    uint8_t data_size_max;
    enum aTalkEncoder encoder;
    /* computed parameters */
    uint16_t msg_encoded_len_max;
    uint16_t receive_buf_enc_len;
    /* sender */
    int (*msgtx)(struct aTalk *);
    uint8_t *send_msg_binary;
    uint8_t send_msg_bin_size;
    int (*encode)(uint8_t *,int,char *);
    char *send_msg_encoded;
    uint16_t send_msg_enc_len;
    uint8_t send_seq_num;
    uint32_t sent_chars;
    uint32_t sent_msgs;
    uint32_t sent_data;
    uint16_t send_errors;
    /* receiver */
    int (*msgrx)(struct aTalk *);
    char *receive_msg_encoded;
    uint16_t receive_msg_enc_len;
    int (*decode)(char *,uint8_t *,int *);
    uint8_t *receive_msg_binary;
    uint8_t receive_msg_bin_size;
    uint8_t receive_seq_num;
    uint32_t received_chars;
    uint32_t received_msgs;
    uint32_t received_data;
    uint16_t receive_errors;
    /* data link driver data */
    void *data_link;
    /* general */
    uint32_t errdat;
};


/* error codes */

enum aTalkSendErrors {
    ATALK_SEND_SUCCESS                = 0,
    ATALK_TX_ERROR                    = -1,
    ATALK_SEND_INCOMPLETE             = -2,
    ATALK_SEND_DATA_SIZE_OVER_MAX     = -3,
};

enum aTalkReceiveErrors {
    ATALK_RECEIVE_SUCCESS             = 0,
    ATALK_SEQ_NUM_ERROR               = -1,
    ATALK_RX_ERROR                    = -2,
    ATALK_MSG_NO_START                = -3,
    ATALK_MSG_NO_END                  = -4,
    ATALK_DECODE_ERROR                = -5,
    ATALK_CRC_ERROR                   = -6
};


/** function prototypes **/

struct aTalk *atalk_init(uint8_t data_size_max,enum aTalkEncoder encoder);

int atalk_send(struct aTalk *atalk,uint8_t *data,uint8_t data_size);

int atalk_receive(struct aTalk *atalk,uint8_t *data,uint8_t *data_size);

#endif    /* ATALK_H */

/**** END ****/
