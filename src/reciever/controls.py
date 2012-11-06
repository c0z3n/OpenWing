import time
import serial
import threading
from collections import deque

COMPORT = 3
BAUD = 9600


class Controls(object):
    def __init__(self):
        # top level variables to store OpenWing control status
        self.faders = list((0,0,0,0,0,0,0,0))
        self.buttons = list((False, False, False, False))
        self.battery = None
        self.ip_address = "0.0.0.0"
        self.comport = COMPORT

        # configuration and communication info
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

        try:
            s = serial.Serial(comport, baud)

            while not self.end_thread:
                # process incoming messages
                raw = s.readline()
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