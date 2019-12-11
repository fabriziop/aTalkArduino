# .+ 
#
# .context    : aTalkArduino, real time data communication
# .title      : src make file
# .kind       : make file
# .author     : Fabrizio Pollastri <mxgbot@gmail.com>
# .site       : Revello - Italy
# .creation   : 19-Nov-2019
# .copyright  : (c) 2019 Fabrizio Pollastri
# .license    : GNU Lesser General Public License version 3
# 
# .-

.PHONY: clean pull

pull:
	# get copy of aTalk core common files
	rm -rf ./atalk
	cp -rdp ../../aTalk/src atalk
	rm -f ./atalk/*.o ./atalk/*.a
	rm ./atalk/Makefile

clean:
	rm -f *.o *.a
	rm -f ./atalk/*.o ./atalk/*.a
	
#### END ####
