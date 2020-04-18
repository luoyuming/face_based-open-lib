import tface as tf
import cv2
import numpy as np
#import matplotlib.pyplot as plt
import os
import sys
import time

#print('-'*50)
#print(os.path.abspath(sys.argv[0]))
#print(os.path.abspath(__file__))
print('-'*50)
init_ok = tf.face_init()
if init_ok == 0:
   print("init is ok")
else:
   print("init is fault errno=%d" % init_ok) 

image_rgb = cv2.imread("./face_image/luo.jpg")
tf.face_input_image(image_rgb);
print('-'*50)
while True:
  check_person = tf.face_check_person()
  if check_person:
     print("person to check")
     ret_json = tf.face_get_face_info()
     print("result :%s" % ret_json) 

     break
  #else:
     #print("person not to check")
  time.sleep(1)





