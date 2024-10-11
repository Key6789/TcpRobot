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

		// QWidget 用于显示预览
		QWidget* showPreview(QWidget* parent = nullptr);

		// QWidget 用于显示工作和焊缝的预览
		QWidget* showWorkAndHolePreview(QWidget* parent = nullptr);

		// QWidget 用于显示焊缝的预览
		QWidget* showHolePreview(QWidget* parent = nullptr);

		// QWidget 用于显示特定焊缝的右侧预览
		QWidget* showHoleRightPreview(QString holeName, QWidget* parent = nullptr);

		// 创建工作和焊缝的数据结构
		WORKANDHOLE createWorkAndHole(QMap<QString, HoleDATAStruct> holeMap);

	signals:
		// 选中的工作名称变化信号
		void seletedWorkChanged(QString workName);

	private:
		// 焊缝数据结构映射
		QMap<QString, HoleDATAStruct> m_holeMap = QMap<QString, HoleDATAStruct>();

		// 右侧视图指针
		QWidget* m_rightView = nullptr;

		// 右侧工作视图指针
		QWidget* m_rightWorkView = nullptr;

		// 焊缝数据的复制
		HoleDATAStruct m_holeDataCopy = HoleDATAStruct();

		// 机器人预览指针
		RobotPreview* m_robotPreview = nullptr;

		// 焊缝预览指针
		QWidget* m_holePreview = nullptr;

		// 当前工作名称
		QString m_currentWorkName = QString();

	};
}

#endif // !
