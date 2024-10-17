﻿#pragma once

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
	class ShapeCommondPreview;
	class TCPROBOT_EXPORT ROSRobot : public QWidget
	{
		Q_OBJECT
	public:
		// 构造
		ROSRobot(QWidget* parent = nullptr);
		~ROSRobot();

		// QWidget 用于显示预览
		QWidget* showPreview(QWidget* parent = nullptr);
		QWidget* showRobotPreview(QWidget* parent = nullptr);

	signals:
		// 选中的工作名称变化信号
		void seletedWorkChanged(QString workName);
	private: // tool
		// 创建目录
		void createOrCheckDirectory(const QString& path);

		// 删除目录
		bool removeDirectory(const QString& path);

		// 获取子目录
		QStringList getSubDirectories(const QString& path);

		bool saveToJsonFile(const QVariantMap& data, const QString& filePath);


	private:
		QMap<QString, SHAPESTRUCT> m_shapeMap = QMap<QString, SHAPESTRUCT>();
		QMap<QString, SHAPESTRUCT> m_robotMap = QMap<QString, SHAPESTRUCT>();
	};

	class TCPROBOT_EXPORT ShapeCommondPreview : public QWidget
	{
		Q_OBJECT
	public:
		ShapeCommondPreview(QWidget* parent = nullptr);
		~ShapeCommondPreview();

		void setRobotPreviewPoint(RobotPreview* robotPreview );

		// 设置形状结构体
		void setShapeStruct(SHAPESTRUCT shapeStruct);
		// 获取形状结构体
		SHAPESTRUCT getShapeStruct() { return m_shapeStruct; };

		// 设置形状类型
		void setShapeType(ShapeType shapeType);

		// 初始化连接
		void initConnect();

		void setLinkIsVisable(bool isVisable);

	public slots:
		// 读取形状路径
		void readShapePath();
		// 读取形状颜色
		void readShapeColor();
		// 读取形状缩放
		void readShapeScale();
		// 读取形状X轴角度
		void readShapeAngleX();
		// 读取形状Y轴角度
		void readShapeAngleY();
		// 读取形状Z轴角度
		void readShapeAngleZ();
		// 读取形状X轴位置
		void readShapePosX();
		// 读取形状Y轴位置
		void readShapePosY();
		// 读取形状Z轴位置
		void readShapePosZ();
		// 读取形状名称
		void readShapeName();

		void addLink();
		void deleteLink();
		void checkChangeLink();
		void linkListItemChanged();

		void readLinkName();
		void readLinkAlpha();
		void readLinkTheta();
		void readLinkDDistance();
		void readLinkADistance();
		void readLinkIndex();

	private:
		// 形状结构体
		SHAPESTRUCT m_shapeStruct;

		// 形状名称输入框
		QLineEdit* m_shapeName = nullptr;
		// 形状颜色输入框
		QLineEdit* m_shapeColor = nullptr;
		// 形状路径输入框
		QLineEdit* m_shapePath = nullptr;
		// 形状缩放输入框
		QLineEdit* m_shapeScale = nullptr;
		// 形状X轴角度输入框
		QLineEdit* m_shapeAngleX = nullptr;
		// 形状Y轴角度输入框
		QLineEdit* m_shapeAngleY = nullptr;
		// 形状Z轴角度输入框
		QLineEdit* m_shapeAngleZ = nullptr;
		// 形状X轴位置输入框
		QLineEdit* m_shapePosX = nullptr;
		// 形状Y轴位置输入框
		QLineEdit* m_shapePosY = nullptr;
		// 形状Z轴位置输入框
		QLineEdit* m_shapePosZ = nullptr;

		QListWidget* m_linkList = nullptr;
		QLineEdit* m_linkIndex = nullptr;

		QPushButton* m_addLinkButton = nullptr;
		QPushButton* m_deleteLinkButton = nullptr;

		QLineEdit* m_linkName = nullptr;
		QLineEdit* m_linkAlpha = nullptr;
		QLineEdit* m_linkTheta = nullptr;
		QLineEdit* m_linkDDistance = nullptr;
		QLineEdit* m_linkADistance = nullptr;
		
		QCheckBox* m_checkLink = nullptr;

		QPushButton* m_readShapePathButton = nullptr;
		QPushButton* m_readShapeColorButton = nullptr;

		// 形状预览
		RobotPreview* m_robotPreview = nullptr;
		QWidget* m_linkWidget = nullptr;


	};



}

#endif // !