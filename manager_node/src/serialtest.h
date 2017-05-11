#ifndef SERIALTEST_H
#define SERIALTEST_H
#include "Serial.h"

#define REC_DATA_LEN  512
class SerialTest
{
public:
    SerialTest();
    ~SerialTest();
    int ValueInitOpenSerial(int index);

    int Write();
    int Read();
private:
    Serial *m_serial;
    int m_serial_fd;
    pthread_t m_rec_threadid;
    unsigned char m_rec_data[REC_DATA_LEN];
};

#endif // SERIALTEST_H
