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
		// 根据路径读取替换模型
		void slotReplaceModelByPath(SHAPESTRUCT shapeStruct);
		// 改变预览颜色
		void slotChangPreviewColor(SHAPESTRUCT shapeStruct);
		// 改变预览比例
		void slotChangedPreviewScale(SHAPESTRUCT shapeStruct);
		// 改变预览旋转
		void slotChangedPreviewRotation(SHAPESTRUCT shapeStruct);
		// 改变预览位移
		void slotChangedPreviewTranslation(SHAPESTRUCT shapeStruct);

		// 移除预览
		void removePreview(SHAPESTRUCT shapeStruct);
		// 移除所有预览
		void removeAllPreview();
		// 移除预览机器人数据
		void RemovePreview(ADDROBOTDATA addRobotData);
	public slots:
		// 更新机器人形状
		void slotUpdataRobotShaps(void) override;


	private:
		QMap<QString, ADDROBOTDATA>  m_mapPreviewData = QMap<QString, ADDROBOTDATA>();
	};
}
#endif() // ROBOTPREVIEW_H_