.. LedControl documentation master file, created by sphinx-quickstart on Mon Feb
   4 12:31:17 2013.  You can adapt this file completely to your liking, but it
   should at least contain the root `toctree` directive.


The Arduino LedControl library
==============================
The LedControl library provides methods to control a ``MAX7221``
or a ``MAX7219`` chip which are both used to that drives a Led matrix 
or a 7-segment display from an Arduino board.  

A detailed description of the hardware and a
schematic can be found `here <hardware>`.

As a teaser here is a picture of my rather crappy (but working) testbed...

.. image:: MAX7221Setup.jpg
   :align: center

The LedControl library is not the first attempt to write . There is already a
library and a lot of code-examples for the Arduino and the MAX72XX available,
but the focus had always been on controlling Leds layed out in some sort of
rectangular matrix. I use the MAX72XX to drive 7-segment displays, so I wanted a
function to display numbers (decimal and hexa-decimal) and also the limited set
of alphanumeric characters that make (visual) sense on this kind of
displays. But the library also provides a basic set of functions by which either
individual or groups of Leds can be switched on and off.

The library documentation is devided into three parts

- :doc:`Hardware and schematics <hardware>`
- :doc:`Documenation of the library functions<software>`
- :doc:`Democode for using the the library <demos>`

.. toctree::
   :hidden:

   hardware
   software
   demos

Known bugs 
********** 
Currently none (with Arduino 0022 and Arduino 1.0 (RC1))

Revision History 
**************** 
*February 27, 2013*
    Moved code to `Github <http://github.com>`_ and documentation to
    `Read The Docs <https://readthedocs.org>`_
*September 10, 2012* 
    Changed the software license from LGPL to an MIT-style license.  
*September 19, 2011* 
    Uploaded new version of LedControl.zip.  Release 1.0 of the
    Arduino-IDE renamed the internal header file ``WProgramm.h`` to 
    ``Arduino.h``. The include statement in ``LedControl.h`` was
    updated so the library compiles under pre- and post-1.0 version 
    of the IDE 
*October 14, 2008* 
    Uploaded a new version of the LedControl.zip. The original 
    version did not compile under arduino-0012.
*December 5, 2007* 
    Documentation and sourcecode is now hosted on the arduino
    playground 
*June 23, 2007* 
    First public release


Feedback 
******** 
Your first stop should be the arduino forum section
``Hardware/Interfacing`` which I visit regulary. But you are also welcome to
send questions, objections or corrections to <e.fahle@wayoda.org>

License (The software) 
********************** 
The sourcecode for this library is released under the Terms of the 
GNU Lesser General Public License version 2.1.


