#include "ros/ros.h"
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include "std_msgs/String.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>


cv_bridge::CvImagePtr cv_ptr;
cv::Mat image;
cv::Mat dstImg;
//cv::CascadeClassifier face_cascade ;
//std::string face_name = "haarcascade_frontalface_default.xml";
//if (!face_cascade.load(face_name)) { printf("--(!) Error loading\n"));
const int detect_thresh = 1000;
const std::string motion_detected = "motion_detected";
const std::string no_motion = "no_motion";
ros::Publisher *pubStr;

void imageROI(cv::Mat inImg,cv::Mat dstImg)
{
	cv::Mat mask = cv::Mat::zeros(inImg.size(), inImg.type());
	
	// define ROI 
        cv::circle(mask, cv::Point(mask.cols/2, mask.rows/2),mask.rows/3, 
		cv::Scalar(255,0,0), -1, 8, 0);
	
	// masking
	inImg.copyTo(dstImg,mask);
}

float imageDist(cv::Mat thisImg, cv::Mat lastImg)
{
	cv::Mat diffImg = thisImg.clone();
	float diff = 0;
	int xIdx = thisImg.cols/2;
	int yIdx = thisImg.rows/2;
	
	cv::absdiff(thisImg,lastImg,diffImg);
	cv::threshold(diffImg,diffImg,80,255, cv::THRESH_BINARY);
	diff = cv::sum(diffImg)[0];
	//std::cout<<"difference : "<<diff<<std::endl;
	diff = diff/(255);

	//std::cout<<"difference : "<<diff<<std::endl;
	return diff;

}	



void publishMotionDetect(const sensor_msgs::ImageConstPtr& rosImage)
{
  cv_ptr = cv_bridge::toCvCopy(rosImage, sensor_msgs::image_encodings::BGR8);
  image = cv_ptr->image;  
  std_msgs::String msg;
  
  if(!image.empty()){
    cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
    
    dstImg = cv::Mat::zeros(image.size(), image.type());

    // masking
    imageROI(image,dstImg);  
    cv::imshow("win", dstImg);
    
    
    // difference between image
    static cv::Mat last_image = dstImg.clone(); 
    
    float imgDist = imageDist(dstImg,last_image);
     

    last_image = dstImg.clone();
    
    //send msg
    if (imgDist>detect_thresh)
    {
	msg.data = motion_detected;
	pubStr->publish(msg);
	ROS_INFO_STREAM(msg);

    }else{
	    msg.data = no_motion;
	    pubStr->publish(msg);
    	   // ROS_INFO_STREAM(no_motion);	
    }

    cv::waitKey(1);
  }
  else{
    std::cout << "error loading image, are you running cv_camera?" << std::endl;
  }

  
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "motion_detect_node");
  ros::NodeHandle n;
  image_transport::ImageTransport it(n);
  image_transport::Subscriber image_sub_;
  pubStr = new ros::Publisher( n.advertise<std_msgs::String>("motion_detect/detected",10));
   
  image_sub_ = it.subscribe("/cv_camera/image_raw", 1, publishMotionDetect);

    
//  if (msg.compare(motion_detected)==1)
//  {
//  	pub.publish(msg);
//  }
  ros::spin();
  
  return 0;
}

