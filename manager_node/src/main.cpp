#include "serialtest.h"

int main(int argc,char **argv)
{
     SerialTest serail_test;

     if(serail_test.ValueInitOpenSerial(ttyUSB0) < 0)
     {
     return -1;
     }

     while(1)
     {
      serail_test.Write();
      setbuf(stdin,NULL);
      getchar();
      usleep(100000);
     }

     return 0;
}
