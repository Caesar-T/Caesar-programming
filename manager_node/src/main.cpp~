#include "serialtest.h"
#include <ros/ros.h>
#include <string>
#include <iostream>
using namespace std;

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
	char data[20] = {0};
	cin>>data;
      serail_test.Write(data);
      setbuf(stdin,NULL);
      getchar();
      //usleep(100000);
     }
     ros::spin();
     return 0;
}
