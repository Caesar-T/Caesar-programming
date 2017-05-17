#include <ros/ros.h>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <cstring>
#include "std_msgs/String.h"

using namespace std;
using namespace boost::asio;


#define REV_BUFFER_LEN 128

class Pump_control
{
    ros::NodeHandle nh;
    int PumpLoopRate;
    //ros::NodeHandle private_nh;
    ros::Subscriber sub_cmd;
    ros::Publisher pub_cmd;
    io_service iosev;
    serial_port *pSerialPort;
    string PumpControlComName;
    char cmdBuf[REV_BUFFER_LEN];


public:
    Pump_control()//:private_nh("~")
    {

        //private_nh.param<int>("PumpLoopRate",PumpLoopRate,5);
        PumpLoopRate = 5;
        PumpControlComName = "/dev/ttyUSB0";
        //private_nh.param<string>("PumpControlComName",PumpControlComName,string("/dev/ttyUSB0"));
        pSerialPort = NULL;
        pSerialPort = new serial_port(iosev);

        if(pSerialPort)
            init_SerialPort(PumpControlComName,8);
        ros::Rate loop_rate(PumpLoopRate);
        sub_cmd = nh.subscribe("pump_control",1000,&Pump_control::pumpControlCallBack,this);
        pub_cmd= nh.advertise<std_msgs::String>("pump_read",1000);

        iosev.run();
        loop_rate.sleep();

    }

    ~Pump_control()
    {
        if(pSerialPort)
            delete pSerialPort;
    }


    void write_to_serial(const char *data)
    {
        int len = write( *pSerialPort, buffer(data,strlen(data)));
        ROS_INFO("The data is [%s],the length is [%d]",data,len);
    }

    void handle_read(char buf[], boost::system::error_code ec,std::size_t bytes_transferred)
    {
        ROS_INFO("\nhandle_read: ");
        cout.write(buf, bytes_transferred);
    }


    void read_from_serial()
    {
        char read[10];
        async_read(*pSerialPort, buffer(read), boost::bind( &Pump_control::handle_read, this, read, _1, _2) );
    }




    bool init_SerialPort(const string & port_name,const int & char_size)
    {
        if(!pSerialPort)
        {
            ROS_ERROR("init_pump_port error!");
            return false;
        }
        pSerialPort->open(port_name);
        pSerialPort->set_option(serial_port::baud_rate(9600));
        pSerialPort->set_option(serial_port::flow_control( serial_port::flow_control::none));
        pSerialPort->set_option(serial_port::parity( serial_port::parity::none ));
        pSerialPort->set_option(serial_port::stop_bits( serial_port::stop_bits::one ));
        pSerialPort->set_option(serial_port::character_size( char_size ));

        return true;

    }



    void pumpControlCallBack(const std_msgs::String::ConstPtr& msg)
    {

        if(msg->data.c_str() == string("jamming_pump on"))
        {
        	ROS_INFO(msg->data.c_str());
            memset(cmdBuf,'a',sizeof('a'));
            ROS_INFO("date is :[%s]",cmdBuf);
            write_to_serial(cmdBuf);
            //ROS_INFO("Open jamming_pump ");
        }
        else if(msg->data.c_str() == string("jamming_pump of"))
        {

            memset(cmdBuf,'b',128);
            write_to_serial(cmdBuf);
            //ROS_INFO("Shut down jamming_pump ");
        }
        else if(msg->data.c_str() == string("gripper_pump on"))
        {

            memset(cmdBuf,'c',128);
            write_to_serial(cmdBuf);
            //ROS_INFO("Open gripper_pump ");
        }
        else
        {
            memset(cmdBuf,'d',128);
			ROS_INFO("date is :[%s]",cmdBuf);
            write_to_serial(cmdBuf);
            //ROS_INFO("Shutdown jamming_pump ");
        }
    }


//    void call_handle()
//    {
//        iosev.run();
//    }
};





int main(int argc, char** argv)
{
    ros::init(argc, argv, "pump_write");
    ros::NodeHandle n;

    
    Pump_control pc;
 
    ROS_INFO("ok,let's go");
    ros::spin();
    return 0;
}
