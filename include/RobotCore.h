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
			ShapeType shapeType = ShapeType::ShapeType_None,
			MoveDirection moveType = MoveDirection::MoveDirection_ZAxis);

		// 工件 移动
		void slotShapesMoveShape(
			QMap<QString, ADDROBOTDATA>& robotMap,
			double moveDistance,
			ShapeType shapeType = ShapeType::ShapeType_None,
			MoveDirection moveType = MoveDirection::MoveDirection_ZAxis);
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

	public slots:
		// 自适应显示
		void slotFitAllView() { myView->FitAll(); };
		// 修改指定工件颜色
		void slotChangeShapeColor(QString shapeName, QColor color);
		// 显示/隐藏指定工件


	public:
		// tools
		// 文件读入
		QVariantMap readJsonFileToMap(QString filePath);

		// 工件集合
		QMap<QString, ADDROBOTDATA> m_shapesMap = QMap<QString, ADDROBOTDATA>();




	};
}
#endif() // ROBOTPREVIEW_H_