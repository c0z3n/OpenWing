from controls import Controls
import win32api, win32con, time
SENS = 6
c = Controls(3)
last_b0 = last_b1 = b0 = b1 = 0

while(True):
    b0 = c.button[3]
    b1 = c.button[2]
    if(b0 == 1 and last_b0 == 0): win32api.mouse_event(win32con.MOUSEEVENTF_LEFTDOWN,0,0)

    if(b0 == 0 and last_b0 == 1): win32api.mouse_event(win32con.MOUSEEVENTF_LEFTUP,0,0)

    if(b1 == 1 and last_b1 == 0): win32api.mouse_event(win32con.MOUSEEVENTF_RIGHTDOWN,0,0)

    if(b1 == 0 and last_b1 == 1): win32api.mouse_event(win32con.MOUSEEVENTF_RIGHTUP,0,0)

    c_pos = win32api.GetCursorPos()
    n_pos = (c_pos[0] + c.track_diff[0]*SENS, c_pos[1] + c.track_diff[1]*SENS)
    win32api.SetCursorPos(n_pos)
    last_b0 = b0
    last_b1 = b1

    time.sleep(0.02)

