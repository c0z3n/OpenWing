from pysimpledmx import DMXConnection
from openwing import OpenWing

c = OpenWing(3)
dmx = DMXConnection(7)

while True:
    dmx.setChannel(1, c.fader[0])
    dmx.setChannel(2, c.fader[1])
    dmx.setChannel(3, c.fader[2])
    dmx.render()