
#include <iostream>
#include <QApplication>
#include "include/ROSRobot.h"
int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    std::cout << "Hello, World!" << std::endl;
    TCP_ROBOT::ROSRobot * robot = new TCP_ROBOT::ROSRobot();
    robot->show7103ShapePreview()->show();
    return a.exec();
}