# output_disp.py - an easy graphical display 
# of the current state of the OpenWing device

# requires pyprocessing - http://code.google.com/p/pyprocessing/


from pyprocessing import *
from openwing import OpenWing

def setup():
    size(800,400, caption="OpenWing Viewer")
    
def draw():
    background(0)
    fader_array(160, 68, 80)
    button_array(40, 60, 70)

def draw_button(x, y, ind):
    """draw a single button based on a button on the OpenWing
    """
    noStroke()
    f_col = 128*c.button[ind] + 64
    fill(f_col)
    rect(x, y, 60, 60)


def draw_fader(x, y, ind):
    """draw a single fader based on a fader on the OpenWing
    """
    noStroke()
    fill(196)
    rect(x, y, 22, 255)
    fill(64)
    rect(x, y, 22, 255-c.fader[ind])

def fader_array(x, y, spacing):
    for ind in xrange(8):
        draw_fader(x+spacing*ind, y, 7-ind)

def button_array(x, y, spacing):
    for ind in xrange(4):
        draw_button(x, y+spacing*ind, 3-ind)

c = OpenWing(3)
run()
