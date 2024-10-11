#include "../include/RobotPreview.h"

namespace TCP_ROBOT
{
	RobotPreview::RobotPreview(QWidget* parent)
	{

	}
	RobotPreview::~RobotPreview()
	{

	}
	void RobotPreview::slotUpdateModel(WORKANDHOLE workAndHole)
	{
		QMap<QString, ADDROBOTDATA> robotData = createRobotData(workAndHole);
		m_myRobotData = robotData;
		if (m_myRobotData.isEmpty())
		{
			 qDebug() << "slotUpdateModel";
			return;
		}
		myContext->RemoveAll(true);
		addComponentRobotALL();
	}
	
	void RobotPreview::slotUpdateSingleModel(WORKANDHOLE workAndHole, QString holeName)
	{
		QMap<QString, ADDROBOTDATA> robotData = createRobotData(workAndHole);
		foreach(QString key, robotData.keys())
		{
			QStringList robotNames = m_myRobotData.keys();
			if (robotNames.contains(key))
			{
				removeRobotShapes(m_myRobotData[key].myAisShapes);
				m_myRobotData.remove(key);
				m_myRobotData.insert(key, robotData[key]);
			}
			
		}
		addComponentRobotALL();

	}
	QMap<QString, ADDROBOTDATA> RobotPreview::createRobotData(WORKANDHOLE workAndHole)
	{
		// 创建机器人数据
		QMap<QString, ADDROBOTDATA> robotData;

		QMap<QString, HOLEDATA> holeData = workAndHole.holeShape;

		foreach(QString key, holeData.keys())
		{
			HOLEDATA hole = holeData[key];
			ADDROBOTDATA robot;
			robot.name = key;

			// 机器人坐标
			robot.x = hole.positionX.toDouble();
			robot.y = hole.positionY.toDouble();
			robot.z = hole.positionZ.toDouble();

			// 机器人姿态
			robot.angleX = hole.angleX.toDouble();
			robot.angleY = hole.angleY.toDouble();
			robot.angleZ = hole.angleZ.toDouble();

			// 机器人 路径
			robot.path = hole.path;

			// 机器人连杆
			robot.a = hole.aDistance;
			robot.alpha = hole.alpha;
			robot.theta = hole.theta;
			robot.d = hole.dDistance;

			// 机器人尺寸
			robot.sacle = hole.sacle.toDouble();
			
			// 机器人下一个形状名称
			robot.nextShapeName = hole.nextShapeName;

			// 机器人装配点
			robot.assemblyPoint = gp_Pnt(hole.positionX.toDouble(), hole.positionY.toDouble(), hole.positionZ.toDouble());

			// 机器人颜色
			QString colorStr = hole.color;
			// 转换颜色格式
			QColor color(colorStr);
			// 转换为Quantity_Color
			robot.color = Quantity_Color(color.redF(), color.greenF(), color.blueF(), Quantity_TOC_sRGB);
			
			// 机器人坐标系
			robot.xDirCurrent = getTransformationMatrix(robot, robot.xDir);
			robot.yDirCurrent = getTransformationMatrix(robot, robot.yDir);
			robot.zDirCurrent = getTransformationMatrix(robot, robot.zDir);

			// 获取模型
			robot.shapes = getShapesFromResult(robot.path, loadFilesInParallel({ robot.path }));
			robot.shapes = scaleShapes(robot.shapes, robot.sacle);
			// 加入数据
			robotData.insert(key, robot);
		}

		return robotData;
	}
	void RobotPreview::slotUpdataRobotShaps(void)
	{
		displayCoordinateAxes();
		myView->FitAll();
	}
}