

#include <iostream>

#include "include/ROSRobot.h"

int main()
{
    std::cout << "Hello, World!" << std::endl;
    TCP_ROBOT::ROSRobot *robot = new TCP_ROBOT::ROSRobot();
    robot->show();
    return 0;
}