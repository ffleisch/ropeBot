import numpy as np

import test_sending as ts
import msvcrt

class Options:
    def __init__(self):
        self.mode = 0
        self.increment = 1024
        self.delay = 2048
        self.ab=np.array([0,0])

def interpret_char(c, opt):
    if c == b'\xe0':
        c = msvcrt.getch()
        if opt.mode == 0:
            if (c == b"M"):
                print("left")
                opt.ab+=ts.do_cardinal(1, -1, opt.increment)
            if (c == b"K"):
                print("right")
                opt.ab+=ts.do_cardinal(-1, 1, opt.increment)
            if (c == b"H"):
                print("up")
                opt.ab+=ts.do_cardinal(-1, -1, opt.increment)
            if (c == b"P"):
                print("down")
                opt.ab+=ts.do_cardinal(1, 1, opt.increment)

        if opt.mode == 1:
            if (c == b"M"):
                print("left")
                opt.ab+=ts.do_cardinal(0, -1, opt.increment)
            if (c == b"K"):
                print("right")
                opt.ab+=ts.do_cardinal(0, 1, opt.increment)
            if (c == b"H"):
                print("up")
                opt.ab+=ts.do_cardinal(-1, 0, opt.increment)
            if (c == b"P"):
                print("down")
                opt.ab+=ts.do_cardinal(1, 0, opt.increment)
        print(opt.ab)
    if c == b'+':
        opt.increment *= 2
        print(opt.increment)
    if c == b'-':
        opt.increment = int(opt.increment / 2)
        opt.increment = max(1, opt.increment)
        print(opt.increment)

    if c == b'*':
        opt.delay *= 2
        ts.set_speed(opt.delay)
        print(opt.delay)

    if c == b'/':
        opt.delay /= 2
        ts.set_speed(opt.delay)
        print(opt.delay)

    if c == b'\x1b':
        return -1

if __name__ =="__main__":
    o = Options()
    ts.set_speed(o.delay)
    while True:

        c = msvcrt.getch()
        if interpret_char(c, o) == -1:
            break
