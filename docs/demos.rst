Democode for the LedControl-library
===================================
Here are a few demo-sketches that using the LedControl-library. You
can either copy+paste the code directly into the editor of the
Arduino-IDE, or download the sketches in a zip-file :

   ``LedControlDemos.zip``

.. note :: **LedControl must be installed first...**

   The LedControl-library must obviously be installed on your
   computer, otherwise the demos won't even compile...

.. toctree::
   :maxdepth: 1

LedMatrixDemo - Demo for a Led matrix
*************************************
Here is a small demo to be run on a Led matrix. A matrix of at least
5x7 Leds is recommended, but there is no problem running it on a
smaller matrix, it simply won't look that good.

Please have a close look at the initialization-code and the
``setup()``-function. If you don't to pull the ``MAX72XX`` from
powersaving mode with the ``shutdown()``-function or if you forget to
call ``setIntensiy()`` for setting the display brightness, your Leds
my remain dark even though the sketch runs without errors.


Otherwise the code should be straight forward. ``rows(), columns()``
and `` single()`` show the different ways to update the
matrix. ``writeArduinoOnMatrix()`` is a bit of eye-candy, that shows
how you could drive a small text display if you had a few spare
``MAX72XX``.  

.. note :: **Update Pin numbers...**

   Please update the pin-numbers used in the code according to you hardware!

