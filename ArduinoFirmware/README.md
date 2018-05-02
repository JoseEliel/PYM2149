#Arduino Firmware for PYM2149

C code for the arduino microcontroller to be able to talk to the YM2149 chip. This part makes use of the code makes use of an extension of Florent Flament's code in https://github.com/FlorentFlament/ym2149-test. It adds up extra features: Defines the necessary notes for driving 4 octaves including sharp notes, it adds the functionality to control volume and pitch with MIDI channels defined in the python code and the possibility on putting an envelope to the output also using a MIDI channel defined in the python code. 

Below you'll find the original README.md from Florent Flament's code: 


Title: ym2149-test

The ym2149-test is a little firmware to check that the [YM2149 &
Arduino circuit][2] is working properly.

Requirements
------------

The following libraries are required:

* avr-gcc
* avr-libc
* avrdude

How to test the circuit
-----------------------

    $ make
    $ make flash

More information
----------------

More information can be found on my blog:

* [Driving YM2149F sound chip with an Arduino][2]
* [Arduino Hello World without IDE][3]

Besides, a video showing the [YM2149 & Arduino circuit playing a tune][1] is
available.

[1]: https://www.youtube.com/watch?v=MTRJdDbY048
[2]: http://www.florentflament.com/blog/driving-ym2149f-sound-chip-with-an-arduino.html
[3]: http://www.florentflament.com/blog/arduino-hello-world-without-ide.html
