import time
import serial
import threading
from collections import deque

COMPORT = 0
BAUD = 9600


class Controls(object):
    def __init__(self):
        # top level variables to store OpenWing control status
        # create these all as lists, so we can modify them by reference easily
        self.faders = list((0,0,0,0,0,0,0,0))
        self.buttons = list((False, False, False, False))
        self.power = list((None))
        self.ip_address = list(("0.0.0.0"))

        # configuration and communication info
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


    def spawn_updater(self):
        """create the thread to update control values 
           from the device
        """
        self.thread = threading.Thread(target=self.updater_thread)
        self.thread.daemon = False
        self.thread.start()


    def updater_thread(self, comport=COMPORT, baud=BAUD):
        """ define the control updating procedure that 
            will be threaded out into the background.
        """

        # create references to all of the control state lists to avoid sending extra characters
        f=self.faders
        b=self.buttons
        p=self.power
        ip=self.ip_address

        try:
            s = serial.Serial(comport, baud)

            while not self.end_thread:

                # process incoming messages
                raw = s.readline() #read everything up to the next endline

                #print repr(raw)

                notraw = raw.rstrip('\r\n')
                try:
                    exec(notraw)
                except:
                    print "exec failed"
                    pass

                print repr(self.buttons)
                # parsing on raw data to be added here

                # do our outbound communication
                for n in xrange(len(self.outbound)):
                    s.write(self.outbound[n])
                    self.outbound.popleft()

                time.sleep(self.delay)

                # # flush our buffers
                # s.flushInput()
                # s.flushOutput()
        except Exception as e:
            print e