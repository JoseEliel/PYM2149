# PYM2149
Interfacing a YM2149 sound generating chip with any USB MIDI controller using an Arduino UNO. 
The setup has two main parts: 

* C code for the arduino microcontroller to be able to talk to the YM2149 chip. This part makes use of the code makes use of an extension of Florent Flament's code in https://github.com/FlorentFlament/ym2149-test. It adds up extra features: Defines the necessary notes for driving 4 octaves including sharp notes, it adds the functionality to control volume and pitch with MIDI channels defined in the python code and the possibility on putting an envelope to the output also using a MIDI channel defined in the python code. 

* Python code for reading in the input from a USB MIDI Controller and translating it into intructions for the arduino to talk to the YM2149 chip, with the possibility of assigning MIDI channels to pitch, volume and envelope. It uses the MIDI interface features included in Pygame modules (http://www.pygame.org)


# Quick start

* Upload the code to the Arduino: 
Due to timing issues the code for the microcontroller has to be uploaded to the Arduino without using the Arduino IDE. You can follow Florent Flament's tutorial here: http://www.florentflament.com/blog/arduino-hello-world-without-ide.html. Use the code in the ArduinoFirmware folder. Be sure to change the Makefile to have the correct port for the Arduino's serial communication.

* Setup the electronics 
Follow the schematics for setting up your circuit as in https://github.com/FlorentFlament/ym2149-test. 

* Setup your USB MIDI controller
Add to the Python code in the PYM folder the correct port for your MIDI input. 

* Ready, Steady, Go! 
Just run pym.py in the command line. The code should print pressed notes as: 

Timestamp: 58075ms, Channel: 53, Value: 17
([1, 1, 0], [53, 64, 53])
Timestamp: 58103ms, Channel: 62, Value: 73
([1, 1, 1], [53, 64, 62])
Timestamp: 58162ms, Channel: 64, Value: 0
([1, 0, 1], [53, 64, 62])
Timestamp: 58194ms, Channel: 53, Value: 0


