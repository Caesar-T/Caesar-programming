#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <zbar.h>
#include <std_msgs/String.h>
#include <iostream>
#include <iomanip>

using namespace std;
using namespace cv;
using namespace zbar;


class qr_subscriber
{
    ros::NodeHandle nh;    
    image_transport::ImageTransport it;  
    image_transport::Subscriber sub;
    ros::Publisher qr_pub_;
    ImageScanner scanner;
public:
  qr_subscriber():it(nh)
  {
  	  //ROS_INFO("I am construtor");
      sub = it.subscribe("camera/image",1,&qr_subscriber::imageCallback,this);

      qr_pub_ = nh.advertise<std_msgs::String>("qr_code", 10);
      scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);
  }
  ~qr_subscriber()
  {
  		//ROS_INFO("bye bye");
  }


  void imageCallback(const sensor_msgs::ImageConstPtr& msg)
  {
  		//ROS_INFO("I am coming!");
        cv_bridge::CvImagePtr cv_ptr;
        cv::Mat frame_grayscale;
        std_msgs::String text;
        //every 0.5 sec deal 1 image
        static ros::Time last_image = ros::Time();
        if ((ros::Time::now()).toSec() - last_image.toSec() < 0.5) 
            return;
        last_image = ros::Time::now();
        // convert ros image to mat
        try
        {
        	//ROS_INFO("transform successfully!");
            cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
        }
        catch (cv_bridge::Exception& e)
        {
            ROS_ERROR("cv_bridge exception: %s", e.what());
            return;
        }
        // Convert to grayscale
        cvtColor(cv_ptr->image, frame_grayscale, CV_BGR2GRAY);
        // Obtain image data
        int width = frame_grayscale.cols;
        int height = frame_grayscale.rows;
        uchar *raw = (uchar *)(frame_grayscale.data);
        // Wrap image data
        Image image(width, height, "Y800", raw, width * height);
        // Scan the image for barcodes
        scanner.scan(image);
        // Extract results
        int counter = 0;
        for (Image::SymbolIterator symbol = image.symbol_begin(); symbol != image.symbol_end(); ++symbol)
        {
            ROS_INFO("decoded: %s, count: %d, symbol: %s", (symbol->get_type_name()).c_str(), counter, (symbol->get_data()).c_str());
            text.data = symbol->get_data();
            static string last_text = "";
            static ros::Time last_time = ros::Time::now();
            //publish
            if(text.data != "") 
            {
            	//ROS_INFO("text is : [%s]",text.data.c_str());
                qr_pub_.publish(text);
                if (last_text != text.data || (ros::Time::now()).toSec() - last_time.toSec() > 10) 
                {
                    
                    last_text = text.data;
                    last_time = ros::Time::now();
                }
            }
            counter++;
        }
    }
  
};





int main(int argc, char **argv)
{
    ros::init(argc, argv, "image_listener");
    qr_subscriber qr_s;
    ros::spin();
}
