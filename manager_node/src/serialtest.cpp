#include "serialtest.h"
#include <pthread.h>

SerialTest::SerialTest()
{
    m_serial = new Serial();
}

SerialTest::~SerialTest()
{

}

void* ReadSerialDate(void *para)
{
    SerialTest *p_serial_test = (SerialTest*)para;
    printf("ReadSerialDate Thread Start\n");
    while(1)
    {
         p_serial_test->Read();
         usleep(10000);
    }
    printf("Receive thread break!\n");
    pthread_detach(pthread_self());
    pthread_exit(0);
    return 0;
}

int SerialTest::ValueInitOpenSerial(int index)
{
    m_serial_fd =  m_serial->OpenPort(index);
    if (m_serial_fd < 0)
    {
          printf("Open serial port %d failed!\n",index);
         return -1;
     }
    else
    {
        printf("Open serial port %d Success\n",m_serial_fd);
    }

    m_serial->SetPara(m_serial_fd,6);

  if(pthread_create(&m_rec_threadid, NULL,ReadSerialDate,this) != 0)
  {
      printf("creat Receive_Thread failed \n");
      m_serial->ClosePort(m_serial_fd);
      m_serial_fd = -1;
      return -1;
   }
  return 0;
}

int SerialTest::Write()
{
    char data[10]= {0};
    data[0] = 0x41;
    data[1] = 0x54;
    data[2] = 0x0D;
    data[3] = 0x0A;
    if(m_serial->WriteData(m_serial_fd,data,strlen(data)) < 0)
    {
           printf("Write Data Fail!\n");
           return -1;
    }
    return 0;
}

int SerialTest::Read()
{
      if( m_serial->ReadData(m_serial_fd,m_rec_data,REC_DATA_LEN) > 0)
    {
      printf("REC Data %s\n",m_rec_data);
      return 0;
     }
      return -1;
}
