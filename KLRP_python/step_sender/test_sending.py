import serial
import time

print("Hello wall")

arduino=None
try:
    arduino = serial.Serial(port='COM5', baudrate=115200, timeout=.5)

except serial.serialutil.SerialException:
    print("No Aruino Connected")


def write_read(x):
    arduino.write(x)
    time.sleep(0.05)
    data = arduino.readline()
    return data


def do_cardinal(left,right,num):
    '''b=0
    if(left==1 or left==-1):
        b|=0b0100
    if(left==1):
        b|=0b1000

    if (right == 1 or right == -1):
        b |= 0b0001
    if (right == -1):
        b |= 0b0010
    #print(bin(b))
        #write_read(data)
    #num*=2
    while num>0:
        chunk=min(chunk_size,num)
        data=[b]*chunk
        #data=[b if i%2==0 else 0 for i in range(chunk)]
        num-=chunk
        #print(data)
        arduino.write(bytes(data))
        #time.sleep(0.005)
        res = arduino.readline()
        print(res)
        try:
            res=int(res)
        except ValueError:
            res=0

        print(res)
        if res>500:
            time.sleep(0.5)
        arduino.flushInput()'''
    steps=[]
    for i in range(num):
        steps.append((left,right))
    do_steps(steps)


chunk_size=16
buffer_limit=128

def do_steps(steps):

    print("len",len(steps))
    print("steps",steps)
    while(len(steps)>0):
        chunk=steps[:chunk_size]
        steps=steps[chunk_size:]
        #print(chunk)

        data=[]
        for c in chunk:

            left=c[0]
            right=c[1]
            b=0
            if(left!=0):
                b|=0b0100
            if(left>0):
                b|=0b1000

            if (right !=0):
                b |= 0b0001
            if (right <0):
                b |= 0b0010
            data.append(b)

            #print(c,b)
        #print(data)

        arduino.write(bytes(data))
        def read_ard_buff_size():
            #arduino.flushInput()
            res = arduino.readline()
            #print(res)
            try:
                res = res.split()[1]
                res = int(res)
            except ValueError:
                res = 0
            except IndexError:
                res=0
            print(res)
            return res
        res =read_ard_buff_size()
        #print(res)
        #if(res==0):
        #    print("Arduino Buffer empty")

        if(res==255):
            raise Exception("Arduino Buffer full")
        while True:
            res=read_ard_buff_size()
            if res==255:
                raise Exception("Arduino Buffer full")

            if res<buffer_limit:
                break
            #time.sleep(0.025)

        arduino.flushInput()


def reverse(steps):
    steps_new=[]

    for s in steps:
        steps_new.insert(0,(s[0]*-1,s[1]*-1))
    return steps_new

def sum_steps(steps):
    a=0
    b=0
    for s in steps:
        a+=s[0]
        b+=s[0]
    print(a,b)

if arduino is not None:
    do_cardinal(0,0,100)


if __name__=="__main__":
    time.sleep(1)
    #print(arduino.readline())

    '''for i in range(6):

        num=i*1000;
        do_cardinal(1,0,num)
        do_cardinal(0,1,num)
        do_cardinal(-1,0,num)
       '''# do_cardinal(0,-1,num)
    #do_cardinal(-1,-1,500)
    test_steps=[(-1 if i%2==0 else 0,1) for i in range(1024)]

    do_steps(test_steps)
    do_steps(reverse(test_steps))



    print( test_steps)
    print(reverse(test_steps))
    print(sum_steps(test_steps))
    print(sum_steps(reverse(test_steps)))
    #do_cardinal(-1,0,10000)

