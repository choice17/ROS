Motion Detect

*modified version, to check motion in front of camera

The purpose of this node is to demonstrate how to retrieve images in ROS and manipulate them using opencv

Installation
- install cv_camera package
  - 'sudo apt install ros-kinetic-usb-cam'
- clone this package into the src folder of your workspace
- Run 'catkin_make' in the root of your workspace

Running the example
- Run 'source devel/setup.bash' in the root of your workspace
- Run 'roslaunch motiondetect motiondetect.launch'
