#pragma once

#ifndef ROBOTCORE_H_
#define ROBOTCORE_H_

#include "TcpRobotManger_global.h"
#include "SDHRobot.h"
#include "RobotBase.h"

class QMenu;
class QRubberBand;
namespace TCP_ROBOT
{
	class TCPROBOT_EXPORT RobotCore : public RobotBase
	{
		Q_OBJECT
	public:
		// 构造
		RobotCore(QWidget* parent = nullptr);
		~RobotCore();
	};
}
#endif() // ROBOTPREVIEW_H_