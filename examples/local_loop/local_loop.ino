/* .+

.context    : aTalk, real time data communication
.title      : random data send/receive closed on local loop
.kind       : c source
.author     : Fabrizio Pollastri <mxgbot@gmail.com>
.site       : Revello - Italy
.creation   : 12-Sep-2019
.copyright  : (c) 2019 Fabrizio Pollastri
.license    : GNU Lesser General Public License version 3

.description

This program sends blocks of random data with random size, the blocks are
trasmitted to a local loop and received from the same loop. The received
data is checked again the trasmitted data for errors.

.- */


#define NUMBER_OF_TESTS 10
#define DATA_SIZE_MIN 1
#define DATA_SIZE_MAX 32
#define MSG_ENCODER BASE64
#define QUEUE_DEPTH 4


#include <aTalk.h>

extern "C" {
#include <atalk/atalk_loop.h>
}

struct aTalk *atalk;
uint8_t random_data[DATA_SIZE_MAX];
uint8_t random_data_size;
uint8_t receive_data[DATA_SIZE_MAX];
uint8_t receive_data_size;
int error;


void setup() {

  Serial.begin(9600);
  
  /* init aTalk */
  atalk = atalk_init(DATA_SIZE_MAX,MSG_ENCODER);
  if (atalk == NULL) {
    Serial.println("ERROR: atalk_init failed to allocate aTalk struc.");
    exit(1);
  }
  
  /* init local loop */
  error = atalk_loop_init(atalk,QUEUE_DEPTH);
  if (error) {
    Serial.println("ERROR: atalk_loop_init failed to allocate Queue struc.");
    exit(1);
  }

}


void loop() {

    /* repeat the test, each time changing data */
    for (uint8_t i=0; i < NUMBER_OF_TESTS; i++) {

        /* generate a buffer of random data, with random size in the range
	       * DATA_SIZE_MIN - DATA_SIZE_MAX */
        random_data_size = rand() %
	          (DATA_SIZE_MAX - DATA_SIZE_MIN) + DATA_SIZE_MIN;
        for (unsigned int r=0; r < random_data_size; r++) {
	          random_data[r] = rand();
        }

        /* send random data */
	      error = atalk_send(atalk,random_data,random_data_size);

	      /* check for send errors */
        if (error) {
            Serial.print("ERROR ");
            Serial.print(error);
            Serial.print(" sending data at test #");
            Serial.println(i);
	          continue;
        }
	
        /* receive data */
  	    error = atalk_receive(atalk,receive_data,&receive_data_size);

	      /* check for receive errors */
        if (error) {
            Serial.print("ERROR ");
            Serial.print(error);
            Serial.print(" receiving data at test #");
            Serial.println(i);
	          continue;
        }
	
        /* compare received data with transmitted data */	
        for (int j=0; j < receive_data_size; j++) {
	          if (random_data[j] != receive_data[j]) {
                Serial.print("ERROR on data at test #");
                Serial.println(i);
	        	    Serial.print("ERROR receive data size = ");
	        	    Serial.println(receive_data_size);
                Serial.print("ERROR tx data[");
                Serial.print(j);
                Serial.print("] = ");
                Serial.println(random_data[j]);
		            Serial.print("ERROR rx data[");
		            Serial.print(j);
		            Serial.print("] = ");
		            Serial.println(receive_data[j]);
	              exit(1);
	          }
        }
    }
    
    Serial.println("TEST OK");
    delay(1000);
}
