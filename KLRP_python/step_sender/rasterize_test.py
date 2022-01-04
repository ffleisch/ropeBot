import math

import test_sending as ts
import numpy as np
from matplotlib import pyplot as plt
import matplotlib.patches as patches
import scipy.spatial
microstepping=8

def mm_to_step(mm):
    return int(mm*((256*microstepping)/75.8))



def steps_to_mm(steps):
    return steps*(75.8/(256*microstepping))

ax=0
bx=1954


l1=1184

l2=1134



def xy_to_ab(x,y):
    return math.sqrt((ax-x)*(ax-x)+y*y),math.sqrt((bx-x)*(bx-x)+y*y)

def ab_to_xy(a,b):
    x=(b*b-a*a-bx*bx+ax*ax)/(2*(ax-bx))
    try:
        return  x,math.sqrt(a*a-(ax-x)*(ax-x))
    except ValueError as e:
        print(e)
        plt.show();
        return 0

start_x, start_y=ab_to_xy(l1, l2)


print("Startpos=", start_x, start_y)


a_pos=mm_to_step(l1)
b_pos=mm_to_step(l2)

def line_matrix(sx,sy,ex,ey):
    dx = ex - sx
    dy = ey - sy
    fac = 1 / (dx * dx + dy * dy)
    matrix = [[dx * fac, dy * fac], [-dy * fac, dx * fac]]
    return matrix


last_x=start_x
last_y=start_y

def raster_line(ex, ey):
    global a_pos,b_pos
    global last_x,last_y
    sx=last_x
    sy=last_y
    if sx!=ex or sy!=ey:
        matrix=line_matrix(sx,sy,ex,ey)
        #print(matrix)
        l_mid=float("inf")
        steps=[]
        x_list=[]
        y_list=[]
        while True:
            min_d=float("inf")
            min_l=0
            min_ad=0
            min_bd=0
            for i in [-1,0,1]:
                for j in [-1, 0, 1]:
                    if i!=0 or j!=0:
                        a=a_pos+i
                        b=b_pos+j
                        x,y=ab_to_xy(steps_to_mm(a),steps_to_mm(b))
                        #print(a,b,x,y)

                        l,d=np.matmul(matrix,(x-sx,y-sy))
                        d=abs(d)
                        #print(x,y,l,d)
                        if l>l_mid:
                            if d<min_d:
                                min_d=d
                                min_l=l
                                min_ad=i
                                min_bd=j
            a_pos+=min_ad
            b_pos+=min_bd
            steps.append((min_ad,min_bd))
            x_t,y_t=ab_to_xy(steps_to_mm(a_pos),steps_to_mm(b_pos))
            #plt.plot(x_t, y_t)
            #print(x_t,y_t,a_pos,b_pos)

            x_list.append(x_t)
            y_list.append(y_t)
            l_mid=min_l
            if min_l>=1:
                break

            #if min_d == float("inf"):
            #    break
        last_x=ex
        last_y=ey
        #print(steps)
        #print(x_list,y_list)
        plt.plot(x_list,y_list,"-k")

        #plt.show()
        return steps
    else:
        return []
def raster_line_add(dx,dy):
    return raster_line(last_x+dx,last_y+dy)




#steps=raster_line_add(0,250)

#ts.do_steps(ts.reverse(steps))

def draw_line_add(x,y,do_send=True):
    steps = raster_line_add(x, y)
    if do_send:
        ts.do_steps(steps)

def draw_line_to(x,y,do_send=True):
    steps = raster_line(start_x+x,start_y+ y)
    if do_send:
        ts.do_steps(steps)




def plot_csv_line_list(path,offset,scale,do_send=True,do_convex_hull=False):
    with open(path,"r") as f:
        #print(f)
        parts=f.readline().split(",")
        ox=float(parts[0])*scale
        oy=float(parts[1])*scale
        start_offset=(offset[0]-ox,offset[1]-oy)
        #draw_line_to(offset[0], offset[1], do_send)
        points=[]
        for l in f:
            parts=l.split(",")
            x=float(parts[0])*scale+start_offset[0]
            y=float(parts[1])*scale+start_offset[1]
            points.append((x,y))
            #print(x,y)

        if do_convex_hull:
            hull=scipy.spatial.ConvexHull(points)
            points_new=[]
            for v in hull.vertices:
                points_new.append(points[v])
            points=points_new
        perc=0
        for i,p in enumerate(points):
            n_perc=int(100*float(i)/len(points))
            if n_perc>perc:
                perc=n_perc
                print(perc)
            draw_line_to(p[0], p[1], do_send)
    #draw_line_to(0,0,do_send)

#steps=raster_line_add(100,0)
#print(ts.sum_steps(steps))
#ts.do_steps(steps)


#steps=raster_line_add(0,100)
#print(ts.sum_steps(steps))
#ts.do_steps(steps)

#steps=raster_line_add(-100,0)
#print(ts.sum_steps(steps))
#ts.do_steps(steps)




#steps=raster_line_add(0,-100)
#print(ts.sum_steps(steps))
#ts.do_steps(steps)


n=7
m=1
r=125

height=r
pitch=20
'''
while r>5:
    for i in range(n+1):
        angle=-2*3.141592*(i)/n-3.141592/2
        print(angle,math.cos(angle),math.sin(angle))
        draw_line_to(0+r*math.cos(angle),height+r*math.sin(angle))
    r-=pitch

#draw_line_to(0,height)
i=0
while r>10:
    angle=-m*2*3.141592*(i)/n-3.141592/2
    print(angle,math.cos(angle),math.sin(angle))
    draw_line_to(0+r*math.cos(angle),height+r*math.sin(angle))
    i+=1
    r-=pitch/n

draw_line_to(0,0)

'''

#plt.clf()
plt.axes().set_aspect("equal")
rect=patches.Rectangle((0,0),bx,1000,linewidth=1,edgecolor="r",facecolor="none")
plt.axes().add_patch(rect);
plt.gca().invert_yaxis()



plot_csv_line_list("./path_files/path_dragon_very_fine.csv",(0,0),0.33,True,False)


plt.show()

draw_line_to(0,0)
'''

#for i in range(len(steps)):
#    steps[i]=(steps[i][0]*-1,steps[i][1]*-1)

#print(steps)

#ts.do_steps(ts.reverse(steps))
'''