#!/usr/bin/python
import roslib
#roslib.load_manifest('facial_landmarks')
import sys
import rospy
import cv2
import numpy as np
from std_msgs.msg import String 
#from facial_landmarks.msg import FloatPair
from sensor_msgs.msg import Image
from cv_bridge import CvBridge, CvBridgeError
#import datetime
from imutils import face_utils
import imutils
import dlib
import os
#from matplotlib import pyplot as plt
#from __future__ import print_function

packageName = '/src/facial_landmarks/src/'
face_cascade_name = "/home/pi/ros_catkin_ws/src/opencv3/data/haarcascades/haarcascade_frontalface_default.xml"

class image_worker:

    def __init__(self):
        #self.left_pub = rospy.Publisher("/facial_landmarks/left_eye_bdist",String,queue_size=1)
        #self.right_pub = rospy.Publisher("/facial_landmarks/right_eye_bdist",String,queue_size=1)
        self.eye_dist_pub = rospy.Publisher("/facial_landmarks/eye_dist",String,queue_size=10)
        self.bridge = CvBridge()
        self.image_sub = rospy.Subscriber("/cv_camera/image_raw",Image,self.callback)
        #self.detector = dlib.get_frontal_face_detector():
        self.detector = cv2.CascadeClassifier()
        if (self.detector.load(face_cascade_name)==False):
            print('!error loading file from '+face_cascade_name)      
        else:
            self.detector.load(face_cascade_name)
            print('use file from '+face_cascade_name)
            print('check if face detector is empty :',self.detector.empty())
        fileName = "shape_predictor_68_face_landmarks.dat"
        filepath = os.getcwd()+packageName
        
        try:
            print('used file from '+filepath+fileName)
            self.predictor = dlib.shape_predictor(filepath+fileName)
        #self.predictor = dlib.shape_predictor("shape_predictor_68_face_landmark.dat")
        except RuntimeError as e:
            print(e)



    def callback(self,data):
        
        cv_image = self.bridge.imgmsg_to_cv2(data, "bgr8")

                     # detect eye pos
                   
        cv_image = imutils.resize(cv_image,height=300,width=400)
       
       

       
        gray = cv2.cvtColor(cv_image, cv2.COLOR_BGR2GRAY)

            #self.image_pub.publish(self.bridge.cv2_to_imgmsg(gray,"mono8"))
         
        eyedist, cv_image = self.detect_eye(gray,cv_image) 
            
        cv2.imshow("image",cv_image)
        cv2.waitKey(1)

        '''
        try:
                cv2.imshow("facial_window",cv_image )
                #cv2.waitKey(3)
            #self.image_pub.publish(self.bridge.cv2_to_imgmsg(cv_image,"bgr8"))
                self.image_pub.publish(self.bridge.cv2_to_imgmsg(cv_image,"bgr8"))

            except:
                print(eyedist,rospy.get_time()) 
        '''
        #except CvBridgeError as e:
        #    print(e)
            
    def detect_eye(self,gray,cv_image):
        
        this_eye = np.zeros((12,2))
        this_eye_dist = np.zeros((1,2))   
        frame = cv_image.copy()
        this_mouth = np.zeros((4,2))
        this_mouth_dist = 0

        try: 
            #rects = self.detector.(gray,0)
            rects = self.detector.detectMultiScale(gray, scaleFactor=1.1,
                        minNeighbors=5, minSize = (30,30),
                        flags=cv2.CASCADE_SCALE_IMAGE)
            for (x,y,w,h) in rects:
                rect = dlib.rectangle(int(x),int(y),int(x+w),int(y+h))
                shape = self.predictor(gray,rect)
                shape = face_utils.shape_to_np(shape)
                #print(rect)
                point_idx = 0
                mouth_idx = 0
                for (x, y) in shape:
                    cv2.circle(frame, (x, y), 1, (0,0, 255), -1)

                    if (point_idx >=36 and point_idx <=47):
                        writer_idx = point_idx-36
                        this_eye[writer_idx,] = [x,y]
                
                    if (point_idx ==61 or point_idx==63 or point_idx==65
                            or point_idx==67):
                        this_mouth[mouth_idx,] = [x,y]
                        mouth_idx += 1

                    point_idx +=1

            this_eye_dist = get_eye_dist(this_eye).astype(float)
            this_mouth_dist = get_mouth_dist(this_mouth).astype(float)        
            #cv2.imshow("Image",frame)
            print('distance of twoeye :',this_eye_dist)
            print('distance of mouth :',this_mouth_dist)
            
       #     print('type is of ',this_eye_dist[0,0],' : ', type(this_eye_dist[0,0]))
       #     print('type is of ',str(this_eye_dist[0]),' : ', type(str(this_eye_dist[0])))
                    
            pub_msg = ""       
            #self.left_pub.publish(str(this_eye_dist[0,0])))   
            #self.right_pub.publish(str(this_eye_dist[0,1]))
            #if ((this_eye_dist[0,0]<=0.03) and (this_eye_dist[0,1]>0.03)):
                        
                        
            #    pub_msg = "right_blink"
         
            #elif ((this_eye_dist[0,0]>=0.03) and (this_eye_dist[0,1]<0.03)):
                        
            #    pub_msg = "left_blink"

            if ((this_eye_dist[0,0]>=0.03) and (this_eye_dist[0,1]>=0.03)):
            
                pub_msg = "all_open"

            elif ((this_eye_dist[0,0]<0.03) and (this_eye_dist[0,1]<0.03)):
                        
                pub_msg = "all_close"
            
            if (this_mouth_dist>0.95):
                pub_msg = "mouth_open"

            
            self.eye_dist_pub.publish(pub_msg)
            print(pub_msg)

            return this_eye_dist,frame

         #       print('dead at two')
         #       return this_eye_dist,[]
        except RuntimeError as e:
            print('error in  detection')
            print(e)
            return this_eye_dist,cv_image
            


def dist(point_1,point_2):
    return np.sum((point_1-point_2)**2)

def get_eye_dist(eye_coord):
    pts = eye_coord
    right_eye = (dist(pts[1,:],pts[5,:])+dist(pts[2,:],pts[4,:]))/(
                2*dist(pts[0,:],pts[3,:]))
    left_eye = (dist(pts[7,:],pts[11,:])+dist(pts[8,:],pts[10,:]))/(
                2*dist(pts[6,:],pts[9,:]))
    pts_2 = np.array([[right_eye,left_eye]])
    return pts_2

def get_mouth_dist(mouth_coord):
    pts = mouth_coord
    pts_1 = (dist(pts[1,:],pts[3,]))/(dist(pts[0,:],pts[2,:]))
    
    return pts_1

def main(args):
    ic = image_worker()
    #rate = rospy.Rate(1) 
    rospy.init_node('facial_landmarks_node',anonymous=True)
    #while not rospy.is_shutdown(): 
        
    #    rate.sleep()
    try:
        rospy.spin()
    except KeyboardInterrupt:
        print("Shutting down")
        cv2.destroyAllWindows()

if __name__ == '__main__':
    main(sys.argv)
       
