===================
Linux serial driver
===================

The Linux serial driver for aTalk is able to send and receive aTalk messages
through asynchronous serial interfaces: Linux devices like */dev/ttyUSB0,
/dev/ttyS0, etc.*. The driver implements the standard aTalk driver API model
of 3 C functions:

  * atalk_serial_init
  * atalk_serial_tx
  * atalk_serial_rx

The driver source files live in the directory *src* of the aTalkLinux
repository::

  .
  └─ aTalkLinux
     ├── examples    communication examples
     ├── src         data link drivers        <====
     └── test        development testing


Driver API
==========

int **atalk_serial_init(** struct aTalk* **atalk**, char* **device**, int **speed**, uint16_t **receive_timeout)**

  Called once by the application after the call to *atalk_init* and before
  to start the aTalk communication, wich means before to call *atalk_send*
  or *atalk_receive*. It sets the communication interface parmeters and
  connects functions **atalk_serial_tx** and **atalk_serial_rx** to aTalk core.

  **atalk**: a pointer to the control structure of aTalk, returned by function.

  **device**: the serial Linux device name to be used by aTalk, i.e.
  "/dev/ttyUSB0".

  **speed**: the communication speed (baud rate) to be set to the Linux
  serial interface.

  **receive_timeout**: the maximum time (ms) to wait when reading from the
  serial interface (call to *atalk_receive*). If no complete aTalk message
  is read within this interval of time, *atalk_receive* will return an
  error code.

  **return value**: 0 for init successful, -1 for failure.

Fanctions *atalk_serial_tx* and *atalk_serial_rx* are managed internally by
aTalk, so no user knowledge is needed.


Copyright
---------

aTalk is authored by Fabrizio Pollastri <mxgbot_a_t_gmail.com>, year 2019, under the GNU Lesser General Public License version 3.

.. ==== END ====
