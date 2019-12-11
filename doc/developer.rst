===========================
aTalk Developer information
===========================

Information about aTalk internals, useful for aTalk developement.

Package layout
==============

The package layout of aTalk was designed having in mind to separate the files
common to all supported platforms from the files specific for each platform.
Since git is used for version management, the original idea was to have
only one git repository with 3 main subdirectories: one for common files, one
for Linux files and one for Arduino files. The last 2 directories pointing
to the common files through symbolic links.
Unfortunately, the format of Arduino IDE libraries do not allows symbolic
links and moreover git do not follows symbolic links.
Another way to implement a clean dependency of the Arduino and Linux files
from the aTalk comon files was to substitute the symbolic links using git
submodules. But again, the format of Arduino libraries do not allow git
submodules.
So to have the requested format compliance and to keep it simple, aTalk is
splitted into 3 git repositories:

* **aTalk**        aTalk common files
* **aTalkArduino** aTalk Arduino library with examples
* **aTalkLinux**   aTalk Linux library with examples
 
Each platform repository has a copy of the common files needed. To synchronize
the common file of aTalk with the platform repositories there is a specific
make target: do *make pull* in the root directory of each platform repository.


Directory layout
================

::

  .
  └─ aTalk
     ├── doc         documentation
     ├── src         aTalk common sources
     └── test        aTalk common sources testing programs

  .
  └─ aTalkArduino    Arduino library legacy files
     ├── examples    communication examples
     └── src         data link drivers

  .
  └─ aTalkLinux
     ├── examples    communication examples
     ├── src         data link drivers
     └── test        development testing


Data link drivers
=================

From the *Package layout*, platform specific files are separated from aTalk
common files that is the core of the data link communication. These platform
specific files are typically the drivers of the hardware communication
interfaces that are obviuosly platform dependent.

Each driver must implement the following C functions:

* **init** called once by the application before to start the aTalk
  communication, sets the communication interface parmeters and tells
  to aTalk core where are the **tx** and **rx** functions. The function
  prototype may change due to different interface parameters for different
  platforms, but the first and the last arguments are fixed::

    int <root_name>_init(struct aTalk *atalk,...<plat. depend. args>...,
        uint16_t receive_timeout);

  *receive_timeout* is the maximum wait time (milliseconds) for an aTalk
  message to be read. Used by **rx** function.

* **tx** called internally by aTalk core when a message is to be sent on the
  data link, it sends one aTalk message at each call. This function is called
  just after the application calls the *atalk_send* function and aTalk core
  completes the encoding of the the message to be sent. The function
  prototype is::

    int <root_name>_tx(struct aTalk *atalk);

* **rx** called internally by aTalk core to wait for a message coming from
  the data link. This happens when the application calls the *atalk_receive*
  function. This is a blocking function, so it is very appropriate to
  implement in this function a read timeout. The function prototype is::

    int <root_name>_rx(struct aTalk *atalk);


Drivers API model
=================

int **atalk_<INTERFACE>_init(** struct aTalk* **atalk,<plat. depend. args>)**

  This function init the driver, for further details see above.
  **INTERFACE** is the data link interface. For example, the function
  name for the serial interface is **atalk_serial_init**.

  **atalk**: a pointer to the control structure of aTalk.

  **...<plat. depend. args>...**: platform dependent arguments, for example
  the serial Linux driver has 3 arguments:

    * serial linux device, i.e */dev/ttyUSB0* ;
    * serial interface speed (baud rate);
    * serial receive timeout (milliseconds); 

  **return value**: 0 for init successful, -1 for failure.

 
int **atalk_<INTERFACE>_tx(** struct aTalk* **atalk)**

  This function trasmits an aTalk message over the physical data link.

  **atalk**: a pointer to the control structure of aT\alk.

  **return value**: >0 number of trasmitter characters (excluding
  terminator and newline characters, < 0 there are errors.
 

int **atalk_<INTERFACE>_rx(** struct aTalk* **atalk)**

  This function receive an aTalk message from the physical data link.

  **atalk**: a pointer to the control structure of aTalk.

  **return value**: >0 number of received message characters (excluding
  terminator and newline characters, =< 0 there are errors.


Message format
==============

Fig. 1, a byte breakdown scheme of aTalk message::

    0     1     2     3     4     5          L-5   L-4   L-3   L-2   L-1    L
 +-----+-----+-----+-----+-----+-----+-~ ~-+-----+-----+-----+-----+-----+-----+
 |  {  |sdadd| seq | res | ad0 | ad1 |.~ ~.|adN-2|adN-1| res | crc0| crc1|  }  |
 +-----+-----+-----+-----+-----+-----+-~ ~-+-----+-----+-----+-----+-----+-----+
 |start|<--- header ---->|<- app data -~ ~------------>|<--- trailer --->|term.|
       |<--- base 16 or 64 encoded ----~ ~------------------------------>|

- sdadd: source/destination address
- seq  : message sequence number
- adN  : application data byte, may be missing.
- res  : reserved, bits set to 0xff.
- crc0 : crc LSB
- crc1 : crc MSB
- L    : message length
- N    : application data length


Fig. 2, a byte breakdown scheme of aTalk message without application data::

    0     1     2     3     4     5     6     7
 +-----+-----+-----+-----+-----+-----+-----+-----+
 |  {  |sdadd| seq | res | res | crc0| crc1|  }  |
 +-----+-----+-----+-----+-----+-----+-----+-----+
 |start|<--- header ---->|<--- trailer --->|term.|
       |<----- base 16 or 64 encoded ----->|


Each message content is preceded by the start character "{" (left bracket)
and is followed by the termination character "}" (right bracket).

The message content is composed of 2 or 3 parts: a header of 3 bytes,
the application data with a variable number of char, this part may be absent,
and a trailer of 3 bytes.
If the whole message length is more than 8 bytes, application data is present.
If message length is equal to 8 bytes, no application data is present.
8 bytes is the minimum message length.

All chars between the start char and the termination char are encoded before
trasmission into ASCII printable characters. The encoding can be base 16 or
base 64 at user option.


Source/destination address byte
-------------------------------

The source/destination address bytes tells which application is the source
of the message and to wich application the message is to be routed.
Distination address can be used be aTalk as a multiplexing output address.

Address 0x0 refers to the default application.
Address 0xf refers to the atalk software at both ends of the data link.

Fig. 3, format of the source/destination address byte (message byte #1)::

 bit 7    6     5     4     3     2     1     0
 +-----+-----+-----+-----+-----+-----+-----+-----+
 | <--- source addr ---> | <- destination addr ->|
 +-----+-----+-----+-----+-----+-----+-----+-----+


Message sequence number
-----------------------

Each message sent has a send sequence number, starting from 0 for the first
message and increasing by one after each message is sent. The send sequence
number is kept in the range 0-255 applying a modulo 256 operation.
This number is used by the receiving side to detect message loss.


Application data
----------------

Application data, if present, it is trasported from one side of the
communication channel to the other side as is, without any change. The
communication protocol is completely agnostic about the content of
the application data.


CRC
---

The cyclic redundancy check (CRC) is computed over the whole message content,
excluding start, stop characters and the CRC itself.
The checksum algorithm is the CRC-CCITT 16 bits with seed = FFFF hex.
The CRC is complemented to one and written into message trailer.
At reception side, a message without errors exhibits a CRC-CCITT = 0,
if the CRC is computed over the whole message content, including the
received CRC, but excluding start and stop characters.


Copyright
---------

aTalk is authored by Fabrizio Pollastri <mxgbot_a_t_gmail.com>, year 2019, under the GNU Lesser General Public License version 3.

.. ==== END ====
