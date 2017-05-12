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
    Publisher_qr()
    {
        qr_pub = nh.advertise<std_msgs::String>("connection", 1000); 
        camerascanner();
    }
    ~Publisher_qr()
    {}



    void  camerascanner()
    {
        VideoCapture cap(0);
        if (!cap.isOpened()) 
        {
            ROS_INFO("Could not open camera.");
            exit(EXIT_FAILURE);
        }
        namedWindow("captured", CV_WINDOW_AUTOSIZE);
        
        //sensor_msgs::ImagePtr msg;
         
        ros::Rate loop_rate(1);
        int counter = 0;
        while(ros::ok())
        {
            cv::Mat frame, frame_grayscale;
            cap >> frame;  
            // Convert to grayscale
            cvtColor(frame, frame_grayscale, CV_BGR2GRAY);

            // Obtain image data
            int width = frame_grayscale.cols;
            int height = frame_grayscale.rows;
            uchar *raw = (uchar *)(frame_grayscale.data);

            // Wrap image data
            Image image(width, height, "Y800", raw, width * height);

            // Scan the image for barcodes
            //int n = scanner.scan(image);
            scanner.scan(image);  
            // Configure the reader

            scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1); 
            std_msgs::String text;
            
            for(Image::SymbolIterator symbol = image.symbol_begin(); symbol != image.symbol_end(); ++symbol) 
            {

                ROS_INFO("decoded: %s, count: %d, symbol: %s", (symbol->get_type_name()).c_str(), counter, (symbol->get_data()).c_str());
            
                // Draw location of the symbols found
                if (symbol->get_location_size() == 4) 
                {
                    //rectangle(frame, Rect(symbol->get_location_x(i), symbol->get_location_y(i), 10, 10), Scalar(0, 255, 0));
                    line(frame, Point(symbol->get_location_x(0), symbol->get_location_y(0)), Point(symbol->get_location_x(1), symbol->get_location_y(1)), Scalar(0, 255, 0), 2, 8, 0);
                    line(frame, Point(symbol->get_location_x(1), symbol->get_location_y(1)), Point(symbol->get_location_x(2), symbol->get_location_y(2)), Scalar(0, 255, 0), 2, 8, 0);
                    line(frame, Point(symbol->get_location_x(2), symbol->get_location_y(2)), Point(symbol->get_location_x(3), symbol->get_location_y(3)), Scalar(0, 255, 0), 2, 8, 0);
                    line(frame, Point(symbol->get_location_x(3), symbol->get_location_y(3)), Point(symbol->get_location_x(0), symbol->get_location_y(0)), Scalar(0, 255, 0), 2, 8, 0);
                }

                
                text.data = symbol->get_data().c_str();
                static string last_text = "";
                static ros::Time last_time = ros::Time::now();
                //publish
                if (text.data != "") 
                {
                    qr_pub.publish(text);
                    if (last_text != text.data || (ros::Time::now()).toSec() - last_time.toSec() > 10) 
                    {
                        last_text = text.data;
                        last_time = ros::Time::now();
                    }
                }
                counter++;
            }
            imshow("captured",frame);
            // clean up
            image.set_data(NULL, 0);
        }

    }
      
}; 



int main(int argc, char *argv[])
{
    ros::init(argc,argv,"qr_publisher");
    Publisher_qr p_qr;
    ros::spin();
    return 0;
}