#include "talker.h"

talker::talker():rate_loop(10)
{

    pub = n.advertise<std_msgs::String>("chatter",1000);
    say();

}

talker::~talker()
{}

void talker::say()
{
    int count = 1;
    while(ros::ok())
    {
        std_msgs::String msg;

        std::stringstream ss;
        ss << "hello world " << count;
        msg.data = ss.str();

        ROS_INFO("%s", msg.data.c_str());
        pub.publish(msg);
        rate_loop.sleep();
        count++;
    }

}
