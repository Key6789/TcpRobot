#pragma once

#ifndef ROBOTPREVIEW_H_
#define ROBOTPREVIEW_H_

#include "TcpRobotManger_global.h"
#include "SDHRobot.h"
#include "RobotBase.h"

class QMenu;
class QRubberBand;
namespace TCP_ROBOT
{
	class TCPROBOT_EXPORT RobotPreview : public RobotBase
	{
		Q_OBJECT
	public:
		// 构造
		RobotPreview(QWidget* parent = nullptr);
		~RobotPreview();

	public slots:
		void slotUpdataRobotShaps(void) override;

	};
}
#endif() // ROBOTPREVIEW_H_