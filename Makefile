# .+ 
#
# .context    : aTalkArduino, real time data communication
# .title      : main make file
# .kind       : make file
# .author     : Fabrizio Pollastri <mxgbot@gmail.com>
# .site       : Revello - Italy
# .creation   : 9-Dec-2019
# .copyright  : (c) 2019 Fabrizio Pollastri
# .license    : GNU Lesser General Public License version 3
# 
# .-

.PHONY: clean pull


clean:
	$(MAKE) -C examples clean
	$(MAKE) -C src clean
	rm -f *.o *.a

pull:
	rm -f README.rst
	rm -rf doc
	cp -rdp ../aTalk/README.rst .
	cp -rdp ../aTalk/doc .
	$(MAKE) -C src pull

#### END ####
