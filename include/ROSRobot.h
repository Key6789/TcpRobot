#pragma once

#ifndef ROSROBOT_H
#define ROSROBOT_H
#include "TcpRobotManger_global.h"
#include "SDHRobot.h"
#include "RobotBase.h"
#include "RobotPreview.h"
#include "RobotCore.h"

class QMenu;
class QRubberBand;
namespace TCP_ROBOT
{
	class TCPROBOT_EXPORT ROSRobot : public QWidget
	{
		Q_OBJECT
	public:
		// 构造
		ROSRobot(QWidget* parent = nullptr);
		~ROSRobot();

		QWidget * showPreview(QWidget* parent = nullptr);
		QWidget * showWorkAndHolePreview(QWidget* parent = nullptr);
	};
}

#endif // !
