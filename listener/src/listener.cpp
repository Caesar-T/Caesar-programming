#include "listener.h"

void listener::chatterCallback(const std_msgs::String::ConstPtr& msg)
{
  ROS_INFO("I heard: [%s]", msg->data.c_str());
}

listener::listener()
{
    sub = n.subscribe("chatter", 1000, &listener::chatterCallback,this);
	ros::spin();
}

listener::~listener()
{}


