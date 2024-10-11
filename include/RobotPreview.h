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

		// 模型更新
		void slotUpdateModel(WORKANDHOLE workAndHole);

		// 单个模型更新
		void slotUpdateSingleModel(WORKANDHOLE workAndHole,QString holeName);

		QMap<QString, ADDROBOTDATA> createRobotData(WORKANDHOLE workAndHole);


	};
}
#endif() // ROBOTPREVIEW_H_