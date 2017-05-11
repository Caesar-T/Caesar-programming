#ifndef TALKER_H
#define TALKER_H
#include "ros/ros.h"
#include "std_msgs/String.h"
#include <string>
#include <iostream>
#include <sstream>
using namespace std;


class talker
{
private :
    ros::NodeHandle n;
    ros::Rate rate_loop;
    ros::Publisher pub;

public:
    talker();
    ~talker();
    void say();

};


#endif
