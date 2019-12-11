/* .+

.context    : aTalk, real time data communication
.title      : serial echo char by char
.kind       : c source
.author     : Fabrizio Pollastri <mxgbot@gmail.com>
.site       : Revello - Italy
.creation   : 30-Sep-2019
.copyright  : (c) 2019 Fabrizio Pollastri
.license    : GNU Lesser General Public License version 3

.description

This program echoes back immediately any character received on the serial line.

.- */


char rxchar;

void setup() {

    Serial.begin(9600);

}


void loop() {
 
    if (Serial.available() > 0) {

        rxchar = Serial.read();
        Serial.print(rxchar);
    }
}

/**** END ****/
