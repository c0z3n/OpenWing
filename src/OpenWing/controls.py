# Controls.py - the python interface for programming with the OpenWing project
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

class Controls(object):
    def __init__(self, comport, baud=9600):
        # top level variables to store OpenWing control status
        self.fader      = list((0, 0, 0, 0, 0, 0, 0, 0))
        self.button     = list((0, 0, 0, 0))
        self.trackball  = list((0, 0))
        self.track_diff = list((0,0))

        # configuration and communication info
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

        if not 0 <= lednum <= 4:
            raise ValueError("invalid lednum provided, \"%s\" is out of range" %lednum)

        self.outbound.append("lx" + chr(lednum) + chr(255-color[0]) + chr(255-color[1]) + chr(255-color[2]))


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

                # make sure we are aligned properly on the string by looking for a key of "dx"
                while True:
                    a1 = s.read(1)
                    if a1 == "d":
                        a2 = s.read(1)
                        if a2 == "x":
                            break
                    # print "misaligned"

                # get the list of raw bytes as chars from the serial
                rawdata = s.read(16)
                listdata = list()
                # convert them into a list of usable numbers
                for n in xrange(len(rawdata)):
                    listdata.append(ord(rawdata[n]))

                # apply that the new data to the control objects
                self.button = listdata[:4]
                self.fader = listdata[4:12]

                self.track_diff = list((0,0))

                self.trackball[0] += listdata[12]
                self.track_diff[0] += listdata[12]

                self.trackball[1] += listdata[15]
                self.track_diff[1] += listdata[15]

                self.trackball[0] -= listdata[14]
                self.track_diff[0] -= listdata[14]

                self.trackball[1] -= listdata[13]
                self.track_diff[1] -= listdata[13]


        except Exception as e:
            print e


if __name__ == "__main__":
    c = Controls(3)
    while True:
        print repr(c.button), repr(c.trackball),repr(c.fader)
        time.sleep(0.1)

