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

		// 根据路径读取替换模型
		void slotReplaceModelByPath(SHAPESTRUCT shapeStruct);
		void slotChangPreviewColor(SHAPESTRUCT shapeStruct);
		void slotChangedPreviewScale(SHAPESTRUCT shapeStruct);
		void slotChangedPreviewRotation(SHAPESTRUCT shapeStruct);
		void slotChangedPreviewTranslation(SHAPESTRUCT shapeStruct);

		void removePreview(SHAPESTRUCT shapeStruct);
		void removeAllPreview();

		// 单个模型更新
		void slotUpdateSingleModel(WORKANDHOLE workAndHole,QString holeName);

		ADDROBOTDATA createRobotData(SHAPESTRUCT shapeStruct);

		void displaySingalAddRobot(ADDROBOTDATA addRobotData);
		void RemovePreview(ADDROBOTDATA addRobotData);
	private:
		


		QMap<QString, ADDROBOTDATA>  m_mapPreviewData = QMap<QString, ADDROBOTDATA>();


	};
}
#endif() // ROBOTPREVIEW_H_