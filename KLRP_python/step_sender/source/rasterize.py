import math

import numpy as np
import test_sending as ts
from matplotlib import pyplot as plt

class RobotConfig:
    def __init__(self):
        self.ax = 0
        self.bx = 1954

        self.l1 = 1184

        self.l2 = 1134

        self.microstepping = 8

    def mm_to_step(self,mm):
        return int(mm*((256*self.microstepping)/75.8))

    def steps_to_mm(self,steps):
        return steps*(75.8/(256*self.microstepping))


    def xy_to_ab(self,x,y):
        return math.sqrt((self.ax-x)*(self.ax-x)+y*y),math.sqrt((self.bx-x)*(self.bx-x)+y*y)

    def ab_to_xy(self,a,b):
        x=(b*b-a*a-self.bx*self.bx+self.ax*self.ax)/(2*(self.ax-self.bx))
        try:
            return  x,math.sqrt(a*a-(self.ax-x)*(self.ax-x))
        except ValueError as e:
            print(e)
            return 0


class Path:
    def calc_local_coordinates(self,x,y):
        return 0,0

class LinePath(Path):
    def __init__(self,sx,sy,ex,ey):
        self.matrix=self.line_matrix(sx,sy,ex,ey)
        self.sx=sx
        self.sy=sy
        self.ex=ex
        self.ey=ey
    def line_matrix(self,sx,sy,ex,ey):
        dx = ex - sx;
        dy = ey - sy;
        fac = 1 / (dx * dx + dy * dy);
        matrix = [[dx * fac, dy * fac], [-dy * fac, dx * fac]];
        return matrix

    def calc_local_coordinates(self,x,y):
       l,d= np.matmul(self.matrix,(x-self.sx,y-self.sy))
       return  l, abs(d)

class CirclePath(Path):


    def __init__(self,sx,sy,ex,ey,cx,cy):
        self.cx=cx
        self.cy=cy
        self.sx=sx
        self.sy=sy
        self.ex=ex
        self.ey=ey

        self.radius_s=np.linalg.norm((sx-cx,sy-cy))
        self.radius_e=np.linalg.norm((ex-cx,ey-cy))
        print(self.radius_s,self.radius_e)
        self.radius=max(self.radius_s,self.radius_e)
        self.start_angle=math.atan2(sx-cx,sy-cy)

    def calc_local_coordinates(self,x,y):
        radius_diff=np.linalg.norm((x-self.cx,y-self.cy))-self.radius

        angle=math.atan2(x-self.cx,y-self.cy)-self.start_angle
        print(math.atan2(x-self.cx,y-self.cy),self.start_angle)
        return angle,abs(radius_diff)
class Rasterizer:
    def __init__(self,config,do_draw=True,do_plot=False):
        self.config=config
        self.root_x,self.root_y=config.ab_to_xy(config.l1,config.l2)
        self.a_pos=config.mm_to_step(config.l1)
        self.b_pos=config.mm_to_step(config.l2)
        self.do_plot=do_plot
        self.do_draw=do_draw

    def  rasterize(self,path):
        # print(matrix)
        l_mid = float("inf")
        steps = []



        x_list = []
        y_list = []

        while True:
            min_d = float("inf")
            min_l = 0
            min_ad = 0
            min_bd = 0
            for i in [-1, 0, 1]:
                for j in [-1, 0, 1]:
                    if i != 0 or j != 0:
                        a = self.a_pos + i
                        b = self.b_pos + j
                        x, y = self.config.ab_to_xy(self.config.steps_to_mm(a), self.config.steps_to_mm(b))
                        # print(a,b,x,y)

                        l, d = path.calc_local_coordinates(x,y)
                        # print(x,y,l,d)
                        if l > l_mid:
                            if d < min_d:
                                min_d = d
                                min_l = l
                                min_ad = i
                                min_bd = j
            self.a_pos += min_ad
            self.b_pos += min_bd
            steps.append((min_ad, min_bd))

            if self.do_plot:
                x_t, y_t = self.config.ab_to_xy(self.config.steps_to_mm(self.a_pos), self.config.steps_to_mm(self.b_pos))

                x_list.append(x_t)
                y_list.append(y_t)


            l_mid = min_l
            if min_l >= 1:
                break

        if self.do_plot:
            plt.plot(x_list, y_list, "-k")

        # plt.show()
        return steps

    def get_xy(self):
        return self.config.ab_to_xy(self.config.steps_to_mm(self.a_pos),self.config.steps_to_mm(self.b_pos))

    def draw_segment(self,path):
        steps=self.rasterize(path)
        if self.do_draw:
            ts.do_steps(steps)

    def draw_line_to(self,x,y):
        sx,sy=self.get_xy()
        line=LinePath(sx,sy,self.root_x+x,self.root_y+y)
        self.draw_segment(line)


    def draw_line_add(self,x,y):
        sx,sy=self.get_xy()
        line=LinePath(sx,sy,sx+x,sy+y)
        self.draw_segment(line)




def plot_csv_line_list(path,rasterizer,offset,scale):
    with open(path,"r") as f:
        #print(f)
        parts=f.readline().split(",")
        ox=float(parts[0])*scale
        oy=float(parts[1])*scale
        start_offset=(offset[0]-ox,offset[1]-oy)
        #draw_line_to(offset[0], offset[1], do_send)




        for l in f:
            parts=l.split(",")
            x=float(parts[0])*scale+start_offset[0]
            y=float(parts[1])*scale+start_offset[1]
            rasterizer.draw_line_to(x,y)


if __name__=="__main__":

    my_config=RobotConfig()
    my_rasterizer=Rasterizer(my_config,do_plot=True,do_draw=False)

    x,y=my_rasterizer.get_xy()
    c=CirclePath(x+0,y+0,x+100,y+100,x+0,y+100)
    my_rasterizer.draw_segment(c)
    #plot_csv_line_list("path_files/path.csv",my_rasterizer,(0,0),.5)

    plt.show()
