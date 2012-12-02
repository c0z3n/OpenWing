from pyprocessing import *
from controls import Controls

def setup():
    size(800,400, caption="OpenWing Viewer")
    
def draw():
    background(0)
    fader_array(160, 68, 80)
    button_array(40, 60, 70)

def do_button(x, y, ind):
    noStroke()
    f_col = 128*c.button[ind] + 64
    fill(f_col)
    rect(x, y, 60, 60)


def do_fader(x, y, ind):
    noStroke()
    fill(196)
    rect(x, y, 22, 255)
    fill(64)
    rect(x, y, 22, 255-c.fader[ind])

def fader_array(x, y, spacing):
    for ind in xrange(8):
        do_fader(x+spacing*ind, y, 7-ind)

def button_array(x, y, spacing):
    for ind in xrange(4):
        do_button(x, y+spacing*ind, 3-ind)

c = Controls(0)
run()
