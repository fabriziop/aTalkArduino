/* .+

.context    : aTalk, real time data communication
.title      : serial echo line by line
.kind       : c source
.author     : Fabrizio Pollastri <mxgbot@gmail.com>
.site       : Revello - Italy
.creation   : 7-Nov-2019
.copyright  : (c) 2019 Fabrizio Pollastri
.license    : GNU Lesser General Public License version 3

.description

This program echoes any line received on the serial line. A line is a sequence
of ascii characters terminated by a '\n' (newline) character. The line is
echoed immediately after newline reception.

.- */

#define LINE_LEN_MAX 256
#define NEWLINE '\n'


byte line[LINE_LEN_MAX];
int line_len;

void setup() {

  Serial.begin(9600);

}

void loop() {

  /* read line by line */
  line_len = Serial.readBytesUntil(NEWLINE,line,LINE_LEN_MAX);
 
  /* if no error ... */
  if (line_len > 0) {

    /* add line terminator stripped by read */
    line[line_len] = '\n';

    /* echo received line */
    Serial.write(line,line_len+1);      
  } 
}

/**** END ****/
