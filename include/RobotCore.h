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
		void slotWorkRotateShape(double angle, MoveDirection moveType = MoveDirection::MoveDirection_ZAxis);

		void ShapesLink();

		ADDROBOTDATA ShapesRotated(ADDROBOTDATA addRobotData, double angle);
		ADDROBOTDATA ShapesRotated(ADDROBOTDATA addRobotData, gp_Pnt center, double angle);
	public:

		TopoDS_Shape ShapeRotateX(TopoDS_Shape shape, double angleX);
		TopoDS_Shape ShapeRotateY(TopoDS_Shape shape, double angleY);
		TopoDS_Shape ShapeRotateZ(TopoDS_Shape shape, double angleZ);
		TopoDS_Shape ShapeMove(TopoDS_Shape shape, gp_Pnt movePoint);

		Eigen::Vector3d rotateAroundZAxis(const Eigen::Vector3d& point, const Eigen::Vector3d& pivot, double theta);
		Eigen::Vector3d rotateAroundYAxis(const Eigen::Vector3d& point, double theta);
		Eigen::Vector3d rotateAroundXAxis(const Eigen::Vector3d& point, double theta);

	};
}
#endif() // ROBOTPREVIEW_H_