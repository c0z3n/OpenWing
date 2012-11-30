import serial
import threading
from collections import deque
import time

COMPORT = 3
BAUD = 9600


class Controls(object):
    def __init__(self):
        # top level variables to store OpenWing control status
        self.fader = list((0,0,0,0,0,0,0,0))
        self.button = list((0, 0, 0, 0))



        # configuration and communication info
        self.ip_address = "0.0.0.0"
        self.comport = COMPORT
        self.baud = BAUD
        self.outbound = deque()
        self.inbound = deque()
        self.comport = 0
        self.delay = 0
        self.thread = None
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
            if not 0<=c <= 255:
                raise ValueError("invalid color value provided, out of range")
                return
        if not 0<=lednum<=4:
            raise ValueError("invalid lednum provided, out of range.")
            
        self.outbound.append("lx" + chr(lednum) + chr(color[0]) + chr(color[1]) + chr(color[2]))


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
            s = serial.Serial(COMPORT, BAUD)

            while not self.end_thread:

                # see if we have anything to send to the device
                if len(self.outbound) > 0:
                    s.write(self.outbound[0])
                    self.outbound.popleft()

                # make sure we are aligned properly on the string by looking for a key of "dx"
                while True:
                    a1=s.read(1)
                    if a1 == "d":
                        a2 = s.read(1)
                        if a2 == "x":
                            break
                    print "misaligned"

                # get the list of raw bytes as chars from the serial
                rawdata = s.read(12)
                listdata = list()

                # convert them into a list of usable numbers
                for n in xrange(len(rawdata)):
                    listdata.append(ord(rawdata[n]))

                # apply that the new data to the control objects
                self.button = listdata[:4]
                self.fader = listdata[4:12]

                # print repr(self.fader), repr(self.button)

        except Exception as e:
            print e


