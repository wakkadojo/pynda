#!/usr/bin/env python2

import sys
sys.path.append ('../build/pywrap/')
import pynda
import random
#import cv2
import numpy

cell_size = pynda.vec3d (0.2, 0.2, 0.2)
bi = pynda.interactor (0.5, 0.5)
dt = 0.001
w = pynda.world (cell_size, bi, dt)

b = pynda.brick (pynda.vec3d (0.25, 0.0, 0.0), pynda.vec3d (0.25, 0.25, 1.0))
w.add_brick (b)

r, m, I = 0.025, 0.025**2, 0.025**4/2

s = pynda.sphere (r, m, I, pynda.vec3d (), pynda.vec3d (), pynda.vec3d (0.0, 0.0, 10.0))
s.flag = pynda.sphere.state.fixed
s.r = 1.5*s.r
w.add_sphere (s)

L = 512
for i in range (500):
    w.step (5)
    print ('step ' + str (i) + ' ' + str (w.num_spheres ()))
    if i % 10 == 0:
        omegamax = 2*numpy.pi/5/r
        omega = pynda.vec3d (0, 0, omegamax*(-0.5 + random.random ()))
        v = pynda.vec3d (1.0, 0, 0)
        x = pynda.vec3d (-0.25, -0.05+0.1*random.random (), 0.0)
        s = pynda.sphere (r, m, I, x, v, omega)
        w.add_sphere (s)

    # rendering
    #img = 255 - numpy.zeros ((L, L, 3), numpy.uint8)
    #for j in range (w.num_spheres ()):
    #    s = w.get_sphere (j)
    #    xs, ys = int ((s.x[0]+0.5)*L), int ((s.x[1]+0.5)*L)
    #    rs, qs = int (s.r*L), s.q[2]
    #    color = (0, 0, 255) # BGR
    #    cv2.circle (img, (xs, ys), rs, color, -1, cv2.CV_AA)
    #    re = rs/5
    #    rs = rs-re
    #    cv2.circle (img, (xs, ys), rs, (0, 0, 0), re, cv2.CV_AA)
    #    cv2.line (img, 
    #              (xs, ys), 
    #              (int (xs + rs*numpy.cos (qs)), int (ys + rs*numpy.sin (qs))),
    #              (0, 0, 0), re, cv2.CV_AA)
    #for j in range (w.num_bricks ()):
    #    b = w.get_brick (j)
    #    x1, y1 = int ((b.x[0]-b.L[0]/2+0.5)*L), int ((b.x[1]-b.L[1]/2+0.5)*L)
    #    x2, y2 = int ((b.x[0]+b.L[0]/2+0.5)*L), int ((b.x[1]+b.L[1]/2+0.5)*L)
    #    cv2.rectangle (img, (x1, y1), (x2, y2), (0, 0, 0), -1, cv2.CV_AA)
    #                    
    #cv2.imwrite ('images/' + str (i).zfill (5) + '.png', img)
    w.save ('states/' + str (i) + '.bin')
    w.load ('states/' + str (i) + '.bin')
