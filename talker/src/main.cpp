#include <iostream>
#include "ros/ros.h"
#include "talker.h"
using namespace std;

int main(int argc,char **argv)
{
    ROS_INFO("start to talk...");

    ros::init(argc, argv, "we_laser_detect");
    talker tk;

    ros::spinOnce();
    return 0;
}
