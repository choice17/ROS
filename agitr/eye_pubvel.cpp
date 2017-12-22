// This program publishes randomly-generated velocity
// messages for turtlesim.
#include <ros/ros.h>
#include <geometry_msgs/Twist.h>  // For geometry_msgs::Twist
#include <stdlib.h> // For rand() and RAND_MAX
#include "std_msgs/String.h"
#include <iostream>

static ros::Publisher *pubStr;

static geometry_msgs::Twist msg;

static geometry_msgs::Twist msg_init()
{
    		geometry_msgs::Twist this_msg;
    		// linear speed to [1,3]
    		this_msg.linear.x = 0.5;
    		//msg.linear.x = 2*double(rand())/double(RAND_MAX)+1;
    		// right turn
    		// right turn
    		this_msg.angular.z = 0;

    		// Send a message to rosout with the details.
    		//ROS_INFO_STREAM("turning left"
      		//<< " linear=" << this_msg.linear.x
      		//<< " angular=" << this_msg.angular.z);

		//return 
		return this_msg;
}

void callback2(const std_msgs::String::ConstPtr& data)
{

	std::string thismsg = data->data.c_str();
	//ROS_INFO_STREAM(data->data.c_str()<<thismsg.compare("motion_detected"));
	
	if (thismsg.compare("motion_detected")==0)
	{
			   

	//ROS_INFO(data->data.c_str());
            	// linear speed to [1,3]
    		msg.linear.x = 2;
    		//msg.linear.x = 2*double(rand())/double(RAND_MAX)+1;
    		// right turn
    		// left turn
    		msg.angular.z = 1.2;

    		// Publish the message.

    		// Send a message to rosout with the details.
    		//ROS_INFO_STREAM("turning left"
      		//<< " linear=" << msg.linear.x
      		//<< " angular=" << msg.angular.z);
	}else if (thismsg.compare("no_motion")==0)
	{
		msg = msg_init();
	//	ROS_INFO_STREAM("going straight");
			
	}

	pubStr->publish(msg);
    	ROS_INFO_STREAM("vel message"
      	<< " linear=" << msg.linear.x
      	<< " angular=" << msg.angular.z);
	
}

void callback(const std_msgs::String::ConstPtr& data)
{

	std::string::size_type sz;
	std::string this_msg = data->data.c_str();
	ROS_INFO_STREAM("received msg:"<<this_msg);	
	if (this_msg.compare("left_blink")==0)
	{
    		msg.linear.x = 4;
    		msg.angular.z = 2;

	}else if (this_msg.compare("right_blink")==0)
	{
    		msg.linear.x = 4;
    		msg.angular.z = -2;

		msg = msg_init();
	}else if (this_msg.compare("all_open")==0)
	{
    		msg.linear.x = 2;
    		msg.angular.z = 0;

	}else if (this_msg.compare("all_close")==0)
	{
    		msg.linear.x = 3;
    		msg.angular.z = -3;

	}else if (this_msg.compare("mouth_open")==0)
	{
		msg.linear.x = 3;
		msg.angular.z = 3;
	}
	pubStr->publish(msg);

       ROS_INFO_STREAM("Sending random velocity command:"
      	<< " linear=" << msg.linear.x
      	<< " angular=" << msg.angular.z);


}
int main(int argc, char **argv) {
  // Initialize the ROS system and become a node.
  ros::init(argc, argv, "publish_velocity");
  ros::NodeHandle nh;

  pubStr = new ros::Publisher(nh.advertise<geometry_msgs::Twist>(
    "turtle1/cmd_vel", 1000));
  ros::Subscriber sub2 = nh.subscribe("motion_detect/detected",1000,callback2);
  ros::Subscriber sub = nh.subscribe("facial_landmarks/eye_dist",1000,callback);



  //msg = msg_init();


  // Loop at 2Hz until the node is shut down.
 // ros::Rate rate(2);
 // while(ros::ok()) {
    // Create and fill in the message.  The other four
    // fields, which are ignored by turtlesim, default to 0.

    // Publish the message.
    //pubStr->publish(msg);

    // Send a message to rosout with the details.
    //ROS_INFO_STREAM("Sending random velocity command:"
    //  << " linear=" << msg.linear.x
    //  << " angular=" << msg.angular.z);

    // Wait until it's time for another iteration.
    //rate.sleep();
   ros::spin(); 
//}
  //delete pubStr;
}
