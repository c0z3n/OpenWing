# openwing.py - the python interface for programming with the OpenWing project
# full repository located at github.com/c0z3n/OpenWing
# contact gordon.p.clement@gmail.com
#
#     Copyright (C) Gordon Clement 2012
#
#     This program is free software: you can redistribute it and/or modify
#     it under the terms of the GNU General Public License as published by
#     the Free Software Foundation, either version 3 of the License, or
#     (at your option) any later version.
#
#     This program is distributed in the hope that it will be useful,
#     but WITHOUT ANY WARRANTY; without even the implied warranty of
#     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#     GNU General Public License for more details.
#
#     You should have received a copy of the GNU General Public License
#     along with this program.  If not, see http://www.gnu.org/licenses.

import serial
import threading
import time
from collections import deque

class OpenWing(object):
    def __init__(self, comport, baud=57600):
        # top level variables to store OpenWing control status
        self.fader      = list((0, 0, 0, 0, 0, 0, 0, 0))
        self.button     = list((0, 0, 0, 0))
        self.trackball  = list((0, 0))
        self.track_diff = list((0,0))

        # configuration and communication info
        self.controlraw = list((0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0))
        self.ip_address = "0.0.0.0"
        self.comport    = comport
        self.baud       = baud
        self.outbound   = deque()
        self.inbound    = deque()
        self.delay      = 0
        self.thread     = None
        self.end_thread = False

        # spawn our thread to update control values
        self.spawn_updater()

    def setLED(self, lednum, color):
        """ set one of the device LEDs to a certain color.
            lednum should be the number of the led to set, 0-3
            color should be a tuple of three color values (r,g,b), each 0-255
        """
        # validate the parameters
        for c in color:
            if not 0 <= c <= 255:
                raise ValueError("invalid color value provided, \"%s\" is out of range" %c)
            if c == 255: # 255 is a reserved value
                c = 254

        if not 0 <= lednum <= 4:
            raise ValueError("invalid lednum provided, \"%s\" is out of range" %lednum)



        self.outbound.append(chr(0) + chr((lednum+1)) + chr(255-color[0]) + chr(255-color[1]) + chr(255-color[2]))
        print "setting color", str(lednum), str(255-color[0]), str(255-color[1]), str(255-color[2])


    def spawn_updater(self):
        """create the thread to update control values 
           from the device
        """
        self.thread = threading.Thread(target=self.updater_thread)
        self.thread.daemon = False
        self.thread.start()


    def updater_thread(self):
        """ define the control updating procedure that 
            will be threaded out into the background.
        """
        try:
            s = serial.Serial(self.comport, self.baud)
            while not self.end_thread:
                # see if we have anything to send to the device
                if len(self.outbound) > 0:
                    s.write(self.outbound[0])
                    self.outbound.popleft()

                numbytes = 0
                # make sure we are aligned properly on the string by looking for a key of 00
                # followed by a packet size byte
                if s.inWaiting() > 3:
                    key = ord(s.read(1))+ord(s.read(1))
                    if key > 0:
                        # print "something's wrong"
                        s.flushInput()
                        continue

                    numbytes = ord(s.read(1)) # grab the number of bytes we need to read off the buffer
                    rawdata = s.read(numbytes*2) # get the list of raw bytes as chars from the buffer
                    listdata = list()
                    for n in xrange(len(rawdata)): # convert them into a list of usable numbers
                        listdata.append(ord(rawdata[n]))

                    # apply that the new data to the control objects
                    control_ids = list(listdata[0::2])
                    control_vals = list(listdata[1::2])
                    for n in xrange(len(control_ids)):
                        self.controlraw[control_ids[n]-1] = control_vals[n]

                    self.fader = self.controlraw[0:8]
                    self.button = self.controlraw[8:12]
                    self.trackball[0] += (self.controlraw[12]-self.controlraw[13])
                    self.trackball[1] += (self.controlraw[14]-self.controlraw[15])

        except Exception as e:
            print e


if __name__ == "__main__":
    c = OpenWing(12)
    while True:
        print repr(c.button),repr(c.fader),repr(c.trackball)
        time.sleep(0.1)

