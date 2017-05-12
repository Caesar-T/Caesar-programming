#include "serialtest.h"
#include <pthread.h>
#include <string>
#include <iostream>
using namespace std;

SerialTest::SerialTest()
{
    m_serial = new Serial();

    runSerialComThread = false;
}

SerialTest::~SerialTest()
{

}

void SerialTest::ReadSerialDate()
{
    
    ROS_INFO("Read SerialDateThread Start\n");
    sleep(5);
    ros::Time::init();
    while(runSerialComThread)
    {
        m_mutex.lock();
        Read();
        
        m_mutex.unlock();
        
    }
    ROS_INFO("Receive thread interrupt!\n");
    
}

int SerialTest::ValueInitOpenSerial(int index)
{
    runSerialComThread = true;
    m_serial_fd =  m_serial->OpenPort(index);
    if (m_serial_fd < 0)
    {
          ROS_ERROR("Open serial port %d failed!\n",index);
         return -1;
     }
    else
    {
        ROS_INFO("Open serial port %d Success\n",m_serial_fd);
    }

    m_serial->SetPara(m_serial_fd,6);
    m_rec_threadid = new boost::thread(boost::bind(&SerialTest::ReadSerialDate,this));
    if(m_rec_threadid == NULL)
    {
      ROS_ERROR("creat Receive_Thread failed \n");
      m_serial->ClosePort(m_serial_fd);
      m_serial_fd = -1;
      m_rec_threadid->join();
      runSerialComThread = false;
      delete m_rec_threadid;
      return -1;
   }
  return 0;
}

int SerialTest::Write(const char * str)
{
    
    //data[0] = 0x41;
    //data[1] = 0x54;
    //data[2] = 0x0D;
    //data[3] = 0x0A;
	cout<<str<<endl;
    if(m_serial->WriteData(m_serial_fd,str,strlen(str)) < 0)
    {
           ROS_ERROR("Write Data Fail!\n");
           return -1;
    }
    return 0;
}

int SerialTest::Read()
{
      if( m_serial->ReadData(m_serial_fd,m_rec_data,REC_DATA_LEN) > 0)
    {
      ROS_INFO("REC Data %s\n",m_rec_data);
      return 0;
     }
      return -1;
}
