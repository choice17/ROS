# ROS system

**RPI3** **ROS** **OpenCV** **project** 

## motion detection

```python
git clone <project.git>
```
and back to master workspace folder and run 

`catkin_make` 

To launch the ROS node 

```python
rosrun cv_camera cv_camera_node
rosrun motion_detect motion_detect_node.py
```

## facial landmarks detection 

It is able to run in raspberry pi 3 with smooth 10fps

Here is the node connection of the project

![alt text](https://github.com/choice17/ROS/blob/master/image_/rqt_graph.png)

to run this node
```python
git clone <project.git>
```

and back to master workspace folder and run 
`catkin_make`

`roscore`

```python
rosrun cv_camera cv_camera_node
rosrun facial_landmarks facial_landmarks_node.py
rosrun agitr eye_pubvel
rosrun turtlesim turtlesim_node
```

**Note** to use this project, you must install dlib into rpi3, 
please lookinto [dlib website](http://dlib.net/) for installation 

And also `git clone` the necessary dependency node in [ROS-WIKI](http://wiki.ros.org/)

[image_common](http://wiki.ros.org/image_common)
[cv_camera](http://wiki.ros.org/cv_camera)


[Video demo](https://www.youtube.com/watch?v=gxUL3YSi55M&feature=youtu.be)
