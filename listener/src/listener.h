#ifndef LISTENER_H
#define LISTENER_H
#include "ros/ros.h"
#include "std_msgs/String.h"
#include <string>
#include <iostream>
#include <sstream>
using namespace std;


class listener
{
private :
    ros::NodeHandle n;
    //ros::Rate rate_loop;
    ros::Subscriber sub;

public:
    listener();
    ~listener();
    //void say();
    void chatterCallback(const std_msgs::String::ConstPtr& msg);

};


#endif
