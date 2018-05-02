# Created by José Eliel Camargo-Molina 
# Copyright 2018


import sys
import os
import pygame
import pygame.midi
from pygame.locals import *
import serial
import time
 
port = "/dev/tty.usbmodem1421"
baud = 57600

# display a list of MIDI devices connected to the computer
def print_device_info():
    for i in range( pygame.midi.get_count() ):
        r = pygame.midi.get_device_info(i)
        (interf, name, input, output, opened) = r
        in_out = ""
        if input:
            in_out = "(input)"
        if output:
            in_out = "(output)"
        print ("%2i: interface: %s, name: %s, opened: %s %s" %
               (i, interf, name, opened, in_out))
#Pygame initialization
pygame.init()
pygame.fastevent.init()
event_get = pygame.fastevent.get
event_post = pygame.fastevent.post
pygame.midi.init()
#Opening the serial port
serialCOM = serial.Serial(port, baud, timeout=1)
#Print if serial is open
if serialCOM.isOpen():
     print(serialCOM.name + ' is open...')
#Print MIDI keyboard Being used
print ("Available Keyboards:")
print_device_info();
# Change this to override use of default input device
device_id = None
if device_id is None:
    input_id = pygame.midi.get_default_input_id()
else:
    input_id = device_id
print ("Using input_id: %s" % input_id)
i = pygame.midi.Input( input_id )

going = True

serialdata="49"
currentlyBeingPlayed=[0,0,0] #tracks notes being played
ChannelState=[0,0,0] #tracks which channels are on. Corresponds with currentlyBeingPlayed
currentChan=0 #Last Channel that turned on
EnvelopeData='500' #No Envelope to start

while going:#Here MIDI is read
    events = event_get()
    for E in events:
        if E.type in [QUIT]:
            going = False
        if E.type in [KEYDOWN]:
            going = False
        if E.type in [pygame.midi.MIDIIN]:
        # Translate keypress to Serial data
            print ("Timestamp: " + str(E.timestamp) + "ms, Channel: " + str(E.data1) + ", Value: " + str(E.data2))

            if E.data1==1:# Here we choose the envelope freq. 
                #print("Envelope: ", E.data2)
                EnvelopeData=str(500+E.data2)
                #print(serialdata)
                if(ChannelState!=[0,0,0]): serialCOM.write(EnvelopeData+'\n')
               
                
            elif E.data1==0 or E.data1==127:#Here we set the change in Freq
                #print("Slinky: ", E.data2)
                serialdata=str(628+E.data2)
                print(628+E.data2-692)
                serialCOM.write(serialdata+'\n')
                if(ChannelState[0]==1): serialCOM.write(str(currentlyBeingPlayed[0]-36+100)+'\n')
                if(ChannelState[1]==1): serialCOM.write(str(currentlyBeingPlayed[1]-36+200)+'\n')
                if(ChannelState[2]==1): serialCOM.write(str(currentlyBeingPlayed[2]-36+300)+'\n')

            elif E.data1==7:
                volume=str((E.data2*12)/127)
                print("Volume set to: ", volume)
                serialCOM.write(volume+'\n')
                serialCOM.write('500'+'\n')

            else:

                if E.data2==0 and E.data1 in currentlyBeingPlayed:#The conditionals make it so that notes are turned off properly in each channel. 
                    whichChannel=[x for x in range(3) if currentlyBeingPlayed[x]==E.data1][0]
                    ChannelState[whichChannel]=0
                    print(ChannelState,currentlyBeingPlayed)
                    ChannelFactor=(whichChannel+1)*100
                    serialdata=str(ChannelFactor+49)
                    #print(serialdata)
                    serialCOM.write(serialdata+'\n')
                    if(ChannelState==[0,0,0]): serialCOM.write('500'+'\n')

                elif E.data2==0: #Do nothing if a note pressed long ago is released
                    pass
                else:
                    currentChanList=[x for x in range(3) if ChannelState[x]==0]
                    if len(currentChanList)!=0:
                        currentChan=currentChanList[0]
                        ChannelState[currentChan]=1
                        currentlyBeingPlayed[currentChan]=E.data1
                        ChannelFactor=(currentChan+1)*100
                        serialdata=str(E.data1-36+ChannelFactor)
                        #print(serialdata)
                        serialCOM.write(serialdata+'\n')#writing the ouput to serial
                        serialCOM.write(EnvelopeData+'\n')
                        #out = serialCOM.read(3) #reading what Arduino says
                        print(ChannelState,currentlyBeingPlayed)
                    else:
                        print("Just three notes at the time man!") 
    
    # if there are new data from the MIDI controller
    if i.poll():
        midi_events = i.read(10)
        midi_evs = pygame.midi.midis2events(midi_events, i.device_id)
        for m_e in midi_evs:
            event_post( m_e )