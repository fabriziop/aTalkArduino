===================================
aTalk, real time data communication
===================================

aTalk is an implementation of a real time data link layer communication
protocol based on the exchange of ASCII encoded messages. This design has
two main pourposes. The first is to keep the maximum compatibility with
comunication channels that do not tolerate well binary data, i.e. serial
interfaces. The second is to allow an easy inspection the content of the
messages for monitoring, debugging, etc.

Here is an example of an aTalk message with a payload of 11 bytes of
binary data. aTalk messages are surrounded by brackets that mark the
start and the end of each message. The two following character strings
represent the message coded in hexadecimal ASCII (base 16) and coded
in base 64 ASCII respectively.

  **{1110ff6c963715ffa4ce92dcabbaff7514}**

  **{EQH/xmlzUf9K7CnNuqv/V0E=}**

When the consumption of communication bandwidth is critital, the base 64
encoding is the better choice, it has only 33% of band overhead against
the original binary data. Otherwise, the hexadecimal encoding is more
useful for debug or monitoring pourposes, since it is far more human
readable than the base 64 encoding.


Features
========

* real time oriented communication protocol.
* no message handshake.
* messages base 16 or base 64 ASCII encoded.
* data payload from 0 to 255 bytes.
* data link error detection.
* lost message detection.
* C language lightweight implementation.
* hardware data link drivers API.
* drivers for serial interface and local software loop included.
* supported platforms: Linux and Arduino.
* tested on PC (cpu 64bit), arduino nano (cpu AVR328), NodeMCU (cpu ESP8266),
  NodeMCU 32s (cpu ESP32), CORE-ESP32 (cpu RISC-V).


Package structure
=================

The package is splitted in several git repositories. One repository with
an aTalk library for each supported platform and one repository for the aTalk
core files common to all platforms.
These are source libraries to be compiled togheter with the application
software.
The repositories names are::

  * aTalk         the aTalk core common files
  * aTalkArduino  the aTalk library for Arduino
  * aTalkLinux    the aTalk library for Linux

Also the following examples using the aTalk libraries are provided.

* single host local communication loop
* dual host remote communication loop


A first example
===============

To begin with aTalk programming, the `Linux-Arduino communication example`__
is a good starting point.

__ LINARDEX_


Installation
============

For installation details see the example above.


aTalk API
=========

struct aTalk* **atalk_init(** uint8_t **data_size_max**, enum aTalkEncoder **encoder)**

  Called once by the application before to start the aTalk communication,
  wich means before to call *atalk_send* or *atalk_receive*. It allocates the
  required data structures and it inits all required variables.

  **data_size_max**: the maximum size (number of bytes) of binary data to be
  exchanged by aTalk messages.
  
  **encoder**: the requested message encoding, BASE16 or BASE64.
  
  **return data**: a pointer to the control structure of aTalk. If pointer
  is NULL *atalk_init* failed.


int **atalk_send(** struct aTalk* **atalk**, uint8_t* **data**, uint8_t **data_size)**

  Called by the application each time it is needed to send a buffer of binary
  data by an aTalk message.

  **atalk**: a pointer to the control structure of aTalk.

  **data**: a pointer to the binary data buffer to be sent with an aTalk
  message.
  
  **data_size**: the size of the binary data buffer to be sent.
  
  **return data**: error code, =0 function successful, <0 error happened.


int **atalk_receive(** struct aTalk* **atalk**, uint8_t* **data**, uint8_t* **data_size)**

  Called by he application to receive an aTalk message. The function
  terminates when an aTalk message is received or when no message is
  received whitin a given time out interval (see data link driver).

  **atalk**: a pointer to the control structure of aTalk.

  **data**: a pointer to the binary data buffer where to write the binary
  data received by an aTalk message.
  
  **data_size**: the size of the received binary data.
  
  **return data**: error code, =0 function successful, <0 error happened.


Data link drivers
=================

* `Arduino serial driver`__

__ ARDSERIAL_

* `Linux serial driver`__

__ LINSERIAL_


Contributing
============

Send wishes, comments, patches, etc. to mxgbot_a_t_gmail.com .

aTalk internals can be found at `Developer information`__ .


__ DEVINFO_


Copyright
---------

aTalk is authored by Fabrizio Pollastri <mxgbot_a_t_gmail.com>, year 2019, under the GNU Lesser General Public License version 3.


.. _LINARDEX: doc/linux_arduino_example.rst
.. _ARDSERIAL: doc/arduino_serial.rst
.. _LINSERIAL: doc/linux_serial.rst
.. _DEVINFO: doc/developer.rst

.. ==== END ====
