import numpy as np
import source.rasterize as rs

def calc_origin(d_p1,d_p2,k_p1,k_p2,w):

    def error(a,b):
        p1_ab=[a+d_p1[0],b+d_p1[1]]
        p2_ab=[a+d_p2[0],b+d_p2[1]]

        p1_diff=

        return 0



    pass

s0=np.array([-7112,  1160])
s1=np.array([ 3032, -9080])
s2=np.array([-2280, 4616])



my_robot_config=rs.RobotConfig()


def point_steps_to_mm(p):
    return [my_robot_config.steps_to_mm(p[0]),my_robot_config.steps_to_mm(p[1])]

def point_mm_to_steps(p):
    return [my_robot_config.mm_to_step(p[0]),my_robot_config.mm_to_step(p[1])]


def ab_to_xy(p):
    return rs.RobotConfig.ab_to_xy(my_robot_config,p[0],p[1])




p1=np.array(point_steps_to_mm(s0-s2))
p2=np.array(point_steps_to_mm(s0-s2))



if __name__=="__main__":
    calc_origin(p1,p2,(0,297),(420,0),1954)
    print(p1,p2)
    pass
