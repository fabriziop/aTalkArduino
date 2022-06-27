===================================
Linux-Arduino communication example
===================================

To introduce aTalk programming, let see this example of communication
between a Linux system and an Arduino board. The hardware setup can be a PC
running Linux, connected with a USB cable to the USB port of the Arduino
board. The setup was tested with two Arduino boards: nano and NodeMCU v3.

Software installation and building requires some C language developement
tools (C compiler, linker and make) and the Arduino IDE developement tool.

The program running on the Linux system generates random blocks of binary
data with a random size. Since the USB connection to Arduinos behaves like a
serial connection, the program uses aTalk to send each block to the Arduino
through the USB connection, one block at a time.
The program running on the Arduino board uses aTalk to receive each block.
If the data block is received without errors, the program uses aTalk again to
send it back to the Linux system.
Finally, the program running on the Linux system uses aTalk to receive the
data block sent back by the Arduino and the receive data block is checked
for identity with the original random data that started the trasmission chain.


**Linux installation**

The part of this example to be installed on the Linux system can be found
on *github*. To clone it in a your dir do::

  $ cd <YOUR_ATALK_ROOT_DIR>
  $ git clone https://github.com/fabriziop/aTalkLinux.git

Alternatively, one can dowload a zip file from *github*: press the green
button on the left at page https://github.com/fabriziop/aTalkLinux, the file
aTalkLinux-master.zip is dowloaded on your system. Then unzip it into a dir
selected to be the aTalkLinux root.

Build the example with::

  $ cd <YOUR_ATALK_ROOT_DIR>
  $ make


**Arduino installation**

For the installation on the Arduino board,
`the Arduino IDE <https://www.arduino.cc/en/Main/Software>`_ is a very
convenient developement tool running on the Linux system. So the part of
this example to be installed on the Arduino board is developed to be compatible
with this tool.

First the aTalk Arduino library needs to be installed by the Arduino IDE.
In the Arduino IDE on the Linux system, open the library manager window
(menu: Sketch > Include Library > Manage Libraries). Use the search filter
of the library manager to select the *aTalkArduino* library, then click the
install button. For more info see `library manager
<https://www.arduino.cc/en/Guide/Libraries#toc3>`_ .

Now, the example *atalk_echo* needs to be built and uploaded to the Arduino
board. Open the example selecting from the Arduino IDE menu File > Examples,
then scroll the Examples list untill the item *aTalkArduino*, click it and
select from the just opened list the item *atalk_echo*.
Connect the Linux system to the Arduino board with an USB cable, typically
this connection is seen fron the Linux system as an */dev/ttyUSBn* device.
Where n is generally 0 if no other USB hardware is connected.
In the Arduino IDE, select the proper Arduino board (menu: Tools > Boards),
i.e. *Arduino nano*, and the proper connection port (menu: Tools > Port),
i.e. */dev/ttyUSB0*. Finally, click the *upload* button of the Arduino IDE
and wait for upload to complete.


**Running the example**

With the Arduino board already connected to the Linux system, run on the
Linux system::

  $ cd <YOUR_ATALK_ROOT_DIR>/examples/
  $ ./atalk_remote_loop

The program asks for the Arduino connection device, it defaults to
*/dev/ttyUSB0* . After the aswer, the program start a testing communication
with the Arduino board. Each message sent is printed out togheter with
the corresponding message echoed by the Arduino board.
An excerpt of the program printout follows::

  $ ./atalk_remote_loop 
  device connecting Arduino [/dev/ttyUSB0]: 
  TX: {1110ff6c963715ffa4ce92dcabbaff7514}
  RX: {1110ff6c963715ffa4ce92dcabbaff7514}
  TX: {1120ffbf3e64c72c458fb18e7ed867a5e23633f99ca96623d07b13ff1428}
  RX: {1120ffbf3e64c72c458fb18e7ed867a5e23633f99ca96623d07b13ff1428}
    ...
  TX: {1136ff19c69ef912cb2531b1a24f67bd4af193803fa8f298251f48ff1c1f}
  RX: {1136ff19c69ef912cb2531b1a24f67bd4af193803fa8f298251f48ff1c1f}
  TX: {1146ff1733a1cc30d2d5f661cf093df43aee97895645c348d8447771ffafd3}
  RX: {1146ff1733a1cc30d2d5f661cf093df43aee97895645c348d8447771ffafd3}
  TEST OK
  $ 


Copyright
=========

aTalk is authored by Fabrizio Pollastri <mxgbot_a_t_gmail.com>, year 2019, under the GNU Lesser General Public License version 3.

.. ==== END
