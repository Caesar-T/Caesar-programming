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


class Publisher_qr
{
    ros::NodeHandle nh;
    //image_transport::ImageTransport it_;
    //image_transport::Publisher qr_pub;
    ros::Publisher qr_pub;
    ImageScanner scanner;
    
public:
    Publisher_qr();
    ~Publisher_qr();



    void  camerascanner(); 
}; 
