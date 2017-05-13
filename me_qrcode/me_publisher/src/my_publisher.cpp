#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <zbar.h>

using namespace cv;
using namespace zbar;
using namespace std;

class qr_publisher
{
	ros::NodeHandle nh;   
  	image_transport::ImageTransport it;  
  	image_transport::Publisher pub;
  	cv::VideoCapture  cap; 
public:
	qr_publisher():it(nh),cap(0)
	{
		pub = it.advertise("camera/image", 1);
		videoscanner();
	}
	~qr_publisher(){}

	void videoscanner()
	{
		if(!cap.isOpened())   
  		{  
      		ROS_INFO("can not opencv video device\n");  
      		exit(EXIT_FAILURE);
  		}  

  		// cv::Mat frame;  
  		// sensor_msgs::ImagePtr msg;  
  		ros::Rate loop_rate(5);
  		while(ros::ok())
  		{
  			cv::Mat frame;  
  			sensor_msgs::ImagePtr msg;
  			cap>>frame;

  			if(!frame.empty()) 
  			{  
  				//ROS_INFO("ASDFKJASDK");
      			msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", frame).toImageMsg();  
      			pub.publish(msg);    
    		}
    		
    		loop_rate.sleep();
    		imshow("captured",frame);
    		waitKey(1);
  		} 

	}
	
};

int main(int argc, char** argv)
{
  ros::init(argc, argv, "image_publisher");
  qr_publisher qr_p;
  ros::spin();
}
