# .+ 
#
# .context    : aTalk, real time data communication
# .title      : doc make file
# .kind       : make file
# .author     : Fabrizio Pollastri <mxgbot@gmail.com>
# .site       : Revello - Italy
# .creation   : 8-Dec-2019
# .copyright  : (c) 2019 Fabrizio Pollastri
# .license    : GNU Lesser General Public License version 3
# 
# .-

.PHONY: all html clean

%.html: %.rst
	rst2html $< > $@

targets = arduino_serial.html developer.html linux_arduino_example.html \
	  linux_serial.html

all: html

html: $(targets)

clean:
	rm -f *.html

#### END ####
