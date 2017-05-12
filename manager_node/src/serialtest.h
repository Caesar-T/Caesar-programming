#ifndef SERIALTEST_H
#define SERIALTEST_H
#include "Serial.h"
 #include "ros/console.h"
#include <unistd.h>
 #include "ros/ros.h"
#include <pthread.h>
#include <boost/thread/thread.hpp>
#include <boost/thread/recursive_mutex.hpp>


#define REC_DATA_LEN  512
class SerialTest
{
public:
    SerialTest();
    ~SerialTest();
    int ValueInitOpenSerial(int index);
    void ReadSerialDate();

    int Write();
    int Read();
private:
    Serial *m_serial;
    int m_serial_fd;
    boost::thread *m_rec_threadid;
    boost::mutex m_mutex;
    bool runSerialComThread;
    //pthread_t m_rec_threadid;
    unsigned char m_rec_data[REC_DATA_LEN];
};

#endif // SERIALTEST_H
