*ROS system

** RPI3 ROS OpenCV project 

* motion detection

```bash
git clone project.git
```
and back to master workspace folder and run
'catkin_make'


* facial landmarks detection 

It is able to run in raspberry pi 3 with smooth 10fps
![alt text](https://github.com/choice17/ROS/blob/master/image_/rqt_graph.png)

to run this node
```bash
git clone project.git
```

and back to master workspace folder and run
'catkin_make

`roscore`

```bash
rosrun cv_camera cv_camera_node
rosrun facial_landmarks facial_landmarks_node.py
rosrun agitr eye_pubvel
rosrun turtlesim turtlesim_node
```
