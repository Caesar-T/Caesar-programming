#include "zbar_publisher.h"

int main(int argc, char *argv[])
{
    ros::init(argc,argv,"qr_publisher");
    Publisher_qr p_qr;
    ros::spin();
    return 0;
}
