﻿#pragma once

#ifndef ROBOTCORE_H_
#define ROBOTCORE_H_

#include "TcpRobotManger_global.h"
#include "SDHRobot.h"
#include "RobotBase.h"

class QMenu;
class QRubberBand;
namespace TCP_ROBOT
{
	class Vector3D;

	class TCPROBOT_EXPORT RobotCore : public RobotBase
	{
		Q_OBJECT
	public:
		// 构造
		RobotCore(QWidget* parent = nullptr);
		~RobotCore();

		QWidget* initZeroWidget(QWidget* parent = nullptr);
		void initZeroData();

		// 初始化 模型显示
		void initRobotCore();

		QVector<double> getChangedPostion(QString shapeValue);

		void printTransform(const gp_Trsf& transform, const QString& message)
		{
			qDebug() << message.toStdString().c_str();
			qDebug() << "Translation: " << transform.TranslationPart().X() << ", " << transform.TranslationPart().Y() << ", " << transform.TranslationPart().Z();
			qDebug() << "Rotation Matrix:";
			qDebug() << "  " << transform.Value(1, 1) << ", " << transform.Value(1, 2) << ", " << transform.Value(1, 3);
			qDebug() << "  " << transform.Value(2, 1) << ", " << transform.Value(2, 2) << ", " << transform.Value(2, 3);
			qDebug() << "  " << transform.Value(3, 1) << ", " << transform.Value(3, 2) << ", " << transform.Value(3, 3);
		}
		void slotShapMove(QString shapeValue);
		void updateLastCurrentPostion(QString shapeValue);
		void slotInitMove();

		void slotUpdataRobotShaps(void) override;

		void loadWorkShapes(QString filePath);
		void loadRobotShape(QString filePath);
		void loadOtherShape(QString filePath);

		// 工件移动

		// 工件旋转
		void slotShapesRotateShape(
			QMap<QString, ADDROBOTDATA>& robotMap,
			double angle,
			ShapeType shapeType = ShapeType::ShapeType_None,
			MoveDirection moveType = MoveDirection::MoveDirection_ZAxis);

		// 工件 移动
		void slotShapesMoveShape(
			QMap<QString, ADDROBOTDATA>& robotMap,
			double moveDistance,
			ShapeType shapeType = ShapeType::ShapeType_None,
			MoveDirection moveType = MoveDirection::MoveDirection_ZAxis);


		void slotRobotRotateShape(
			QMap<QString, ADDROBOTDATA>& robotMap,
			double angle,
			ShapeType shapeType = ShapeType::ShapeType_None,
			MoveDirection moveType = MoveDirection::MoveDirection_ZAxis,
			int index = -1);

		// 工件 移动
		void slotRobotMoveShape(
			QMap<QString, ADDROBOTDATA>& robotMap,
			double moveDistance,
			ShapeType shapeType = ShapeType::ShapeType_None,
			MoveDirection moveType = MoveDirection::MoveDirection_ZAxis,
			int index = -1);
		void ShapesLink();


		// 模型显示核心迭代
		void ShapesTransformRecursively(QMap<QString, ADDROBOTDATA>& robotMap, QString name, const TRANSFORMDATA& data);
		void ShapesTransform(QMap<QString, ADDROBOTDATA>& robotMap, TRANSFORMDATA data);
		void ShapesCreateTransformAngle(QMap<QString, ADDROBOTDATA>& robotMap, ADDROBOTDATA originData, double angle, MoveDirection moveType = MoveDirection::MoveDirection_ZAxis);
		void ShapesCreateTransformDistance(QMap<QString, ADDROBOTDATA>& robotMap, ADDROBOTDATA originData, double moveDistance, MoveDirection moveType = MoveDirection::MoveDirection_ZAxis);


		// 模型加载核心解析
		void loadShapesModel(QMap<QString, ADDROBOTDATA>& robotMap, SHAPESTRUCT shapeStruct);
		void removeShapeModel(QMap<QString, ADDROBOTDATA>& robotMap, ADDROBOTDATA data);
		void removeAllShapes();


		// 刷新 工件显示
		void updateShapeModel(QMap<QString, ADDROBOTDATA>& robotMap, QString shapeName);
		// 高速 刷新 工件显示
		QVector<Handle(AIS_Shape)> updateShapTrsf(ADDROBOTDATA data);

		void updateShapTrsf(QMap<QString, ADDROBOTDATA>& robotMap);


		// 工件范围
	signals:
		void signalUpdateRobotShaps(bool isUpdate);

	public slots:
		// 自适应显示
		void slotFitAllView() { myView->FitAll(); };
		// 修改指定工件颜色
		void slotChangeShapeColor(QString shapeName, QString color);
		// 显示/隐藏指定工件
		void slotSeletedWorkChanged(QString shapeName);

	public:
		// tools
		// 文件读入
		QVariantMap readJsonFileToMap(QString filePath);
		void writeJsonFileFromMap(QString filePath, QVariantMap map);

		// 工件集合
		QMap<QString, ADDROBOTDATA> m_shapesMap = QMap<QString, ADDROBOTDATA>();
		QMap<QString, ADDROBOTDATA> m_robotMap = QMap<QString, ADDROBOTDATA>();
		QMap<QString, ADDROBOTDATA> m_otherMap = QMap<QString, ADDROBOTDATA>();


		QStringList m_currentPostion = QStringList();
		QString m_lastCurrentPostion = "";
		bool m_isUpdate = true;
		bool m_isActiveMove = false;

		double m_rowRate = 1.0;

		bool m_isActiveUpdateShap = false;

		QString m_currentWorkPath = "";
		QString m_currentRobotPath = "";
		QString m_currentOtherPath = "";

		QString m_zeroPositon = "";

	};
}
#endif() // ROBOTPREVIEW_H_