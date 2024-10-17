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
	class Vector3D;

	class TCPROBOT_EXPORT RobotCore : public RobotBase
	{
		Q_OBJECT
	public:
		// 构造
		RobotCore(QWidget* parent = nullptr);
		~RobotCore();

		void slotUpdataRobotShaps(void) override;

		void loadWorkShapes(QString filePath);
		void loadRobotShape(QString filePath);

		// 工件移动

		// 工件旋转
		void slotShapesRotateShape(
			QMap<QString, ADDROBOTDATA>& robotMap,
			double angle,
			ShapeType shapeType = ShapeType::ShapeType_Work,
			MoveDirection moveType = MoveDirection::MoveDirection_ZAxis);

		// 工件 移动
		void slotShapesMoveShape(
			QMap<QString, ADDROBOTDATA>& robotMap,
			double moveDistance,
			ShapeType shapeType = ShapeType::ShapeType_Work,
			MoveDirection moveType = MoveDirection::MoveDirection_ZAxis);
		void ShapesLink();


		// 模型显示核心迭代
		void ShapesTransformRecursively(QMap<QString, ADDROBOTDATA>& robotMap, QString name, const TRANSFORMDATA& data);
		void ShapesTransform(QMap<QString, ADDROBOTDATA>& robotMap, TRANSFORMDATA data);
		void ShapesCreateTransformAngle(QMap<QString, ADDROBOTDATA>& robotMap, ADDROBOTDATA originData, double angle, MoveDirection moveType = MoveDirection::MoveDirection_ZAxis);
		void ShapesCreateTransformDistance(QMap<QString, ADDROBOTDATA>& robotMap, ADDROBOTDATA originData, double moveDistance, MoveDirection moveType = MoveDirection::MoveDirection_ZAxis);
	
	

	public:
		// tools
		// 文件读入
		QVariantMap readJsonFileToMap(QString filePath);
	};
}
#endif() // ROBOTPREVIEW_H_