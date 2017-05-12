#include "serialtest.h"
#include <ros/ros.h>

int main(int argc,char **argv)
{
    ros::init(argc,argv,"serialcom");
    ROS_INFO("start serialcom...");
     SerialTest serail_test;

     if(serail_test.ValueInitOpenSerial(ttyUSB0) < 0)
     {
     return -1;
     }

     while(true)
     {
      ROS_INFO("from here to write data to serialcom");
      serail_test.Write();
      setbuf(stdin,NULL);
      getchar();
      usleep(100000);
     }
     ros::spin();
     return 0;
}
