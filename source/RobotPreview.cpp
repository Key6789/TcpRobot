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

	}

	void RobotPreview::slotReplaceModelByPath(SHAPESTRUCT shapeStruct)
	{

		// 读取文件
		// 转换为机器人数据
		ADDROBOTDATA robotData = createRobotData(shapeStruct);
		RemovePreview(m_mapPreviewData[shapeStruct.ShapeName]);
		m_mapPreviewData.insert(shapeStruct.ShapeName, robotData);

		displaySingalAddRobot(robotData);
	}

	void RobotPreview::slotChangPreviewColor(SHAPESTRUCT shapeStruct)
	{
		// 修改颜色
		QString colorStr = shapeStruct.ShapeColor;
		// 转换颜色格式
		QColor color(colorStr);
		// 转换为Quantity_Color
		Quantity_Color qColor(color.redF(), color.greenF(), color.blueF(), Quantity_TOC_sRGB);
		// 修改颜色
		for (auto shape : m_mapPreviewData[shapeStruct.ShapeName].myAisShapes)
		{
			shape->SetColor(qColor);
		}
	}

	void RobotPreview::slotChangedPreviewScale(SHAPESTRUCT shapeStruct)
	{
		slotReplaceModelByPath(shapeStruct);
	}

	void RobotPreview::slotChangedPreviewRotation(SHAPESTRUCT shapeStruct)
	{
		slotReplaceModelByPath(shapeStruct);
	}

	void RobotPreview::slotChangedPreviewTranslation(SHAPESTRUCT shapeStruct)
	{
		// 修改位置
		slotReplaceModelByPath(shapeStruct);
	}

	void RobotPreview::removePreview(SHAPESTRUCT shapeStruct)
	{
		RemovePreview(m_mapPreviewData[shapeStruct.ShapeName]);
	}

	void RobotPreview::removeAllPreview()
	{
		getContext()->EraseAll(true);
		m_mapPreviewData.clear();
	}
	
	void RobotPreview::slotUpdateSingleModel(WORKANDHOLE workAndHole, QString holeName)
	{
		

	}
	ADDROBOTDATA RobotPreview::createRobotData(SHAPESTRUCT shapeStruct)
	{
		ADDROBOTDATA robot;
		robot.name = shapeStruct.ShapeName;

		// 机器人坐标
		robot.x = shapeStruct.ShapePositionX.toDouble();
		robot.y = shapeStruct.ShapePositionY.toDouble();
		robot.z = shapeStruct.ShapePositionZ.toDouble();

		// 机器人姿态
		robot.angleX = shapeStruct.ShapeAngleX.toDouble();
		robot.angleY = shapeStruct.ShapeAngleY.toDouble();
		robot.angleZ = shapeStruct.ShapeAngleZ.toDouble();

		// 机器人 路径
		robot.path = shapeStruct.ShapePath;

		// 机器人连杆
		robot.a = shapeStruct.aDistance;
		robot.alpha = shapeStruct.alpha;
		robot.theta = shapeStruct.theta;
		robot.d = shapeStruct.dDistance;

		// 机器人尺寸
		robot.sacle = shapeStruct.ShapeScale.toDouble();

		// 机器人下一个形状名称
		robot.nextShapeName = shapeStruct.nextShapeName;

		// 机器人装配点
		robot.assemblyPoint = gp_Pnt(shapeStruct.ShapePositionX.toDouble(), shapeStruct.ShapePositionY.toDouble(), shapeStruct.ShapePositionZ.toDouble());

		// 机器人颜色
		QString colorStr = shapeStruct.ShapeColor;
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
		robot.myAisShapes = RobotTransformParallelPreview(robot);
		// 加入数据
		return robot;
	}
	void RobotPreview::displaySingalAddRobot(ADDROBOTDATA addRobotData)
	{
		// 显示机器人
		for (auto shape : addRobotData.myAisShapes)
		{
			getContext()->Display(shape, Standard_True);
		}
		
	}
	void RobotPreview::RemovePreview(ADDROBOTDATA addRobotData)
	{
		if (m_mapPreviewData.contains(addRobotData.name))
		{
			for (auto shape : m_mapPreviewData[addRobotData.name].myAisShapes)
			{
				getContext()->Erase(shape,Standard_True);
			}
			m_mapPreviewData.remove(addRobotData.name);
		}
	}
	void RobotPreview::slotUpdataRobotShaps(void)
	{
		displayCoordinateAxes();
		myView->FitAll();
	}
}