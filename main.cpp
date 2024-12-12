
#include <iostream>
#include <QApplication>
#include "include/ROSRobot.h"
#include "QCusWidgetLib.h"
using namespace QCUSWIDGETLIB;
int main(int argc, char* argv[])
{
    
    QApplication a(argc, argv);
    CWidgetGridLayout* layout = new CWidgetGridLayout(nullptr);
    std::cout << "Hello, World!" << std::endl;
    TCP_ROBOT::ROSRobot * robot = new TCP_ROBOT::ROSRobot();
    robot->setIpAndPort("127.0.0.1", 54600);
    robot->getCommunicationPointer()->setInterval(100);
    robot->getCommunicationPointer()->setRollInterval(3000);
    layout->addWidget(robot->showTable(), 0, 1);
    layout->addWidget(robot->showTeaching(), 1, 0, 1, 2);
    layout->show();
    robot->showCoreRobot()->show();
    robot->show7103ShapePreview()->show();
    return a.exec();
}