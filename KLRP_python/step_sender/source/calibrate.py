import msvcrt

import key_move as km




points=[]

if __name__ =="__main__":
    o = km.Options()
    while True:

        c = msvcrt.getch()

        if c==b's':
            print("save:",o.ab)
            points.append(o.ab.copy())
        else:
            if c== b'd':
                print("p1",points[-3]-points[-2])
                print("p2",points[-3]-points[-1])
            else:
                if km.interpret_char(c, o) == -1:
                    break



